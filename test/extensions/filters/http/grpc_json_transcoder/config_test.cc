#include "envoy/extensions/filters/http/grpc_json_transcoder/v3/transcoder.pb.h"
#include "envoy/extensions/filters/http/grpc_json_transcoder/v3/transcoder.pb.validate.h"

#include "source/extensions/filters/http/grpc_json_transcoder/config.h"

#include "test/mocks/server/factory_context.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace GrpcJsonTranscoder {
namespace {

TEST(GrpcJsonTranscoderFilterConfigTest, ValidateFail) {
  NiceMock<Server::Configuration::MockFactoryContext> context;
  envoy::extensions::filters::http::grpc_json_transcoder::v3::GrpcJsonTranscoder proto_config;
  EXPECT_THROW(GrpcJsonTranscoderFilterConfig().createFilterFactoryFromProto(
                   proto_config, "stats", context),
               ProtoValidationException);
  *proto_config.mutable_reflection_cluster_config()->mutable_cluster_name() = "unknown";
  EXPECT_THROW(GrpcJsonTranscoderFilterConfig().createFilterFactoryFromProto(
                   proto_config, "stats", context),
               ProtoValidationException);
}

} // namespace
} // namespace GrpcJsonTranscoder
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
