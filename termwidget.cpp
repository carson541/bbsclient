#include <qwidget.h>
#include <qevent.h>
#include <qfont.h>
#include <qtimer.h>
#include <qcolor.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qtextcodec.h>

#include <stdio.h>
#ifdef USE_ICONV
#include <iconv.h>
#endif

#include "socket.h"
#include "telnet.h"
#include "terminal.h"
#include "thread.h"
#include "screen.h"

#include "termwidget.h"

static const QColor palette_xterm[] = { /* AARRGGBB */
    0xff000000, /* black */
    0xffcd0000, /* red3 */
    0xff00cd00, /* green3 */
    0xffcdcd00, /* yellow3 */
    0xff0000ee, /* blue2 */
    0xffcd00cd, /* magenta3 */
    0xff00cdcd, /* cyan3 */
    0xffe5e5e5, /* gray90 */
    0xff7f7f7f, /* gray50 */
    0xffff0000, /* red */
    0xff00ff00, /* green */
    0xffffff00, /* yellow */
    0xff5c5cff,
    0xffff00ff, /* magenta */
    0xff00ffff, /* cyan */
    0xffffffff  /* white */
};

void callback_func(void *);
void draw(TermWidget *w);

TermWidget::TermWidget()
{
    setCaption("BBS Client");

    QFont font("unifont", 16, 50, FALSE, QFont::Unicode);
    setCellFont(font);

    setGeometry(0, 25, cell_width * COLS, cell_height * ROWS);
    
    QRect r(0, 0, cell_width * COLS, cell_height * ROWS);
    pix.resize(r.size());

//    setBackgroundColor((QColor)0xff000000);
    setBackgroundMode(NoBackground);
        
//    qDebug("calling test_socket");
    
//    test_socket();

    redrawTimer = new QTimer(this);
    connect(redrawTimer, SIGNAL(timeout()),
         this, SLOT(doRedraw()));
    redrawTimer->start(1000);
    
    treset();

    telnet_init();

    connected = 0;
}

TermWidget::~TermWidget()
{
    socket_close();
}

void TermWidget::paintEvent(QPaintEvent *)
{
//    qDebug("paintEvent");

    QRect r(0, 0, cell_width * COLS, cell_height * ROWS);

    bitBlt(this, r.topLeft(), &pix);
}

