////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Learning Library (ELL)
//  File:     LoadModel.cpp (common)
//  Authors:  Chuck Jacobs
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "LoadModel.h"

// model
#include "InputNode.h"
#include "Map.h"
#include "Model.h"
#include "OutputNode.h"
#include "SliceNode.h"
#include "SpliceNode.h"

// nodes
#include "AccumulatorNode.h"
#include "BinaryOperationNode.h"
#include "BinaryPredicateNode.h"
#include "BroadcastFunctionNode.h"
#include "BufferNode.h"
#include "ClockNode.h"
#include "ConcatenationNode.h"
#include "DCTNode.h"
#include "DiagonalConvolutionNode.h"
#include "ClockNode.h"
#include "CompiledActivationFunctions.h"
#include "DelayNode.h"
#include "DotProductNode.h"
#include "DTWDistanceNode.h"
#include "ExtremalValueNode.h"
#include "FFTNode.h"
#include "FilterBankNode.h"
#include "ForestPredictorNode.h"
#include "GRUNode.h"
#include "HammingWindowNode.h"
#include "IIRFilterNode.h"
#include "L2NormSquaredNode.h"
#include "LinearPredictorNode.h"
#include "LSTMNode.h"
#include "MatrixMatrixMultiplyNode.h"
#include "MatrixVectorProductNode.h"
#include "MovingAverageNode.h"
#include "MovingVarianceNode.h"
#include "MultiplexerNode.h"
#include "NeuralNetworkPredictorNode.h"
#include "ProtoNNPredictorNode.h"
#include "ReceptiveFieldMatrixNode.h"
#include "ReorderDataNode.h"
#include "RNNNode.h"
#include "SimpleConvolutionNode.h"
#include "SinkNode.h"
#include "SourceNode.h"
#include "UnaryOperationNode.h"
#include "UnrolledConvolutionNode.h"
#include "VoiceActivityDetectorNode.h"
#include "WinogradConvolutionNode.h"

// activations
#include "HardSigmoidActivation.h"
#include "LeakyReLUActivation.h"
#include "ParametricReLUActivation.h"
#include "ReLUActivation.h"
#include "SigmoidActivation.h"
#include "TanhActivation.h"

// predictors
#include "ForestPredictor.h"
#include "LinearPredictor.h"
#include "NeuralNetworkPredictor.h"
#include "SingleElementThresholdPredictor.h"

// utilities
#include "Archiver.h"
#include "Files.h"
#include "JsonArchiver.h"

// stl
#include <cstdint>

using namespace std::string_literals;
using namespace ell::predictors::neural;
using namespace ell::utilities;

