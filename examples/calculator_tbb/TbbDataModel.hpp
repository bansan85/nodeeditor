#pragma once

#pragma once

#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>

#include <iostream>
#include <tbb/tbb.h>

#include <ranges>
#include <unordered_set>

using QtNodes::ConnectionId;
using QtNodes::DataFlowGraphModel;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDelegateModel;
using QtNodes::NodeId;
using QtNodes::PortIndex;
using QtNodes::PortType;

using oneapi::tbb::flow::broadcast_node;
using oneapi::tbb::flow::graph_node;

class TbbDataModelVoid : public NodeDelegateModel
{
public:
    virtual void constructTbbGraph(const std::vector<TbbDataModelVoid *> &range,
                                   const std::unordered_set<ConnectionId> &connectIds,
                                   tbb::flow::graph &g,
                                   DataFlowGraphModel &graphModel,
                                   NodeId nodeId)
        = 0;

    virtual void tryPutTbb() = 0;
    virtual void free() = 0;

    template<typename T>
    oneapi::tbb::flow::sender<T> *getTbbSender(PortIndex idx)
    {
        return reinterpret_cast<oneapi::tbb::flow::sender<T> *>(getTbbSenderSub(idx));
    }

protected:
    virtual void *getTbbSenderSub(PortIndex idx) = 0;
};

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
template<typename Input, typename Output>
class TbbDataModel : public TbbDataModelVoid
{};
