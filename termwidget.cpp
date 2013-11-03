#include <qwidget.h>

#include "termwidget.h"
#include "socket.h"

TermWidget::TermWidget()
{
    qDebug("calling test_socket");
    
    test_socket();
}

TermWidget::~TermWidget()
{
}