namespace ell
{
namespace common
{
    template<typename ElementType>
    void RegisterRealNodeTypes(utilities::SerializationContext& context)
    {
        context.GetTypeFactory().AddType<model::Node, model::InputNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, model::OutputNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, model::SpliceNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, model::SliceNode<ElementType>>();

        context.GetTypeFactory().AddType<model::Node, nodes::AccumulatorNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::ArgMaxNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::ArgMinNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::BinaryOperationNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::BroadcastUnaryFunctionNode<ElementType, nodes::HardSigmoidActivationFunction<ElementType>>>();
        context.GetTypeFactory().AddType<model::Node, nodes::BroadcastUnaryFunctionNode<ElementType, nodes::LeakyReLUActivationFunction<ElementType>>>();
        context.GetTypeFactory().AddType<model::Node, nodes::BroadcastUnaryFunctionNode<ElementType, nodes::ReLUActivationFunction<ElementType>>>();
        context.GetTypeFactory().AddType<model::Node, nodes::BroadcastUnaryFunctionNode<ElementType, nodes::SigmoidActivationFunction<ElementType>>>();
        context.GetTypeFactory().AddType<model::Node, nodes::BroadcastLinearFunctionNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::BufferNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::ConcatenationNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::ConstantNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::DelayNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::DiagonalConvolutionNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::DotProductNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::DTWDistanceNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::FFTNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::GRUNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::HammingWindowNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::L2NormSquaredNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::IIRFilterNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::LinearPredictorNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::LinearFilterBankNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::LSTMNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::MelFilterBankNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::MatrixVectorProductNode<ElementType, math::MatrixLayout::rowMajor>>();
        context.GetTypeFactory().AddType<model::Node, nodes::MatrixVectorProductNode<ElementType, math::MatrixLayout::columnMajor>>();
        context.GetTypeFactory().AddType<model::Node, nodes::MatrixMatrixMultiplyNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::MovingAverageNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::MovingVarianceNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::NeuralNetworkPredictorNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::ReceptiveFieldMatrixNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::ReorderDataNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::RNNNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::SimpleConvolutionNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::SinkNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::SourceNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::SumNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::TypeCastNode<bool, ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::TypeCastNode<int, ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::TypeCastNode<int64_t, ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::TypeCastNode<float, ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::TypeCastNode<double, ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::UnaryOperationNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::UnrolledConvolutionNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::VoiceActivityDetectorNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::WinogradConvolutionNode<ElementType>>();

        // NN layer nodes
        context.GetTypeFactory().AddType<model::Node, nodes::ActivationLayerNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::BatchNormalizationLayerNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::BiasLayerNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::BinaryConvolutionalLayerNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::ConvolutionalLayerNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::FullyConnectedLayerNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::ParametricReLUActivationLayerNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::PoolingLayerNode<ElementType, MeanPoolingFunction>>();
        context.GetTypeFactory().AddType<model::Node, nodes::PoolingLayerNode<ElementType, MaxPoolingFunction>>();
        context.GetTypeFactory().AddType<model::Node, nodes::RegionDetectionLayerNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::ScalingLayerNode<ElementType>>();
        context.GetTypeFactory().AddType<model::Node, nodes::SoftmaxLayerNode<ElementType>>();

        // Activations
        context.GetTypeFactory().AddType<predictors::neural::ActivationImpl<ElementType>, predictors::neural::HardSigmoidActivation<ElementType>>();
        context.GetTypeFactory().AddType<predictors::neural::ActivationImpl<ElementType>, predictors::neural::LeakyReLUActivation<ElementType>>();
        context.GetTypeFactory().AddType<predictors::neural::ActivationImpl<ElementType>, predictors::neural::ParametricReLUActivation<ElementType>>();
        context.GetTypeFactory().AddType<predictors::neural::ActivationImpl<ElementType>, predictors::neural::ReLUActivation<ElementType>>();
        context.GetTypeFactory().AddType<predictors::neural::ActivationImpl<ElementType>, predictors::neural::SigmoidActivation<ElementType>>();
        context.GetTypeFactory().AddType<predictors::neural::ActivationImpl<ElementType>, predictors::neural::TanhActivation<ElementType>>();

        // Map the old type names to the new ones for compatibility reasons.
        context.GetTypeFactory().AddType<model::Node, nodes::ActivationLayerNode<ElementType>>("ActivationLayerNode<"s + TypeName<ElementType>::GetName() + ",SigmoidActivation>");
        context.GetTypeFactory().AddType<model::Node, nodes::ActivationLayerNode<ElementType>>("ActivationLayerNode<"s + TypeName<ElementType>::GetName() + ",HardSigmoidActivation>");
        context.GetTypeFactory().AddType<model::Node, nodes::ActivationLayerNode<ElementType>>("ActivationLayerNode<"s + TypeName<ElementType>::GetName() + ",ReLUActivation>");
        context.GetTypeFactory().AddType<model::Node, nodes::ActivationLayerNode<ElementType>>("ActivationLayerNode<"s + TypeName<ElementType>::GetName() + ",LeakyReLUActivation>");
        context.GetTypeFactory().AddType<model::Node, nodes::ActivationLayerNode<ElementType>>("ActivationLayerNode<"s + TypeName<ElementType>::GetName() + ",TanhActivation>");
        context.GetTypeFactory().AddType<model::Node, nodes::ActivationLayerNode<ElementType>>("ActivationLayerNode<"s + TypeName<ElementType>::GetName() + ",ParametricReLUActivation>");
    }

