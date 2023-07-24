//
//  Generated code. Do not modify.
//  source: fk-app.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

class QueryFlags extends $pb.ProtobufEnum {
  static const QueryFlags QUERY_FLAGS_NONE = QueryFlags._(0, _omitEnumNames ? '' : 'QUERY_FLAGS_NONE');
  static const QueryFlags QUERY_FLAGS_LOGS = QueryFlags._(1, _omitEnumNames ? '' : 'QUERY_FLAGS_LOGS');

  static const $core.List<QueryFlags> values = <QueryFlags> [
    QUERY_FLAGS_NONE,
    QUERY_FLAGS_LOGS,
  ];

  static final $core.Map<$core.int, QueryFlags> _byValue = $pb.ProtobufEnum.initByValue(values);
  static QueryFlags? valueOf($core.int value) => _byValue[value];

  const QueryFlags._($core.int v, $core.String n) : super(v, n);
}

class QueryType extends $pb.ProtobufEnum {
  static const QueryType QUERY_NONE = QueryType._(0, _omitEnumNames ? '' : 'QUERY_NONE');
  static const QueryType QUERY_CAPABILITIES = QueryType._(1, _omitEnumNames ? '' : 'QUERY_CAPABILITIES');
  static const QueryType QUERY_CONFIGURE_SENSOR = QueryType._(2, _omitEnumNames ? '' : 'QUERY_CONFIGURE_SENSOR');
  static const QueryType QUERY_LIVE_DATA_POLL = QueryType._(7, _omitEnumNames ? '' : 'QUERY_LIVE_DATA_POLL');
  static const QueryType QUERY_SCHEDULES = QueryType._(8, _omitEnumNames ? '' : 'QUERY_SCHEDULES');
  static const QueryType QUERY_CONFIGURE_SCHEDULES = QueryType._(9, _omitEnumNames ? '' : 'QUERY_CONFIGURE_SCHEDULES');
  static const QueryType QUERY_FILES_SD = QueryType._(10, _omitEnumNames ? '' : 'QUERY_FILES_SD');
  static const QueryType QUERY_DOWNLOAD_FILE = QueryType._(11, _omitEnumNames ? '' : 'QUERY_DOWNLOAD_FILE');
  static const QueryType QUERY_ERASE_FILE = QueryType._(12, _omitEnumNames ? '' : 'QUERY_ERASE_FILE');
  static const QueryType QUERY_RESET = QueryType._(13, _omitEnumNames ? '' : 'QUERY_RESET');
  static const QueryType QUERY_NETWORK_SETTINGS = QueryType._(14, _omitEnumNames ? '' : 'QUERY_NETWORK_SETTINGS');
  static const QueryType QUERY_CONFIGURE_NETWORK_SETTINGS = QueryType._(15, _omitEnumNames ? '' : 'QUERY_CONFIGURE_NETWORK_SETTINGS');
  static const QueryType QUERY_SCAN_MODULES = QueryType._(16, _omitEnumNames ? '' : 'QUERY_SCAN_MODULES');
  static const QueryType QUERY_CONFIGURE_IDENTITY = QueryType._(17, _omitEnumNames ? '' : 'QUERY_CONFIGURE_IDENTITY');
  static const QueryType QUERY_STATUS = QueryType._(18, _omitEnumNames ? '' : 'QUERY_STATUS');
  static const QueryType QUERY_MODULE = QueryType._(19, _omitEnumNames ? '' : 'QUERY_MODULE');
  static const QueryType QUERY_METADATA = QueryType._(20, _omitEnumNames ? '' : 'QUERY_METADATA');
  static const QueryType QUERY_FORMAT = QueryType._(21, _omitEnumNames ? '' : 'QUERY_FORMAT');
  static const QueryType QUERY_GET_READINGS = QueryType._(22, _omitEnumNames ? '' : 'QUERY_GET_READINGS');
  static const QueryType QUERY_TAKE_READINGS = QueryType._(23, _omitEnumNames ? '' : 'QUERY_TAKE_READINGS');
  static const QueryType QUERY_RECORDING_CONTROL = QueryType._(24, _omitEnumNames ? '' : 'QUERY_RECORDING_CONTROL');
  static const QueryType QUERY_CONFIGURE = QueryType._(25, _omitEnumNames ? '' : 'QUERY_CONFIGURE');
  static const QueryType QUERY_SCAN_NETWORKS = QueryType._(26, _omitEnumNames ? '' : 'QUERY_SCAN_NETWORKS');
  static const QueryType QUERY_FILES_SPI = QueryType._(27, _omitEnumNames ? '' : 'QUERY_FILES_SPI');
  static const QueryType QUERY_FILES_QSPI = QueryType._(28, _omitEnumNames ? '' : 'QUERY_FILES_QSPI');

