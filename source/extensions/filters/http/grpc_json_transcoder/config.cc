#include "source/extensions/filters/http/grpc_json_transcoder/config.h"

#include "envoy/extensions/filters/http/grpc_json_transcoder/v3/transcoder.pb.h"
#include "envoy/extensions/filters/http/grpc_json_transcoder/v3/transcoder.pb.validate.h"
#include "envoy/registry/registry.h"

#include "source/extensions/filters/http/grpc_json_transcoder/json_transcoder_filter.h"
#include "source/common/common/assert.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace GrpcJsonTranscoder {

Http::FilterFactoryCb GrpcJsonTranscoderFilterConfig::createFilterFactoryFromProtoTyped(
    const envoy::extensions::filters::http::grpc_json_transcoder::v3::GrpcJsonTranscoder&
        proto_config,
    const std::string&, Server::Configuration::FactoryContext& context) {

  JsonTranscoderConfigSharedPtr filter_config = createConfig(proto_config, context);
  return [filter_config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
    callbacks.addStreamFilter(std::make_shared<JsonTranscoderFilter>(*filter_config));
  };
}

Router::RouteSpecificFilterConfigConstSharedPtr
GrpcJsonTranscoderFilterConfig::createRouteSpecificFilterConfigTyped(
    const envoy::extensions::filters::http::grpc_json_transcoder::v3::GrpcJsonTranscoder&
        proto_config,
    Server::Configuration::ServerFactoryContext& context, ProtobufMessage::ValidationVisitor&) {
  return createConfig(proto_config, context);
}

JsonTranscoderConfigSharedPtr GrpcJsonTranscoderFilterConfig::createConfig(
    const envoy::extensions::filters::http::grpc_json_transcoder::v3::GrpcJsonTranscoder&
        proto_config,
    Server::Configuration::CommonFactoryContext& context) {
  // Construct descriptor_pool_builder differently depending on whether we need
  // reflection or not. The cases are spelled out mostly for readability; this
  // could be collapsed into a single line with a ternary operator if desired.
  std::shared_ptr<DescriptorPoolBuilder> descriptor_pool_builder;
  switch (proto_config.descriptor_set_case()) {
    case envoy::extensions::filters::http::grpc_json_transcoder::v3::GrpcJsonTranscoder::
        DescriptorSetCase::kProtoDescriptor:
    case envoy::extensions::filters::http::grpc_json_transcoder::v3::GrpcJsonTranscoder::
        DescriptorSetCase::kProtoDescriptorBin:
      descriptor_pool_builder = std::make_shared<DescriptorPoolBuilder>(proto_config, context.api());
      break;
    case envoy::extensions::filters::http::grpc_json_transcoder::v3::GrpcJsonTranscoder::
        DescriptorSetCase::kProtoDescriptorClusterName:
        {
          std::shared_ptr<AsyncReflectionFetcher> async_reflection_fetcher =
            std::make_shared<AsyncReflectionFetcher>(
                proto_config.proto_descriptor_cluster_name(),
                proto_config.services(),
                context.api().timeSource(),
                context.clusterManager(),
                context.initManager());
          // This needs to be injected into descriptor_pool_builder so that it can
          // transitively stay alive long enough to do its job.
          descriptor_pool_builder =
              std::make_shared<DescriptorPoolBuilder>(proto_config, async_reflection_fetcher);
          break;
        }
    default:
      NOT_REACHED_GCOVR_EXCL_LINE;
  }

  return std::make_shared<JsonTranscoderConfig>(proto_config, descriptor_pool_builder);
}

/**
 * Static registration for the grpc transcoding filter. @see RegisterNamedHttpFilterConfigFactory.
 */
REGISTER_FACTORY(GrpcJsonTranscoderFilterConfig,
                 Server::Configuration::NamedHttpFilterConfigFactory){"envoy.grpc_json_transcoder"};

} // namespace GrpcJsonTranscoder
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