    void RegisterNodeTypes(utilities::SerializationContext& context)
    {
        RegisterRealNodeTypes<float>(context);
        RegisterRealNodeTypes<double>(context);

        // additional non-real types, only for nodes that support that.
        context.GetTypeFactory().AddType<model::Node, model::InputNode<bool>>();
        context.GetTypeFactory().AddType<model::Node, model::InputNode<int>>();
        context.GetTypeFactory().AddType<model::Node, model::InputNode<int64_t>>();

        context.GetTypeFactory().AddType<model::Node, model::OutputNode<bool>>();
        context.GetTypeFactory().AddType<model::Node, model::OutputNode<int>>();
        context.GetTypeFactory().AddType<model::Node, model::OutputNode<int64_t>>();

        context.GetTypeFactory().AddType<model::Node, model::SpliceNode<bool>>();
        context.GetTypeFactory().AddType<model::Node, model::SpliceNode<int>>();
        context.GetTypeFactory().AddType<model::Node, model::SpliceNode<int64_t>>();

        context.GetTypeFactory().AddType<model::Node, model::SliceNode<bool>>();
        context.GetTypeFactory().AddType<model::Node, model::SliceNode<int>>();
        context.GetTypeFactory().AddType<model::Node, model::SliceNode<int64_t>>();

        context.GetTypeFactory().AddType<model::Node, nodes::AccumulatorNode<int>>();
        context.GetTypeFactory().AddType<model::Node, nodes::AccumulatorNode<int64_t>>();

        context.GetTypeFactory().AddType<model::Node, nodes::ArgMaxNode<int>>();
        context.GetTypeFactory().AddType<model::Node, nodes::ArgMaxNode<int64_t>>();

        context.GetTypeFactory().AddType<model::Node, nodes::ArgMinNode<int>>();
        context.GetTypeFactory().AddType<model::Node, nodes::ArgMinNode<int64_t>>();

        context.GetTypeFactory().AddType<model::Node, nodes::BinaryPredicateNode<int>>();
        context.GetTypeFactory().AddType<model::Node, nodes::BinaryPredicateNode<double>>();

        context.GetTypeFactory().AddType<model::Node, nodes::BroadcastLinearFunctionNode<int>>();

        context.GetTypeFactory().AddType<model::Node, nodes::BufferNode<bool>>();
        context.GetTypeFactory().AddType<model::Node, nodes::BufferNode<int>>();
        context.GetTypeFactory().AddType<model::Node, nodes::BufferNode<int64_t>>();

        context.GetTypeFactory().AddType<model::Node, nodes::ClockNode>();
        context.GetTypeFactory().AddType<model::Node, nodes::SinkNode<int>>();

        context.GetTypeFactory().AddType<model::Node, nodes::ConcatenationNode<bool>>();
        context.GetTypeFactory().AddType<model::Node, nodes::ConcatenationNode<int>>();
        context.GetTypeFactory().AddType<model::Node, nodes::ConcatenationNode<int64_t>>();

        context.GetTypeFactory().AddType<model::Node, nodes::ConstantNode<bool>>();
        context.GetTypeFactory().AddType<model::Node, nodes::ConstantNode<int>>();
        context.GetTypeFactory().AddType<model::Node, nodes::ConstantNode<int64_t>>();

        context.GetTypeFactory().AddType<model::Node, nodes::DCTNode<float>>();
        context.GetTypeFactory().AddType<model::Node, nodes::DCTNode<double>>();

        context.GetTypeFactory().AddType<model::Node, nodes::DelayNode<bool>>();
        context.GetTypeFactory().AddType<model::Node, nodes::DelayNode<int>>();
        context.GetTypeFactory().AddType<model::Node, nodes::DelayNode<int64_t>>();

        context.GetTypeFactory().AddType<model::Node, nodes::DemultiplexerNode<bool, bool>>();

        context.GetTypeFactory().AddType<model::Node, nodes::MultiplexerNode<bool, bool>>();
        context.GetTypeFactory().AddType<model::Node, nodes::MultiplexerNode<int, bool>>();
        context.GetTypeFactory().AddType<model::Node, nodes::MultiplexerNode<int64_t, bool>>();
        context.GetTypeFactory().AddType<model::Node, nodes::MultiplexerNode<float, bool>>();
        context.GetTypeFactory().AddType<model::Node, nodes::MultiplexerNode<double, bool>>();

        context.GetTypeFactory().AddType<model::Node, nodes::ProtoNNPredictorNode>();

        context.GetTypeFactory().AddType<model::Node, nodes::SimpleForestPredictorNode>();

        context.GetTypeFactory().AddType<model::Node, nodes::SingleElementThresholdNode>();

        context.GetTypeFactory().AddType<model::Node, nodes::SumNode<int>>();
        context.GetTypeFactory().AddType<model::Node, nodes::SumNode<int64_t>>();

        context.GetTypeFactory().AddType<model::Node, nodes::TypeCastNode<bool, bool>>();
        context.GetTypeFactory().AddType<model::Node, nodes::TypeCastNode<bool, int>>();
        context.GetTypeFactory().AddType<model::Node, nodes::TypeCastNode<bool, int64_t>>();

        context.GetTypeFactory().AddType<model::Node, nodes::TypeCastNode<int, bool>>();
        context.GetTypeFactory().AddType<model::Node, nodes::TypeCastNode<int, int>>();
        context.GetTypeFactory().AddType<model::Node, nodes::TypeCastNode<int, int64_t>>();

        context.GetTypeFactory().AddType<model::Node, nodes::TypeCastNode<int64_t, bool>>();
        context.GetTypeFactory().AddType<model::Node, nodes::TypeCastNode<int64_t, int>>();
        context.GetTypeFactory().AddType<model::Node, nodes::TypeCastNode<int64_t, int64_t>>();

        context.GetTypeFactory().AddType<model::Node, nodes::TypeCastNode<float, bool>>();
        context.GetTypeFactory().AddType<model::Node, nodes::TypeCastNode<float, int>>();
        context.GetTypeFactory().AddType<model::Node, nodes::TypeCastNode<float, int64_t>>();

        context.GetTypeFactory().AddType<model::Node, nodes::TypeCastNode<double, bool>>();
        context.GetTypeFactory().AddType<model::Node, nodes::TypeCastNode<double, int>>();
        context.GetTypeFactory().AddType<model::Node, nodes::TypeCastNode<double, int64_t>>();
    }