  static const $core.List<QueryType> values = <QueryType> [
    QUERY_NONE,
    QUERY_CAPABILITIES,
    QUERY_CONFIGURE_SENSOR,
    QUERY_LIVE_DATA_POLL,
    QUERY_SCHEDULES,
    QUERY_CONFIGURE_SCHEDULES,
    QUERY_FILES_SD,
    QUERY_DOWNLOAD_FILE,
    QUERY_ERASE_FILE,
    QUERY_RESET,
    QUERY_NETWORK_SETTINGS,
    QUERY_CONFIGURE_NETWORK_SETTINGS,
    QUERY_SCAN_MODULES,
    QUERY_CONFIGURE_IDENTITY,
    QUERY_STATUS,
    QUERY_MODULE,
    QUERY_METADATA,
    QUERY_FORMAT,
    QUERY_GET_READINGS,
    QUERY_TAKE_READINGS,
    QUERY_RECORDING_CONTROL,
    QUERY_CONFIGURE,
    QUERY_SCAN_NETWORKS,
    QUERY_FILES_SPI,
    QUERY_FILES_QSPI,
  ];

  static final $core.Map<$core.int, QueryType> _byValue = $pb.ProtobufEnum.initByValue(values);
  static QueryType? valueOf($core.int value) => _byValue[value];

  const QueryType._($core.int v, $core.String n) : super(v, n);
}

class ReplyType extends $pb.ProtobufEnum {
  static const ReplyType REPLY_NONE = ReplyType._(0, _omitEnumNames ? '' : 'REPLY_NONE');
  static const ReplyType REPLY_SUCCESS = ReplyType._(1, _omitEnumNames ? '' : 'REPLY_SUCCESS');
  static const ReplyType REPLY_BUSY = ReplyType._(2, _omitEnumNames ? '' : 'REPLY_BUSY');
  static const ReplyType REPLY_ERROR = ReplyType._(3, _omitEnumNames ? '' : 'REPLY_ERROR');
  static const ReplyType REPLY_CAPABILITIES = ReplyType._(4, _omitEnumNames ? '' : 'REPLY_CAPABILITIES');
  static const ReplyType REPLY_LIVE_DATA_POLL = ReplyType._(8, _omitEnumNames ? '' : 'REPLY_LIVE_DATA_POLL');
  static const ReplyType REPLY_SCHEDULES = ReplyType._(9, _omitEnumNames ? '' : 'REPLY_SCHEDULES');
  static const ReplyType REPLY_FILES = ReplyType._(10, _omitEnumNames ? '' : 'REPLY_FILES');
  static const ReplyType REPLY_DOWNLOAD_FILE = ReplyType._(11, _omitEnumNames ? '' : 'REPLY_DOWNLOAD_FILE');
  static const ReplyType REPLY_RESET = ReplyType._(12, _omitEnumNames ? '' : 'REPLY_RESET');
  static const ReplyType REPLY_NETWORK_SETTINGS = ReplyType._(13, _omitEnumNames ? '' : 'REPLY_NETWORK_SETTINGS');
  static const ReplyType REPLY_IDENTITY = ReplyType._(14, _omitEnumNames ? '' : 'REPLY_IDENTITY');
  static const ReplyType REPLY_STATUS = ReplyType._(15, _omitEnumNames ? '' : 'REPLY_STATUS');
  static const ReplyType REPLY_MODULE = ReplyType._(16, _omitEnumNames ? '' : 'REPLY_MODULE');
  static const ReplyType REPLY_METADATA = ReplyType._(17, _omitEnumNames ? '' : 'REPLY_METADATA');
  static const ReplyType REPLY_READINGS = ReplyType._(18, _omitEnumNames ? '' : 'REPLY_READINGS');
  static const ReplyType REPLY_NETWORKS = ReplyType._(19, _omitEnumNames ? '' : 'REPLY_NETWORKS');

