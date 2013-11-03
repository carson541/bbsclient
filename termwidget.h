#ifndef _TERMWIDGET_H_
#define _TERMWIDGET_H_

#include <qwidget.h>
#include <qevent.h>
#include <qfont.h>

class TermWidget: public QWidget
{
    Q_OBJECT

public:
    TermWidget();
    ~TermWidget();
    void setCellFont(QFont &font);    

protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);

private:
	QFont cell_font;
    int cell_width;
    int cell_height;
};

#endif /* _TERMWIDGET_H_ */
