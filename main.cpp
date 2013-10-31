#include <qpe/qpeapplication.h>

#include "termwidget.h"

int main(int argc, char **argv)
{
    QPEApplication app(argc, argv);

    TermWidget tw;
    app.showMainWidget(&tw);

    return app.exec();
}