  static const $core.List<ReplyType> values = <ReplyType> [
    REPLY_NONE,
    REPLY_SUCCESS,
    REPLY_BUSY,
    REPLY_ERROR,
    REPLY_CAPABILITIES,
    REPLY_LIVE_DATA_POLL,
    REPLY_SCHEDULES,
    REPLY_FILES,
    REPLY_DOWNLOAD_FILE,
    REPLY_RESET,
    REPLY_NETWORK_SETTINGS,
    REPLY_IDENTITY,
    REPLY_STATUS,
    REPLY_MODULE,
    REPLY_METADATA,
    REPLY_READINGS,
    REPLY_NETWORKS,
  ];

  static final $core.Map<$core.int, ReplyType> _byValue = $pb.ProtobufEnum.initByValue(values);
  static ReplyType? valueOf($core.int value) => _byValue[value];

  const ReplyType._($core.int v, $core.String n) : super(v, n);
}

class DownloadFlags extends $pb.ProtobufEnum {
  static const DownloadFlags DOWNLOAD_FLAG_NONE = DownloadFlags._(0, _omitEnumNames ? '' : 'DOWNLOAD_FLAG_NONE');
  static const DownloadFlags DOWNLOAD_FLAG_METADATA_PREPEND = DownloadFlags._(1, _omitEnumNames ? '' : 'DOWNLOAD_FLAG_METADATA_PREPEND');
  static const DownloadFlags DOWNLOAD_FLAG_METADATA_ONLY = DownloadFlags._(2, _omitEnumNames ? '' : 'DOWNLOAD_FLAG_METADATA_ONLY');

  static const $core.List<DownloadFlags> values = <DownloadFlags> [
    DOWNLOAD_FLAG_NONE,
    DOWNLOAD_FLAG_METADATA_PREPEND,
    DOWNLOAD_FLAG_METADATA_ONLY,
  ];

  static final $core.Map<$core.int, DownloadFlags> _byValue = $pb.ProtobufEnum.initByValue(values);
  static DownloadFlags? valueOf($core.int value) => _byValue[value];

  const DownloadFlags._($core.int v, $core.String n) : super(v, n);
}

class ModuleFlags extends $pb.ProtobufEnum {
  static const ModuleFlags MODULE_FLAG_NONE = ModuleFlags._(0, _omitEnumNames ? '' : 'MODULE_FLAG_NONE');
  static const ModuleFlags MODULE_FLAG_INTERNAL = ModuleFlags._(1, _omitEnumNames ? '' : 'MODULE_FLAG_INTERNAL');

  static const $core.List<ModuleFlags> values = <ModuleFlags> [
    MODULE_FLAG_NONE,
    MODULE_FLAG_INTERNAL,
  ];

  static final $core.Map<$core.int, ModuleFlags> _byValue = $pb.ProtobufEnum.initByValue(values);
  static ModuleFlags? valueOf($core.int value) => _byValue[value];

  const ModuleFlags._($core.int v, $core.String n) : super(v, n);
}

class SensorFlags extends $pb.ProtobufEnum {
  static const SensorFlags SENSOR_FLAG_NONE = SensorFlags._(0, _omitEnumNames ? '' : 'SENSOR_FLAG_NONE');

