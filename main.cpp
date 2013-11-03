#include <qpe/qpeapplication.h>

#include "termwidget.h"

int main(int argc, char **argv)
{
    QPEApplication app(argc, argv);

    TermWidget tw;
    app.setMainWidget(&tw);
    tw.show();

    return app.exec();
}
