#include "GermanTrainer.h"

#include "xlsxdocument.h"

#define tk_bind(elementToBind,signalName,slotName) QObject::connect(elementToBind,SIGNAL(signalName),this,SLOT(slotName))

//Text Label Rescale to fit the Window
void onLabelRescale(QLabel *label,int rescalable)
{
    QFont m_LFont = label->font();

    QString str = label->text();
    while(!rescalable)
    {
        QFontMetrics fm(m_LFont);
        QRect boundRect = fm.boundingRect(0,0,label->size().width(),label->size().height(),Qt::TextWordWrap | Qt::AlignLeft,str);
        if(boundRect.width() <= label->width()-20 && boundRect.height() <= label->height()*1.5) { rescalable = true;}
        else { m_LFont.setPointSizeF(qreal(m_LFont.pointSize() -0.75));}
    }
    label->setFont(m_LFont);
    label->setText(str);

}

void wordsLoader(QString fileName,QList<QString> &germanWords,QList<QString> &translatedWords)
{
    QXlsx::Document wd(fileName);
    if(!wd.load()) { qDebug() << "Error : Cannot load the file"; }
    else
    {
        QString gWord,tWord;
        QString IncompleteCells = "";
        bool emptyCExist = false;
        for(int x = 1; x <= wd.dimension().rowCount(); x++)
        {
             gWord = wd.read("A"+QString::number(x)).toString();
             tWord = wd.read("B"+QString::number(x)).toString();

            if(gWord != "" && tWord != ""){ germanWords.append(gWord);translatedWords.append(tWord);}
            else
            {
                if(gWord != "" && tWord == "" ) { IncompleteCells.append(QString("A"+QString::number(x)+" "));}
                else if(gWord == "" && tWord != "" ) { IncompleteCells.append(QString(" B"+QString::number(x)+" "));}
                emptyCExist = true;
            }
        }
        if(emptyCExist) { qDebug() << "Cells " + IncompleteCells + "are empty !";}
    }

}