void TermWidget::keyPressEvent(QKeyEvent *k)
{
    switch(k->key()) {
    case Qt::Key_Return:
        if(!connected) {
            socket_fd = socket_create();
            connected = 1;
            new_thread((void *)callback_func, (void *)this);
            return;
        }
        break;
    default:
        break;
    }

#if 1
    if(!(k->text().isEmpty())) {
        int len = k->text().length();
        qDebug("len = %d", len);
        QByteArray data = k->text().local8Bit();
        char *s = data.data();
        int n = data.count();
        printf("n(1): %d\n", n);
        for(int i = 0; i < n; i++) {
            printf("%.2x ", (unsigned char)s[i]);
        }
        printf("\n");
        QTextCodec *codec = QTextCodec::codecForName("GBK");
        if(codec == NULL) { // try iconv
#ifdef USE_ICONV
            iconv_t cd = iconv_open("GBK", "UTF-8"); // UTF8 -> GBK
            char outbuf[16];
            int len, inlen, outlen;
            char *in = s;
            char *out = outbuf;
            inlen = n; outlen = 16;
            if(cd != NULL) {
                len = iconv(cd, &in, (size_t *)&inlen,
                            &out, (size_t *)&outlen);
            } else {
            }
            iconv_close(cd);
            if(len != -1) {
                printf("n(3): %d\n", 16 - outlen);
                for(int i = 0; i < 16 - outlen; i++) {
                    printf("%.2x ", (unsigned char)outbuf[i]);
                }
                printf("\n");
            } else {
            }
#endif
        } else { // use text codec
            QByteArray qbstr = codec->fromUnicode(k->text());
            char *s2 = qbstr.data();
            int n2 = qbstr.count();
            printf("n(4): %d\n", n2);
            for(int i = 0; i < n2; i++) {
                printf("%.2x ", (unsigned char)s2[i]);
            }
            printf("\n");
        }
    }
#endif

    if(!connected) return;

    if(k->text().isEmpty()) {
        return;
    }

    QByteArray data = k->text().local8Bit();
    char *s = data.data();
    int n = data.count();
    if(n > 1 && (s[n-1] == '\0')) n--;
    socket_writen(s, n);
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

int TermWidget::IsConnected(void)
{
    return connected;
}

void TermWidget::resetConnected(void)
{
    connected = 0;
}

void TermWidget::Test(void)
{
    qDebug("test");
}

void TermWidget::doRedraw()
{
//    qDebug("doRedraw");

    draw(this);
}

int TermWidget::getCellWidth(void)
{
    return cell_width;
}

int TermWidget::getCellHeight(void)
{
    return cell_height;
}

QFont TermWidget::getCellFont(void)
{
    return cell_font;
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
    
//    qDebug("<callback_func>");

//    p->Test();
    
    while(p->IsConnected()) {
        n = socket_read(buf, 16);

#if 0
        // dump
        printf("read: %d\n", n);
        for(i = 0; i < n; i++) {
            if(i % 16 == 0 && i != 0) printf("\n");
            printf("%.2x ", (unsigned char)buf[i]);
        }
        printf("\n");
#endif

        for(i = 0; i < n; i++) {
            echo = telnet_parse((unsigned char)buf[i]);
            if(echo) tputc(buf[i]);
        }

        if(n <= 0) break;
    }

    p->resetConnected();
    
    treset();

    exit_threadfunc();
}

void draw(TermWidget *w)
{
    int cell_width = w->getCellWidth();
    int cell_height = w->getCellHeight();

    QRect r(0, 0, cell_width * COLS, cell_height * ROWS);
    w->pix.fill(w, r.topLeft());

    QPainter p(&w->pix);
    screen_redraw(w, &p);
    p.end();

    w->update();
}

extern "C" void test_ui_callback(void)
{
    qDebug("<test_ui_callback>");
}

extern "C" void xclear(void *context, void *drawable,
                       int x1, int y1, int x2, int y2)
{
    TermWidget *w = (TermWidget *)context;
    QPainter *p = (QPainter *)drawable;

    int cell_width = w->getCellWidth();
    int cell_height = w->getCellHeight();

    QRect rect(x1 * cell_width, y1 * cell_height,
               cell_width * (x2 - x1 + 1), cell_height * (y2 - y1 + 1));

    p->fillRect(rect, palette_xterm[cursor_bg]);
}

extern "C" void xdraws(void *context, void *drawable,
                       int mode, int fg, int bg,
                       char *str, int x, int y, int charlen, int bytelen)
{
    TermWidget *w = (TermWidget *)context;
    QPainter *p = (QPainter *)drawable;

    QFont cell_font = w->getCellFont();
    int cell_width = w->getCellWidth();
    int cell_height = w->getCellHeight();

    p->setFont(cell_font);
    
    int temp;

    if(mode & ATTR_REVERSE) {
        temp = fg, fg = bg, bg = temp;
    }

    if(mode & ATTR_BOLD) {
        if(fg < 8) {
            fg += 8;
        }
    }

    QColor color;
    color = palette_xterm[fg];

    QRect rect(x * cell_width, y * cell_height,
        cell_width * bytelen, cell_height);

    p->fillRect(rect, palette_xterm[bg]);

    p->setPen(color);

    str[charlen] = '\0';

    QTextCodec *codec = QTextCodec::codecForName("GBK");
    if(codec == NULL) { // try iconv
#ifdef USE_ICONV
        int iconv_fail = 0;
        iconv_t cd = iconv_open("UTF-8", "GBK"); // GBK -> UTF8
        char outbuf[80 * 4];
        int len = -1, inlen, outlen;
        char *in = str;
        char *out = outbuf;
        inlen = charlen; outlen = 80 * 4;
        if(cd != NULL) {
            len = iconv(cd, &in, (size_t *)&inlen,
                        &out, (size_t *)&outlen);
        } else {
            iconv_fail = 1;
        }
        iconv_close(cd);
        if(len != -1) {
            QString qstr;
            QByteArray qbtr;
            qbtr.duplicate(outbuf, 80 *4 - outlen);
            qstr = QString::fromUtf8(qbtr, 80 *4 - outlen);
            p->drawText(rect, Qt::SingleLine, qstr);
        } else {
            iconv_fail = 1;
        }
#else
        int iconv_fail = 0;
#endif
        
        if(iconv_fail) {
//        qDebug("GBK codec not found, fallback to ISO8859-1");
            codec = QTextCodec::codecForName("ISO8859-1");
            if(codec == NULL) {
//        qDebug("codec not found");
            }
            p->drawText(rect, Qt::SingleLine, str);
        }
    } else { // use text codec
        QString qstr;
        QByteArray qbtr;
        qbtr.duplicate(str, charlen);
        qstr = codec->toUnicode(qbtr);
        p->drawText(rect, Qt::SingleLine, qstr);
    }
}

extern "C" void xdrawcursor(void *context, void *drawable,
                            int cursor_x, int cursor_y)
{
    TermWidget *w = (TermWidget *)context;
    QPainter *p = (QPainter *)drawable;

    int cell_width = w->getCellWidth();
    int cell_height = w->getCellHeight();

    QRect rect(cursor_x * cell_width, cursor_y * cell_height,
        cell_width, cell_height);

    QRect r = rect;

    p->drawRect(r);
}