    void RegisterMapTypes(utilities::SerializationContext& context)
    {
        context.GetTypeFactory().AddType<model::Map, model::Map>();
    }

    template <typename UnarchiverType>
    model::Model LoadArchivedModel(std::istream& stream)
    {
        utilities::SerializationContext context;
        RegisterNodeTypes(context);
        UnarchiverType unarchiver(stream, context);
        model::Model model;
        unarchiver.Unarchive(model);
        return model;
    }

    template <typename ArchiverType, typename ObjectType>
    void SaveArchivedObject(const ObjectType& obj, std::ostream& stream)
    {
        ArchiverType archiver(stream);
        archiver.Archive(obj);
    }

    model::Model LoadModel(const std::string& filename)
    {
        if (!utilities::IsFileReadable(filename))
        {
            throw utilities::SystemException(utilities::SystemExceptionErrors::fileNotFound);
        }

        auto filestream = utilities::OpenIfstream(filename);
        return LoadArchivedModel<utilities::JsonUnarchiver>(filestream);
    }

    void SaveModel(const model::Model& model, const std::string& filename)
    {
        if (!utilities::IsFileWritable(filename))
        {
            throw utilities::SystemException(utilities::SystemExceptionErrors::fileNotWritable);
        }
        auto filestream = utilities::OpenOfstream(filename);
        SaveModel(model, filestream);
    }