GermanTrainer::GermanTrainer()
{
    m_Principal = new QWidget();
    m_Principal->setFixedSize(400,100);

   this->move(930,612);
   this->setStyleSheet("background: transparent; \
                        border-radius: 15px; \
                        color: black;");

    this->setAttribute(Qt::WA_TranslucentBackground);

    this->setWindowFlags(Qt::Sheet | Qt::FramelessWindowHint);
    this->setWindowTitle( QApplication::translate("toplevel", "GermanTrainer"));

    m_Background = new QLabel("",this);
    m_Background->setPixmap(QPixmap("assets/images/background.png"));

    m_Background->resize(400,100);

    m_QuitButton = new QPushButton("x", m_Principal);
    m_QuitButton->setFont(QFont("Arial", 12,10,false));
    m_QuitButton->setStyleSheet("border : 0px; color: white; font-weight: italic;");
    m_QuitButton->move(338+47,0);

    m_WordtoDisplay = new QLabel("Hallo Leute",this);
    m_WordtoDisplay->move(10,m_Background->height()/6);

    //Custom Font Add/Set
    QFontDatabase::addApplicationFont("assets/fonts/Happy Boy.ttf");
    m_WordtoDisplay->setFont(QFont("Happy Boy", 44));
    //m_WordtoDisplay->setFont(QFont("Arial",30,1));

    m_WordtoDisplay->setStyleSheet("color: white");
    m_WordtoDisplay->setAlignment(Qt::AlignCenter);
    m_WordtoDisplay->resize(380,m_WordtoDisplay->height()+10);

    onLabelRescale(m_WordtoDisplay,wordRescalable);

    m_Answer = new QLineEdit("",m_Principal);

    m_Answer->move(10,m_Background->height()*3/5);
    m_Answer->resize(380,m_Answer->height());
    m_Answer->setFont(QFont("Arial",24));
    m_Answer->setAlignment(Qt::AlignCenter);
    if(singleCall) { wordsLoader(xlsxFileName,germanWords,translatedWords); if( germanWords.size() >= 2) { gtWordsNumber = germanWords.size(); randWordNumber = QRandomGenerator::system()->bounded(0,gtWordsNumber-1);} m_WordtoDisplay->setText(germanWords[randWordNumber]); m_Answer->setPlaceholderText("Start"); singleCall = false;}

    m_LangSwitch = new QSlider( Qt::Horizontal,m_Principal);
    m_LangSwitch->setToolTip("Language Switch");
    m_LangSwitch->setObjectName("m_LangSwitch");
    this->setStyleSheet("QSlider#m_LangSwitch::groove:horizontal { image: url('assets/images/uncheked.png');}\
                         QSlider#m_LangSwitch::handle:horizontal { image: url('assets/images/draggable.png'); margin-right: -1px; margin-left: -1px;}");

    m_LangSwitch->move(280,5);

    m_LangSwitch->setRange(0,106);
    m_LangSwitch->setFixedWidth(40);


    tk_bind(m_LangSwitch, sliderReleased(),goBackSlide());

    tk_bind(m_LangSwitch, valueChanged(int), slideModifier(int));

    tk_bind(m_Answer,returnPressed(),verifyAnswer());
//QuitButton Slot
    connect(m_QuitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

    setCentralWidget(m_Principal);
//Focus Outside the Field to Answer
    m_Background->setFocus();

}

void QWidget::paintEvent (QPaintEvent * event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

//Slide Skin Modify
void GermanTrainer::slideModifier(int v)
{

    if(v >= 99)
    {
        this->setStyleSheet("QSlider#m_LangSwitch::groove:horizontal { image: url('assets/images/checked.png');}\
                             QSlider#m_LangSwitch::handle:horizontal { image: url('assets/images/draggable.png'); margin-right: -1px; margin-left: -1px;}");        if (!switched) { QList<QString> container(translatedWords); translatedWords = germanWords;germanWords = container; switched = true; m_WordtoDisplay->setText(germanWords[randWordNumber]);}
        m_Answer->setPlaceholderText("Your Language->German");
        m_Background->setFocus();
    }
    else
    {
        if (switched)  { QList<QString> container(translatedWords); translatedWords = germanWords;germanWords = container; switched = false; m_WordtoDisplay->setText(germanWords[randWordNumber]);}
        this->setStyleSheet(
        "QSlider#m_LangSwitch::groove:horizontal { image: url('assets/images/uncheked.png');}\
         QSlider#m_LangSwitch::handle:horizontal { image: url('assets/images/draggable.png'); margin-right: -1px;  margin-left: -1px;}");
        m_Answer->setPlaceholderText("German->Your Language");
        m_Background->setFocus();
    }
}

//Just for Estetics
void GermanTrainer::goBackSlide() { if(m_LangSwitch->value() == 106) { m_LangSwitch->setValue(99); } }

void GermanTrainer::verifyAnswer()
{
    //Display correct Translation
    m_Answer->setText(translatedWords[randWordNumber]);

    //Let Some time for user to memorize the Correction and display next Word
    QTimer::singleShot(2500, [=]
    {
        randWordNumber = QRandomGenerator::system()->bounded(0, gtWordsNumber - 1);
        m_WordtoDisplay->setText(germanWords[randWordNumber]);
        m_Answer->setText("");
    });

}

//Makes Window Movable
void GermanTrainer::mousePressEvent(QMouseEvent *event)
{

    if(event->button() == Qt::LeftButton)
    {
        moveWidget = true;
        lastPoint = event->pos();
    }

}

void GermanTrainer::mouseMoveEvent(QMouseEvent *event)
{

    if((event->buttons() && Qt::LeftButton) )
    {
        if(moveWidget)
        {
            move( mapToParent(event->pos() - lastPoint) );
        }
    }
}

void GermanTrainer::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)

    {
        if(moveWidget)
        {
           lastPoint = QPoint();
           moveWidget = false;
        }

    }
}

