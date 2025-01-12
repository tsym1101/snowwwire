

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui/QApplication>
#else
#include <QtWidgets/QApplication>
#endif

#include "App/App.h"
#include "Core/SwwAssert.h"
#include "App/UnitTest.h"

int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);

    //SwwTest::UnitTest ut;

    App app;
    int result = app.run(a,argc,argv);
    SWW_DEBUG_VALUE(result);
    //exit(result);
    return result;
}
