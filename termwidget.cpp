#include <qwidget.h>
#include <qevent.h>
#include <qfont.h>
#include <qtimer.h>
#include <qcolor.h>
#include <qpainter.h>

#include <stdio.h>

#include "socket.h"
#include "telnet.h"
#include "terminal.h"
#include "thread.h"

#include "termwidget.h"

void callback_func(void *);
void draw(QWidget *w);

TermWidget::TermWidget()
{
    QFont font("unifont", 16, 50, FALSE, QFont::Unicode);
    setCellFont(font);

    setGeometry(0, 20, cell_width * COLS, cell_height * ROWS);
    
//    qDebug("calling test_socket");
    
//    test_socket();

    redrawTimer = new QTimer(this);
    connect(redrawTimer, SIGNAL(timeout()),
         this, SLOT(doRedraw()));
    redrawTimer->start(1000);
    
    treset();

    telnet_init();

    socket_fd = socket_create();

    // create a thread for read
    reading = 1;
    new_thread((void *)callback_func, (void *)this);
}

TermWidget::~TermWidget()
{
    socket_close();
}

void TermWidget::paintEvent(QPaintEvent *)
{
    qDebug("paintEvent");
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

int TermWidget::IsReading(void)
{
    return reading;
}

void TermWidget::Test(void)
{
    qDebug("test");
}

void TermWidget::doRedraw()
{
    qDebug("doRedraw");

    draw(this);
}

/*
 * called by thread
 */ 
void callback_func(void *arg)
{
    TermWidget *p = (TermWidget*)arg;
    int n, i;
    char buf[16];
    int echo;
    
    qDebug("<callback_func>");

    p->Test();
    
    while(p->IsReading()) {
        n = socket_read(buf, 16);

        // dump
        printf("read: %d\n", n);
        for(i = 0; i < n; i++) {
            if(i % 16 == 0 && i != 0) printf("\n");
            printf("%.2x ", (unsigned char)buf[i]);
        }
        printf("\n");

        for(i = 0; i < n; i++) {
            echo = telnet_parse((unsigned char)buf[i]);
            if(echo) tputc(buf[i]);
        }

        if(n <= 0) break;
    }
    
    exit_threadfunc();
}

void draw(QWidget *w)
{
    w->update();
}
