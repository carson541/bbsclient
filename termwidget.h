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
    int IsReading(void);
    void Test(void);

protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);

private:
	QFont cell_font;
    int cell_width;
    int cell_height;
    int socket_fd;
    int reading;
};

#endif /* _TERMWIDGET_H_ */
