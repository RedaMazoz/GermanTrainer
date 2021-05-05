#ifndef GermanTrainer_H
#define GermanTrainer_H

#include <QtGui>
#include <QtWidgets>

#include <qrandom.h>

class GermanTrainer : public QMainWindow
{
Q_OBJECT
public:
    GermanTrainer();
public slots:
    void verifyAnswer();
    void slideModifier(int v);
    void goBackSlide();

//Makes Window Movable
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    QWidget *m_Principal;
    QLabel *m_Background;
    QString xlsxFileName = "GermanTranslations.xlsx";
    QLabel *m_WordtoDisplay;

    int wordRescalable = false;

//Random Word Number Container
    int randWordNumber;

//Data Fetching from Excel File
    QList<QString> germanWords;
    QList<QString> translatedWords;
    int gtWordsNumber = 2;

//to avoid creating a lot of booleans in GUI Loop
    bool singleCall = true;

//Answer and Correction Field
    QLineEdit *m_Answer;

//LangSwitch
    QSlider *m_LangSwitch;
    bool slideClick;
    bool switched = false;

    QPushButton *m_QuitButton;

//Moving Window
    bool moveWidget = false;
    QPoint lastPoint;
};

#endif // CRYSTALIS_H
