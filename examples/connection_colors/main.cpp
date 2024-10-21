#include "QtNodes/internal/ConnectionStyle.hpp"
#include "QtNodes/internal/StyleCollection.hpp"
#include "models.hpp"
#include <memory>
#include <QApplication>
#include <QtNodes/DataFlowGraphicsScene>
#include <QtNodes/GraphicsView>

using QtNodes::ConnectionStyle;
using QtNodes::DataFlowGraphicsScene;
using QtNodes::DataFlowGraphModel;
using QtNodes::GraphicsView;
using QtNodes::NodeDelegateModelRegistry;

static std::shared_ptr<NodeDelegateModelRegistry> registerDataModels()
{
    auto ret = std::make_shared<NodeDelegateModelRegistry>();

    ret->registerModel<NaiveDataModel>();

    /*
     We could have more models registered.
     All of them become items in the context meny of the scene.

     ret->registerModel<AnotherDataModel>();
     ret->registerModel<OneMoreDataModel>();

   */

    return ret;
}

static void setStyle()
{
    ConnectionStyle::setConnectionStyle(
        R"(
  {
    "ConnectionStyle": {
      "UseDataDefinedColors": true
    }
  }
  )");
}

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    setStyle();

    std::shared_ptr<NodeDelegateModelRegistry> registry = registerDataModels();
    DataFlowGraphModel dataFlowGraphModel(registry);

    DataFlowGraphicsScene scene(dataFlowGraphModel);

    GraphicsView view(&scene);

    view.setWindowTitle("Node-based flow editor");
    view.resize(800, 600);
    view.show();

    return app.exec();
}