    void SaveModel(const model::Model& model, std::ostream& outStream)
    {
        SaveArchivedObject<utilities::JsonArchiver>(model, outStream);
    }

    //
    // Map
    //
    model::Map LoadMap(const MapLoadArguments& mapLoadArguments)
    {
        if (mapLoadArguments.HasMapFilename())
        {
            return common::LoadMap(mapLoadArguments.inputMapFilename);
        }
        else if (mapLoadArguments.HasModelFilename())
        {
            auto model = common::LoadModel(mapLoadArguments.inputModelFilename);
            model::InputNodeBase* inputNode = nullptr;
            model::PortElementsBase outputElements;
            if (mapLoadArguments.modelInputsString != "")
            {
                inputNode = mapLoadArguments.GetInput(model);
                if (inputNode == nullptr)
                {
                    throw utilities::InputException(utilities::InputExceptionErrors::invalidArgument, "Can't find input node");
                }
            }
            else // look for first input node
            {
                auto inputNodes = model.GetNodesByType<model::InputNodeBase>();
                if (inputNodes.size() == 0)
                {
                    throw utilities::InputException(utilities::InputExceptionErrors::invalidArgument, "Can't find input node");
                }
                inputNode = inputNodes[0];
            }

            if (mapLoadArguments.modelOutputsString != "")
            {
                outputElements = mapLoadArguments.GetOutput(model);
            }
            else // look for first output node
            {
                auto outputNodes = model.GetNodesByType<model::OutputNodeBase>();
                if (outputNodes.size() == 0)
                {
                    throw utilities::InputException(utilities::InputExceptionErrors::invalidArgument, "Can't find output node");
                }
                auto outputNode = outputNodes[0];
                auto outputPorts = outputNode->GetOutputPorts();
                if (outputPorts.size() == 0)
                {
                    throw utilities::InputException(utilities::InputExceptionErrors::invalidArgument, "Can't find output port");
                }

                auto outputPort = outputPorts[0]; // ptr to port base
                outputElements = model::PortElementsBase(*outputPort);
            }

            return { model, { { "input", inputNode } }, { { "output", outputElements } } };
        }
        else // No model / map file specified -- return an identity map
        {
            model::Model model;
            auto inputNode = model.AddNode<model::InputNode<double>>(mapLoadArguments.defaultInputSize);
            model::PortElements<double> outputElements(inputNode->output);

            return { model, { { "input", inputNode } }, { { "output", outputElements } } };
        }
    }

    model::Map LoadMap(const std::string& filename)
    {
        if (filename == "")
        {
            return model::Map{};
        }

        if (!utilities::IsFileReadable(filename))
        {
            throw utilities::SystemException(utilities::SystemExceptionErrors::fileNotFound);
        }

        auto filestream = utilities::OpenIfstream(filename);
        return LoadArchivedMap<utilities::JsonUnarchiver>(filestream);
    }

    void SaveMap(const model::Map& map, const std::string& filename)
    {
        if (!utilities::IsFileWritable(filename))
        {
            throw utilities::SystemException(utilities::SystemExceptionErrors::fileNotWritable);
        }
        auto filestream = utilities::OpenOfstream(filename);
        SaveMap(map, filestream);
    }

    void SaveMap(const model::Map& map, std::ostream& outStream)
    {
        SaveArchivedObject<utilities::JsonArchiver>(map, outStream);
    }
}
}