  static const $core.List<SensorFlags> values = <SensorFlags> [
    SENSOR_FLAG_NONE,
  ];

  static final $core.Map<$core.int, SensorFlags> _byValue = $pb.ProtobufEnum.initByValue(values);
  static SensorFlags? valueOf($core.int value) => _byValue[value];

  const SensorFlags._($core.int v, $core.String n) : super(v, n);
}

class ModuleQueryType extends $pb.ProtobufEnum {
  static const ModuleQueryType MODULE_QUERY_NONE = ModuleQueryType._(0, _omitEnumNames ? '' : 'MODULE_QUERY_NONE');
  static const ModuleQueryType MODULE_QUERY_STATUS = ModuleQueryType._(1, _omitEnumNames ? '' : 'MODULE_QUERY_STATUS');
  static const ModuleQueryType MODULE_QUERY_CONFIGURE = ModuleQueryType._(2, _omitEnumNames ? '' : 'MODULE_QUERY_CONFIGURE');
  static const ModuleQueryType MODULE_QUERY_RESET = ModuleQueryType._(3, _omitEnumNames ? '' : 'MODULE_QUERY_RESET');

  static const $core.List<ModuleQueryType> values = <ModuleQueryType> [
    MODULE_QUERY_NONE,
    MODULE_QUERY_STATUS,
    MODULE_QUERY_CONFIGURE,
    MODULE_QUERY_RESET,
  ];

  static final $core.Map<$core.int, ModuleQueryType> _byValue = $pb.ProtobufEnum.initByValue(values);
  static ModuleQueryType? valueOf($core.int value) => _byValue[value];

  const ModuleQueryType._($core.int v, $core.String n) : super(v, n);
}

class ModuleReplyType extends $pb.ProtobufEnum {
  static const ModuleReplyType MODULE_REPLY_NONE = ModuleReplyType._(0, _omitEnumNames ? '' : 'MODULE_REPLY_NONE');
  static const ModuleReplyType MODULE_REPLY_SUCCESS = ModuleReplyType._(1, _omitEnumNames ? '' : 'MODULE_REPLY_SUCCESS');
  static const ModuleReplyType MODULE_REPLY_BUSY = ModuleReplyType._(2, _omitEnumNames ? '' : 'MODULE_REPLY_BUSY');
  static const ModuleReplyType MODULE_REPLY_ERROR = ModuleReplyType._(3, _omitEnumNames ? '' : 'MODULE_REPLY_ERROR');

  static const $core.List<ModuleReplyType> values = <ModuleReplyType> [
    MODULE_REPLY_NONE,
    MODULE_REPLY_SUCCESS,
    MODULE_REPLY_BUSY,
    MODULE_REPLY_ERROR,
  ];

  static final $core.Map<$core.int, ModuleReplyType> _byValue = $pb.ProtobufEnum.initByValue(values);
  static ModuleReplyType? valueOf($core.int value) => _byValue[value];

  const ModuleReplyType._($core.int v, $core.String n) : super(v, n);
}

class UdpStatus extends $pb.ProtobufEnum {
  static const UdpStatus UDP_STATUS_ONLINE = UdpStatus._(0, _omitEnumNames ? '' : 'UDP_STATUS_ONLINE');
  static const UdpStatus UDP_STATUS_BYE = UdpStatus._(1, _omitEnumNames ? '' : 'UDP_STATUS_BYE');

  static const $core.List<UdpStatus> values = <UdpStatus> [
    UDP_STATUS_ONLINE,
    UDP_STATUS_BYE,
  ];

  static final $core.Map<$core.int, UdpStatus> _byValue = $pb.ProtobufEnum.initByValue(values);
  static UdpStatus? valueOf($core.int value) => _byValue[value];

  const UdpStatus._($core.int v, $core.String n) : super(v, n);
}


const _omitEnumNames = $core.bool.fromEnvironment('protobuf.omit_enum_names');
