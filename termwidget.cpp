#include <qwidget.h>
#include <qevent.h>
#include <qfont.h>

#include "socket.h"
#include "telnet.h"
#include "terminal.h"

#include "termwidget.h"

TermWidget::TermWidget()
{
    QFont font("unifont", 16, 50, FALSE, QFont::Unicode);
    setCellFont(font);

    setGeometry(0, 20, cell_width * COLS, cell_height * ROWS);
    
//    qDebug("calling test_socket");
    
//    test_socket();
}

TermWidget::~TermWidget()
{
}

void TermWidget::paintEvent(QPaintEvent *)
{
}

void TermWidget::keyPressEvent(QKeyEvent *k)
{
}

void TermWidget::setCellFont(QFont &font)
{
    cell_font = font;
    cell_font.setFixedPitch(true);
    cell_font.setStyleHint(QFont::TypeWriter);

    QFontMetrics fm(font);
    cell_width = fm.width('X');
    cell_height = fm.height();

    // qDebug("cell_width = %d", cell_width);
    // qDebug("cell_height = %d", cell_height);
    // QString s1 = QString::fromUtf8("test");
    // QString s2 = QString::fromUtf8("中文");
    // qDebug("cell_width_en = %d", fm.width(s1));
    // qDebug("cell_width_zh = %d", fm.width(s2));
}
