///
//  Generated code. Do not modify.
//  source: fk-app.proto
//
// @dart = 2.12
// ignore_for_file: annotate_overrides,camel_case_types,constant_identifier_names,deprecated_member_use_from_same_package,directives_ordering,library_prefixes,non_constant_identifier_names,prefer_final_fields,return_of_invalid_type,unnecessary_const,unnecessary_import,unnecessary_this,unused_import,unused_shown_name

import 'dart:core' as $core;
import 'dart:convert' as $convert;
import 'dart:typed_data' as $typed_data;
@$core.Deprecated('Use queryFlagsDescriptor instead')
const QueryFlags$json = const {
  '1': 'QueryFlags',
  '2': const [
    const {'1': 'QUERY_FLAGS_NONE', '2': 0},
    const {'1': 'QUERY_FLAGS_LOGS', '2': 1},
  ],
};

/// Descriptor for `QueryFlags`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List queryFlagsDescriptor = $convert.base64Decode('CgpRdWVyeUZsYWdzEhQKEFFVRVJZX0ZMQUdTX05PTkUQABIUChBRVUVSWV9GTEFHU19MT0dTEAE=');
@$core.Deprecated('Use queryTypeDescriptor instead')
const QueryType$json = const {
  '1': 'QueryType',
  '2': const [
    const {'1': 'QUERY_NONE', '2': 0},
    const {'1': 'QUERY_CAPABILITIES', '2': 1},
    const {'1': 'QUERY_CONFIGURE_SENSOR', '2': 2},
    const {'1': 'QUERY_LIVE_DATA_POLL', '2': 7},
    const {'1': 'QUERY_SCHEDULES', '2': 8},
    const {'1': 'QUERY_CONFIGURE_SCHEDULES', '2': 9},
    const {'1': 'QUERY_FILES_SD', '2': 10},
    const {'1': 'QUERY_DOWNLOAD_FILE', '2': 11},
    const {'1': 'QUERY_ERASE_FILE', '2': 12},
    const {'1': 'QUERY_RESET', '2': 13},
    const {'1': 'QUERY_NETWORK_SETTINGS', '2': 14},
    const {'1': 'QUERY_CONFIGURE_NETWORK_SETTINGS', '2': 15},
    const {'1': 'QUERY_SCAN_MODULES', '2': 16},
    const {'1': 'QUERY_CONFIGURE_IDENTITY', '2': 17},
    const {'1': 'QUERY_STATUS', '2': 18},
    const {'1': 'QUERY_MODULE', '2': 19},
    const {'1': 'QUERY_METADATA', '2': 20},
    const {'1': 'QUERY_FORMAT', '2': 21},
    const {'1': 'QUERY_GET_READINGS', '2': 22},
    const {'1': 'QUERY_TAKE_READINGS', '2': 23},
    const {'1': 'QUERY_RECORDING_CONTROL', '2': 24},
    const {'1': 'QUERY_CONFIGURE', '2': 25},
    const {'1': 'QUERY_SCAN_NETWORKS', '2': 26},
    const {'1': 'QUERY_FILES_SPI', '2': 27},
    const {'1': 'QUERY_FILES_QSPI', '2': 28},
  ],
};

/// Descriptor for `QueryType`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List queryTypeDescriptor = $convert.base64Decode('CglRdWVyeVR5cGUSDgoKUVVFUllfTk9ORRAAEhYKElFVRVJZX0NBUEFCSUxJVElFUxABEhoKFlFVRVJZX0NPTkZJR1VSRV9TRU5TT1IQAhIYChRRVUVSWV9MSVZFX0RBVEFfUE9MTBAHEhMKD1FVRVJZX1NDSEVEVUxFUxAIEh0KGVFVRVJZX0NPTkZJR1VSRV9TQ0hFRFVMRVMQCRISCg5RVUVSWV9GSUxFU19TRBAKEhcKE1FVRVJZX0RPV05MT0FEX0ZJTEUQCxIUChBRVUVSWV9FUkFTRV9GSUxFEAwSDwoLUVVFUllfUkVTRVQQDRIaChZRVUVSWV9ORVRXT1JLX1NFVFRJTkdTEA4SJAogUVVFUllfQ09ORklHVVJFX05FVFdPUktfU0VUVElOR1MQDxIWChJRVUVSWV9TQ0FOX01PRFVMRVMQEBIcChhRVUVSWV9DT05GSUdVUkVfSURFTlRJVFkQERIQCgxRVUVSWV9TVEFUVVMQEhIQCgxRVUVSWV9NT0RVTEUQExISCg5RVUVSWV9NRVRBREFUQRAUEhAKDFFVRVJZX0ZPUk1BVBAVEhYKElFVRVJZX0dFVF9SRUFESU5HUxAWEhcKE1FVRVJZX1RBS0VfUkVBRElOR1MQFxIbChdRVUVSWV9SRUNPUkRJTkdfQ09OVFJPTBAYEhMKD1FVRVJZX0NPTkZJR1VSRRAZEhcKE1FVRVJZX1NDQU5fTkVUV09SS1MQGhITCg9RVUVSWV9GSUxFU19TUEkQGxIUChBRVUVSWV9GSUxFU19RU1BJEBw=');
@$core.Deprecated('Use replyTypeDescriptor instead')
const ReplyType$json = const {
  '1': 'ReplyType',
  '2': const [
    const {'1': 'REPLY_NONE', '2': 0},
    const {'1': 'REPLY_SUCCESS', '2': 1},
    const {'1': 'REPLY_BUSY', '2': 2},
    const {'1': 'REPLY_ERROR', '2': 3},
    const {'1': 'REPLY_CAPABILITIES', '2': 4},
    const {'1': 'REPLY_LIVE_DATA_POLL', '2': 8},
    const {'1': 'REPLY_SCHEDULES', '2': 9},
    const {'1': 'REPLY_FILES', '2': 10},
    const {'1': 'REPLY_DOWNLOAD_FILE', '2': 11},
    const {'1': 'REPLY_RESET', '2': 12},
    const {'1': 'REPLY_NETWORK_SETTINGS', '2': 13},
    const {'1': 'REPLY_IDENTITY', '2': 14},
    const {'1': 'REPLY_STATUS', '2': 15},
    const {'1': 'REPLY_MODULE', '2': 16},
    const {'1': 'REPLY_METADATA', '2': 17},
    const {'1': 'REPLY_READINGS', '2': 18},
    const {'1': 'REPLY_NETWORKS', '2': 19},
  ],
};

/// Descriptor for `ReplyType`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List replyTypeDescriptor = $convert.base64Decode('CglSZXBseVR5cGUSDgoKUkVQTFlfTk9ORRAAEhEKDVJFUExZX1NVQ0NFU1MQARIOCgpSRVBMWV9CVVNZEAISDwoLUkVQTFlfRVJST1IQAxIWChJSRVBMWV9DQVBBQklMSVRJRVMQBBIYChRSRVBMWV9MSVZFX0RBVEFfUE9MTBAIEhMKD1JFUExZX1NDSEVEVUxFUxAJEg8KC1JFUExZX0ZJTEVTEAoSFwoTUkVQTFlfRE9XTkxPQURfRklMRRALEg8KC1JFUExZX1JFU0VUEAwSGgoWUkVQTFlfTkVUV09SS19TRVRUSU5HUxANEhIKDlJFUExZX0lERU5USVRZEA4SEAoMUkVQTFlfU1RBVFVTEA8SEAoMUkVQTFlfTU9EVUxFEBASEgoOUkVQTFlfTUVUQURBVEEQERISCg5SRVBMWV9SRUFESU5HUxASEhIKDlJFUExZX05FVFdPUktTEBM=');
@$core.Deprecated('Use downloadFlagsDescriptor instead')
const DownloadFlags$json = const {
  '1': 'DownloadFlags',
  '2': const [
    const {'1': 'DOWNLOAD_FLAG_NONE', '2': 0},
    const {'1': 'DOWNLOAD_FLAG_METADATA_PREPEND', '2': 1},
    const {'1': 'DOWNLOAD_FLAG_METADATA_ONLY', '2': 2},
  ],
};

/// Descriptor for `DownloadFlags`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List downloadFlagsDescriptor = $convert.base64Decode('Cg1Eb3dubG9hZEZsYWdzEhYKEkRPV05MT0FEX0ZMQUdfTk9ORRAAEiIKHkRPV05MT0FEX0ZMQUdfTUVUQURBVEFfUFJFUEVORBABEh8KG0RPV05MT0FEX0ZMQUdfTUVUQURBVEFfT05MWRAC');
@$core.Deprecated('Use moduleFlagsDescriptor instead')
const ModuleFlags$json = const {
  '1': 'ModuleFlags',
  '2': const [
    const {'1': 'MODULE_FLAG_NONE', '2': 0},
    const {'1': 'MODULE_FLAG_INTERNAL', '2': 1},
  ],
};

/// Descriptor for `ModuleFlags`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List moduleFlagsDescriptor = $convert.base64Decode('CgtNb2R1bGVGbGFncxIUChBNT0RVTEVfRkxBR19OT05FEAASGAoUTU9EVUxFX0ZMQUdfSU5URVJOQUwQAQ==');
@$core.Deprecated('Use sensorFlagsDescriptor instead')
const SensorFlags$json = const {
  '1': 'SensorFlags',
  '2': const [
    const {'1': 'SENSOR_FLAG_NONE', '2': 0},
  ],
};

/// Descriptor for `SensorFlags`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List sensorFlagsDescriptor = $convert.base64Decode('CgtTZW5zb3JGbGFncxIUChBTRU5TT1JfRkxBR19OT05FEAA=');
@$core.Deprecated('Use moduleQueryTypeDescriptor instead')
const ModuleQueryType$json = const {
  '1': 'ModuleQueryType',
  '2': const [
    const {'1': 'MODULE_QUERY_NONE', '2': 0},
    const {'1': 'MODULE_QUERY_STATUS', '2': 1},
    const {'1': 'MODULE_QUERY_CONFIGURE', '2': 2},
    const {'1': 'MODULE_QUERY_RESET', '2': 3},
  ],
};

/// Descriptor for `ModuleQueryType`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List moduleQueryTypeDescriptor = $convert.base64Decode('Cg9Nb2R1bGVRdWVyeVR5cGUSFQoRTU9EVUxFX1FVRVJZX05PTkUQABIXChNNT0RVTEVfUVVFUllfU1RBVFVTEAESGgoWTU9EVUxFX1FVRVJZX0NPTkZJR1VSRRACEhYKEk1PRFVMRV9RVUVSWV9SRVNFVBAD');
@$core.Deprecated('Use moduleReplyTypeDescriptor instead')
const ModuleReplyType$json = const {
  '1': 'ModuleReplyType',
  '2': const [
    const {'1': 'MODULE_REPLY_NONE', '2': 0},
    const {'1': 'MODULE_REPLY_SUCCESS', '2': 1},
    const {'1': 'MODULE_REPLY_BUSY', '2': 2},
    const {'1': 'MODULE_REPLY_ERROR', '2': 3},
  ],
};

/// Descriptor for `ModuleReplyType`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List moduleReplyTypeDescriptor = $convert.base64Decode('Cg9Nb2R1bGVSZXBseVR5cGUSFQoRTU9EVUxFX1JFUExZX05PTkUQABIYChRNT0RVTEVfUkVQTFlfU1VDQ0VTUxABEhUKEU1PRFVMRV9SRVBMWV9CVVNZEAISFgoSTU9EVUxFX1JFUExZX0VSUk9SEAM=');
@$core.Deprecated('Use udpStatusDescriptor instead')
const UdpStatus$json = const {
  '1': 'UdpStatus',
  '2': const [
    const {'1': 'UDP_STATUS_ONLINE', '2': 0},
    const {'1': 'UDP_STATUS_BYE', '2': 1},
  ],
};

/// Descriptor for `UdpStatus`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List udpStatusDescriptor = $convert.base64Decode('CglVZHBTdGF0dXMSFQoRVURQX1NUQVRVU19PTkxJTkUQABISCg5VRFBfU1RBVFVTX0JZRRAB');
@$core.Deprecated('Use queryCapabilitiesDescriptor instead')
const QueryCapabilities$json = const {
  '1': 'QueryCapabilities',
  '2': const [
    const {'1': 'version', '3': 1, '4': 1, '5': 13, '10': 'version'},
    const {'1': 'callerTime', '3': 2, '4': 1, '5': 13, '10': 'callerTime'},
  ],
};

/// Descriptor for `QueryCapabilities`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List queryCapabilitiesDescriptor = $convert.base64Decode('ChFRdWVyeUNhcGFiaWxpdGllcxIYCgd2ZXJzaW9uGAEgASgNUgd2ZXJzaW9uEh4KCmNhbGxlclRpbWUYAiABKA1SCmNhbGxlclRpbWU=');
@$core.Deprecated('Use liveValueDescriptor instead')
const LiveValue$json = const {
  '1': 'LiveValue',
  '2': const [
    const {'1': 'valid', '3': 1, '4': 1, '5': 8, '10': 'valid'},
    const {'1': 'value', '3': 2, '4': 1, '5': 2, '10': 'value'},
    const {'1': 'uncalibrated', '3': 3, '4': 1, '5': 2, '10': 'uncalibrated'},
  ],
};

/// Descriptor for `LiveValue`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List liveValueDescriptor = $convert.base64Decode('CglMaXZlVmFsdWUSFAoFdmFsaWQYASABKAhSBXZhbGlkEhQKBXZhbHVlGAIgASgCUgV2YWx1ZRIiCgx1bmNhbGlicmF0ZWQYAyABKAJSDHVuY2FsaWJyYXRlZA==');
@$core.Deprecated('Use sensorCapabilitiesDescriptor instead')
const SensorCapabilities$json = const {
  '1': 'SensorCapabilities',
  '2': const [
    const {'1': 'number', '3': 1, '4': 1, '5': 13, '10': 'number'},
    const {'1': 'module', '3': 2, '4': 1, '5': 13, '10': 'module'},
    const {'1': 'name', '3': 3, '4': 1, '5': 9, '10': 'name'},
    const {'1': 'frequency', '3': 4, '4': 1, '5': 13, '10': 'frequency'},
    const {'1': 'unitOfMeasure', '3': 5, '4': 1, '5': 9, '10': 'unitOfMeasure'},
    const {'1': 'uncalibratedUnitOfMeasure', '3': 9, '4': 1, '5': 9, '10': 'uncalibratedUnitOfMeasure'},
    const {'1': 'path', '3': 6, '4': 1, '5': 9, '10': 'path'},
    const {'1': 'flags', '3': 7, '4': 1, '5': 13, '10': 'flags'},
    const {'1': 'value', '3': 8, '4': 1, '5': 11, '6': '.fk_app.LiveValue', '10': 'value'},
  ],
};

/// Descriptor for `SensorCapabilities`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List sensorCapabilitiesDescriptor = $convert.base64Decode('ChJTZW5zb3JDYXBhYmlsaXRpZXMSFgoGbnVtYmVyGAEgASgNUgZudW1iZXISFgoGbW9kdWxlGAIgASgNUgZtb2R1bGUSEgoEbmFtZRgDIAEoCVIEbmFtZRIcCglmcmVxdWVuY3kYBCABKA1SCWZyZXF1ZW5jeRIkCg11bml0T2ZNZWFzdXJlGAUgASgJUg11bml0T2ZNZWFzdXJlEjwKGXVuY2FsaWJyYXRlZFVuaXRPZk1lYXN1cmUYCSABKAlSGXVuY2FsaWJyYXRlZFVuaXRPZk1lYXN1cmUSEgoEcGF0aBgGIAEoCVIEcGF0aBIUCgVmbGFncxgHIAEoDVIFZmxhZ3MSJwoFdmFsdWUYCCABKAsyES5ma19hcHAuTGl2ZVZhbHVlUgV2YWx1ZQ==');
@$core.Deprecated('Use moduleHeaderDescriptor instead')
const ModuleHeader$json = const {
  '1': 'ModuleHeader',
  '2': const [
    const {'1': 'manufacturer', '3': 1, '4': 1, '5': 13, '10': 'manufacturer'},
    const {'1': 'kind', '3': 2, '4': 1, '5': 13, '10': 'kind'},
    const {'1': 'version', '3': 3, '4': 1, '5': 13, '10': 'version'},
  ],
};

/// Descriptor for `ModuleHeader`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List moduleHeaderDescriptor = $convert.base64Decode('CgxNb2R1bGVIZWFkZXISIgoMbWFudWZhY3R1cmVyGAEgASgNUgxtYW51ZmFjdHVyZXISEgoEa2luZBgCIAEoDVIEa2luZBIYCgd2ZXJzaW9uGAMgASgNUgd2ZXJzaW9u');
@$core.Deprecated('Use moduleCapabilitiesDescriptor instead')
const ModuleCapabilities$json = const {
  '1': 'ModuleCapabilities',
  '2': const [
    const {'1': 'position', '3': 1, '4': 1, '5': 13, '10': 'position'},
    const {'1': 'name', '3': 2, '4': 1, '5': 9, '10': 'name'},
    const {'1': 'sensors', '3': 3, '4': 3, '5': 11, '6': '.fk_app.SensorCapabilities', '10': 'sensors'},
    const {'1': 'path', '3': 4, '4': 1, '5': 9, '10': 'path'},
    const {'1': 'flags', '3': 5, '4': 1, '5': 13, '10': 'flags'},
    const {'1': 'id', '3': 6, '4': 1, '5': 12, '10': 'id'},
    const {'1': 'header', '3': 7, '4': 1, '5': 11, '6': '.fk_app.ModuleHeader', '10': 'header'},
    const {'1': 'configuration', '3': 8, '4': 1, '5': 12, '10': 'configuration'},
  ],
};

/// Descriptor for `ModuleCapabilities`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List moduleCapabilitiesDescriptor = $convert.base64Decode('ChJNb2R1bGVDYXBhYmlsaXRpZXMSGgoIcG9zaXRpb24YASABKA1SCHBvc2l0aW9uEhIKBG5hbWUYAiABKAlSBG5hbWUSNAoHc2Vuc29ycxgDIAMoCzIaLmZrX2FwcC5TZW5zb3JDYXBhYmlsaXRpZXNSB3NlbnNvcnMSEgoEcGF0aBgEIAEoCVIEcGF0aBIUCgVmbGFncxgFIAEoDVIFZmxhZ3MSDgoCaWQYBiABKAxSAmlkEiwKBmhlYWRlchgHIAEoCzIULmZrX2FwcC5Nb2R1bGVIZWFkZXJSBmhlYWRlchIkCg1jb25maWd1cmF0aW9uGAggASgMUg1jb25maWd1cmF0aW9u');
@$core.Deprecated('Use capabilitiesDescriptor instead')
const Capabilities$json = const {
  '1': 'Capabilities',
  '2': const [
    const {'1': 'version', '3': 1, '4': 1, '5': 13, '10': 'version'},
    const {'1': 'deviceId', '3': 2, '4': 1, '5': 12, '10': 'deviceId'},
    const {'1': 'name', '3': 3, '4': 1, '5': 9, '10': 'name'},
    const {'1': 'modules', '3': 4, '4': 3, '5': 11, '6': '.fk_app.ModuleCapabilities', '10': 'modules'},
    const {'1': 'sensors', '3': 5, '4': 3, '5': 11, '6': '.fk_app.SensorCapabilities', '10': 'sensors'},
  ],
};

/// Descriptor for `Capabilities`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List capabilitiesDescriptor = $convert.base64Decode('CgxDYXBhYmlsaXRpZXMSGAoHdmVyc2lvbhgBIAEoDVIHdmVyc2lvbhIaCghkZXZpY2VJZBgCIAEoDFIIZGV2aWNlSWQSEgoEbmFtZRgDIAEoCVIEbmFtZRI0Cgdtb2R1bGVzGAQgAygLMhouZmtfYXBwLk1vZHVsZUNhcGFiaWxpdGllc1IHbW9kdWxlcxI0CgdzZW5zb3JzGAUgAygLMhouZmtfYXBwLlNlbnNvckNhcGFiaWxpdGllc1IHc2Vuc29ycw==');
@$core.Deprecated('Use networkInfoDescriptor instead')
const NetworkInfo$json = const {
  '1': 'NetworkInfo',
  '2': const [
    const {'1': 'ssid', '3': 1, '4': 1, '5': 9, '10': 'ssid'},
    const {'1': 'password', '3': 2, '4': 1, '5': 9, '10': 'password'},
    const {'1': 'create', '3': 3, '4': 1, '5': 8, '10': 'create'},
    const {'1': 'preferred', '3': 4, '4': 1, '5': 8, '10': 'preferred'},
    const {'1': 'keeping', '3': 5, '4': 1, '5': 8, '10': 'keeping'},
  ],
};

/// Descriptor for `NetworkInfo`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List networkInfoDescriptor = $convert.base64Decode('CgtOZXR3b3JrSW5mbxISCgRzc2lkGAEgASgJUgRzc2lkEhoKCHBhc3N3b3JkGAIgASgJUghwYXNzd29yZBIWCgZjcmVhdGUYAyABKAhSBmNyZWF0ZRIcCglwcmVmZXJyZWQYBCABKAhSCXByZWZlcnJlZBIYCgdrZWVwaW5nGAUgASgIUgdrZWVwaW5n');
@$core.Deprecated('Use networkSettingsDescriptor instead')
const NetworkSettings$json = const {
  '1': 'NetworkSettings',
  '2': const [
    const {'1': 'createAccessPoint', '3': 1, '4': 1, '5': 5, '10': 'createAccessPoint'},
    const {'1': 'connected', '3': 3, '4': 1, '5': 11, '6': '.fk_app.NetworkInfo', '10': 'connected'},
    const {'1': 'macAddress', '3': 4, '4': 1, '5': 9, '10': 'macAddress'},
    const {'1': 'modifying', '3': 5, '4': 1, '5': 8, '10': 'modifying'},
    const {'1': 'networks', '3': 2, '4': 3, '5': 11, '6': '.fk_app.NetworkInfo', '10': 'networks'},
  ],
};

/// Descriptor for `NetworkSettings`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List networkSettingsDescriptor = $convert.base64Decode('Cg9OZXR3b3JrU2V0dGluZ3MSLAoRY3JlYXRlQWNjZXNzUG9pbnQYASABKAVSEWNyZWF0ZUFjY2Vzc1BvaW50EjEKCWNvbm5lY3RlZBgDIAEoCzITLmZrX2FwcC5OZXR3b3JrSW5mb1IJY29ubmVjdGVkEh4KCm1hY0FkZHJlc3MYBCABKAlSCm1hY0FkZHJlc3MSHAoJbW9kaWZ5aW5nGAUgASgIUgltb2RpZnlpbmcSLwoIbmV0d29ya3MYAiADKAsyEy5ma19hcHAuTmV0d29ya0luZm9SCG5ldHdvcmtz');
@$core.Deprecated('Use firmwareDescriptor instead')
const Firmware$json = const {
  '1': 'Firmware',
  '2': const [
    const {'1': 'version', '3': 1, '4': 1, '5': 9, '10': 'version'},
    const {'1': 'build', '3': 2, '4': 1, '5': 9, '10': 'build'},
    const {'1': 'number', '3': 3, '4': 1, '5': 9, '10': 'number'},
    const {'1': 'timestamp', '3': 4, '4': 1, '5': 4, '10': 'timestamp'},
    const {'1': 'hash', '3': 5, '4': 1, '5': 9, '10': 'hash'},
    const {'1': 'logical_address', '3': 6, '4': 1, '5': 4, '10': 'logicalAddress'},
    const {'1': 'name', '3': 7, '4': 1, '5': 9, '10': 'name'},
  ],
};

/// Descriptor for `Firmware`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List firmwareDescriptor = $convert.base64Decode('CghGaXJtd2FyZRIYCgd2ZXJzaW9uGAEgASgJUgd2ZXJzaW9uEhQKBWJ1aWxkGAIgASgJUgVidWlsZBIWCgZudW1iZXIYAyABKAlSBm51bWJlchIcCgl0aW1lc3RhbXAYBCABKARSCXRpbWVzdGFtcBISCgRoYXNoGAUgASgJUgRoYXNoEicKD2xvZ2ljYWxfYWRkcmVzcxgGIAEoBFIObG9naWNhbEFkZHJlc3MSEgoEbmFtZRgHIAEoCVIEbmFtZQ==');
@$core.Deprecated('Use identityDescriptor instead')
const Identity$json = const {
  '1': 'Identity',
  '2': const [
    const {'1': 'device', '3': 1, '4': 1, '5': 9, '10': 'device'},
    const {'1': 'stream', '3': 2, '4': 1, '5': 9, '10': 'stream'},
    const {'1': 'deviceId', '3': 3, '4': 1, '5': 12, '10': 'deviceId'},
    const {'1': 'firmware', '3': 4, '4': 1, '5': 9, '10': 'firmware'},
    const {'1': 'build', '3': 5, '4': 1, '5': 9, '10': 'build'},
    const {'1': 'number', '3': 8, '4': 1, '5': 9, '10': 'number'},
    const {'1': 'name', '3': 6, '4': 1, '5': 9, '10': 'name'},
    const {'1': 'generationId', '3': 7, '4': 1, '5': 12, '10': 'generationId'},
  ],
};

/// Descriptor for `Identity`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List identityDescriptor = $convert.base64Decode('CghJZGVudGl0eRIWCgZkZXZpY2UYASABKAlSBmRldmljZRIWCgZzdHJlYW0YAiABKAlSBnN0cmVhbRIaCghkZXZpY2VJZBgDIAEoDFIIZGV2aWNlSWQSGgoIZmlybXdhcmUYBCABKAlSCGZpcm13YXJlEhQKBWJ1aWxkGAUgASgJUgVidWlsZBIWCgZudW1iZXIYCCABKAlSBm51bWJlchISCgRuYW1lGAYgASgJUgRuYW1lEiIKDGdlbmVyYXRpb25JZBgHIAEoDFIMZ2VuZXJhdGlvbklk');
@$core.Deprecated('Use configureSensorQueryDescriptor instead')
const ConfigureSensorQuery$json = const {
  '1': 'ConfigureSensorQuery',
  '2': const [
    const {'1': 'id', '3': 1, '4': 1, '5': 13, '10': 'id'},
    const {'1': 'frequency', '3': 2, '4': 1, '5': 13, '10': 'frequency'},
  ],
};

/// Descriptor for `ConfigureSensorQuery`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List configureSensorQueryDescriptor = $convert.base64Decode('ChRDb25maWd1cmVTZW5zb3JRdWVyeRIOCgJpZBgBIAEoDVICaWQSHAoJZnJlcXVlbmN5GAIgASgNUglmcmVxdWVuY3k=');
@$core.Deprecated('Use liveDataPollDescriptor instead')
const LiveDataPoll$json = const {
  '1': 'LiveDataPoll',
  '2': const [
    const {'1': 'interval', '3': 1, '4': 1, '5': 13, '10': 'interval'},
  ],
};

/// Descriptor for `LiveDataPoll`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List liveDataPollDescriptor = $convert.base64Decode('CgxMaXZlRGF0YVBvbGwSGgoIaW50ZXJ2YWwYASABKA1SCGludGVydmFs');
@$core.Deprecated('Use liveDataSampleDescriptor instead')
const LiveDataSample$json = const {
  '1': 'LiveDataSample',
  '2': const [
    const {'1': 'sensor', '3': 1, '4': 1, '5': 13, '10': 'sensor'},
    const {'1': 'time', '3': 2, '4': 1, '5': 4, '10': 'time'},
    const {'1': 'value', '3': 3, '4': 1, '5': 2, '10': 'value'},
  ],
};

/// Descriptor for `LiveDataSample`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List liveDataSampleDescriptor = $convert.base64Decode('Cg5MaXZlRGF0YVNhbXBsZRIWCgZzZW5zb3IYASABKA1SBnNlbnNvchISCgR0aW1lGAIgASgEUgR0aW1lEhQKBXZhbHVlGAMgASgCUgV2YWx1ZQ==');
@$core.Deprecated('Use liveDataDescriptor instead')
const LiveData$json = const {
  '1': 'LiveData',
  '2': const [
    const {'1': 'samples', '3': 1, '4': 3, '5': 11, '6': '.fk_app.LiveDataSample', '10': 'samples'},
  ],
};

/// Descriptor for `LiveData`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List liveDataDescriptor = $convert.base64Decode('CghMaXZlRGF0YRIwCgdzYW1wbGVzGAEgAygLMhYuZmtfYXBwLkxpdmVEYXRhU2FtcGxlUgdzYW1wbGVz');
@$core.Deprecated('Use fileDescriptor instead')
const File$json = const {
  '1': 'File',
  '2': const [
    const {'1': 'id', '3': 1, '4': 1, '5': 13, '10': 'id'},
    const {'1': 'time', '3': 2, '4': 1, '5': 4, '10': 'time'},
    const {'1': 'size', '3': 3, '4': 1, '5': 4, '10': 'size'},
    const {'1': 'version', '3': 4, '4': 1, '5': 13, '10': 'version'},
    const {'1': 'name', '3': 5, '4': 1, '5': 9, '10': 'name'},
    const {'1': 'maximum', '3': 6, '4': 1, '5': 4, '10': 'maximum'},
  ],
};

/// Descriptor for `File`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List fileDescriptor = $convert.base64Decode('CgRGaWxlEg4KAmlkGAEgASgNUgJpZBISCgR0aW1lGAIgASgEUgR0aW1lEhIKBHNpemUYAyABKARSBHNpemUSGAoHdmVyc2lvbhgEIAEoDVIHdmVyc2lvbhISCgRuYW1lGAUgASgJUgRuYW1lEhgKB21heGltdW0YBiABKARSB21heGltdW0=');
@$core.Deprecated('Use filesDescriptor instead')
const Files$json = const {
  '1': 'Files',
  '2': const [
    const {'1': 'files', '3': 1, '4': 3, '5': 11, '6': '.fk_app.File', '10': 'files'},
  ],
};

/// Descriptor for `Files`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List filesDescriptor = $convert.base64Decode('CgVGaWxlcxIiCgVmaWxlcxgBIAMoCzIMLmZrX2FwcC5GaWxlUgVmaWxlcw==');
@$core.Deprecated('Use downloadFileDescriptor instead')
const DownloadFile$json = const {
  '1': 'DownloadFile',
  '2': const [
    const {'1': 'id', '3': 1, '4': 1, '5': 13, '10': 'id'},
    const {'1': 'offset', '3': 2, '4': 1, '5': 13, '10': 'offset'},
    const {'1': 'length', '3': 3, '4': 1, '5': 13, '10': 'length'},
    const {'1': 'flags', '3': 4, '4': 1, '5': 13, '10': 'flags'},
  ],
};

/// Descriptor for `DownloadFile`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List downloadFileDescriptor = $convert.base64Decode('CgxEb3dubG9hZEZpbGUSDgoCaWQYASABKA1SAmlkEhYKBm9mZnNldBgCIAEoDVIGb2Zmc2V0EhYKBmxlbmd0aBgDIAEoDVIGbGVuZ3RoEhQKBWZsYWdzGAQgASgNUgVmbGFncw==');
@$core.Deprecated('Use eraseFileDescriptor instead')
const EraseFile$json = const {
  '1': 'EraseFile',
  '2': const [
    const {'1': 'id', '3': 1, '4': 1, '5': 13, '10': 'id'},
  ],
};

/// Descriptor for `EraseFile`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List eraseFileDescriptor = $convert.base64Decode('CglFcmFzZUZpbGUSDgoCaWQYASABKA1SAmlk');
@$core.Deprecated('Use fileDataDescriptor instead')
const FileData$json = const {
  '1': 'FileData',
  '2': const [
    const {'1': 'offset', '3': 1, '4': 1, '5': 13, '10': 'offset'},
    const {'1': 'data', '3': 2, '4': 1, '5': 12, '10': 'data'},
    const {'1': 'size', '3': 3, '4': 1, '5': 13, '10': 'size'},
    const {'1': 'hash', '3': 4, '4': 1, '5': 13, '10': 'hash'},
    const {'1': 'version', '3': 5, '4': 1, '5': 13, '10': 'version'},
    const {'1': 'id', '3': 6, '4': 1, '5': 13, '10': 'id'},
  ],
};

/// Descriptor for `FileData`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List fileDataDescriptor = $convert.base64Decode('CghGaWxlRGF0YRIWCgZvZmZzZXQYASABKA1SBm9mZnNldBISCgRkYXRhGAIgASgMUgRkYXRhEhIKBHNpemUYAyABKA1SBHNpemUSEgoEaGFzaBgEIAEoDVIEaGFzaBIYCgd2ZXJzaW9uGAUgASgNUgd2ZXJzaW9uEg4KAmlkGAYgASgNUgJpZA==');
@$core.Deprecated('Use deviceStatusDescriptor instead')
const DeviceStatus$json = const {
  '1': 'DeviceStatus',
  '2': const [
    const {'1': 'uptime', '3': 1, '4': 1, '5': 13, '10': 'uptime'},
    const {'1': 'batteryPercentage', '3': 2, '4': 1, '5': 2, '10': 'batteryPercentage'},
    const {'1': 'batteryVoltage', '3': 3, '4': 1, '5': 2, '10': 'batteryVoltage'},
    const {'1': 'gpsHasFix', '3': 4, '4': 1, '5': 13, '10': 'gpsHasFix'},
    const {'1': 'gpsSatellites', '3': 5, '4': 1, '5': 13, '10': 'gpsSatellites'},
  ],
};

/// Descriptor for `DeviceStatus`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List deviceStatusDescriptor = $convert.base64Decode('CgxEZXZpY2VTdGF0dXMSFgoGdXB0aW1lGAEgASgNUgZ1cHRpbWUSLAoRYmF0dGVyeVBlcmNlbnRhZ2UYAiABKAJSEWJhdHRlcnlQZXJjZW50YWdlEiYKDmJhdHRlcnlWb2x0YWdlGAMgASgCUg5iYXR0ZXJ5Vm9sdGFnZRIcCglncHNIYXNGaXgYBCABKA1SCWdwc0hhc0ZpeBIkCg1ncHNTYXRlbGxpdGVzGAUgASgNUg1ncHNTYXRlbGxpdGVz');
@$core.Deprecated('Use queryModuleDescriptor instead')
const QueryModule$json = const {
  '1': 'QueryModule',
  '2': const [
    const {'1': 'id', '3': 1, '4': 1, '5': 13, '10': 'id'},
    const {'1': 'address', '3': 2, '4': 1, '5': 13, '10': 'address'},
    const {'1': 'message', '3': 3, '4': 1, '5': 12, '10': 'message'},
  ],
};

/// Descriptor for `QueryModule`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List queryModuleDescriptor = $convert.base64Decode('CgtRdWVyeU1vZHVsZRIOCgJpZBgBIAEoDVICaWQSGAoHYWRkcmVzcxgCIAEoDVIHYWRkcmVzcxIYCgdtZXNzYWdlGAMgASgMUgdtZXNzYWdl');
@$core.Deprecated('Use moduleReplyDescriptor instead')
const ModuleReply$json = const {
  '1': 'ModuleReply',
  '2': const [
    const {'1': 'id', '3': 1, '4': 1, '5': 13, '10': 'id'},
    const {'1': 'address', '3': 2, '4': 1, '5': 13, '10': 'address'},
    const {'1': 'message', '3': 3, '4': 1, '5': 12, '10': 'message'},
  ],
};

/// Descriptor for `ModuleReply`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List moduleReplyDescriptor = $convert.base64Decode('CgtNb2R1bGVSZXBseRIOCgJpZBgBIAEoDVICaWQSGAoHYWRkcmVzcxgCIAEoDVIHYWRkcmVzcxIYCgdtZXNzYWdlGAMgASgMUgdtZXNzYWdl');
@$core.Deprecated('Use wireMessageQueryDescriptor instead')
const WireMessageQuery$json = const {
  '1': 'WireMessageQuery',
  '2': const [
    const {'1': 'type', '3': 1, '4': 1, '5': 14, '6': '.fk_app.QueryType', '10': 'type'},
    const {'1': 'queryCapabilities', '3': 2, '4': 1, '5': 11, '6': '.fk_app.QueryCapabilities', '10': 'queryCapabilities'},
    const {'1': 'configureSensor', '3': 3, '4': 1, '5': 11, '6': '.fk_app.ConfigureSensorQuery', '10': 'configureSensor'},
    const {'1': 'liveDataPoll', '3': 8, '4': 1, '5': 11, '6': '.fk_app.LiveDataPoll', '10': 'liveDataPoll'},
    const {'1': 'downloadFile', '3': 10, '4': 1, '5': 11, '6': '.fk_app.DownloadFile', '10': 'downloadFile'},
    const {'1': 'eraseFile', '3': 11, '4': 1, '5': 11, '6': '.fk_app.EraseFile', '10': 'eraseFile'},
    const {'1': 'networkSettings', '3': 12, '4': 1, '5': 11, '6': '.fk_app.NetworkSettings', '10': 'networkSettings'},
    const {'1': 'identity', '3': 13, '4': 1, '5': 11, '6': '.fk_app.Identity', '10': 'identity'},
    const {'1': 'module', '3': 14, '4': 1, '5': 11, '6': '.fk_app.QueryModule', '10': 'module'},
  ],
};

/// Descriptor for `WireMessageQuery`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List wireMessageQueryDescriptor = $convert.base64Decode('ChBXaXJlTWVzc2FnZVF1ZXJ5EiUKBHR5cGUYASABKA4yES5ma19hcHAuUXVlcnlUeXBlUgR0eXBlEkcKEXF1ZXJ5Q2FwYWJpbGl0aWVzGAIgASgLMhkuZmtfYXBwLlF1ZXJ5Q2FwYWJpbGl0aWVzUhFxdWVyeUNhcGFiaWxpdGllcxJGCg9jb25maWd1cmVTZW5zb3IYAyABKAsyHC5ma19hcHAuQ29uZmlndXJlU2Vuc29yUXVlcnlSD2NvbmZpZ3VyZVNlbnNvchI4CgxsaXZlRGF0YVBvbGwYCCABKAsyFC5ma19hcHAuTGl2ZURhdGFQb2xsUgxsaXZlRGF0YVBvbGwSOAoMZG93bmxvYWRGaWxlGAogASgLMhQuZmtfYXBwLkRvd25sb2FkRmlsZVIMZG93bmxvYWRGaWxlEi8KCWVyYXNlRmlsZRgLIAEoCzIRLmZrX2FwcC5FcmFzZUZpbGVSCWVyYXNlRmlsZRJBCg9uZXR3b3JrU2V0dGluZ3MYDCABKAsyFy5ma19hcHAuTmV0d29ya1NldHRpbmdzUg9uZXR3b3JrU2V0dGluZ3MSLAoIaWRlbnRpdHkYDSABKAsyEC5ma19hcHAuSWRlbnRpdHlSCGlkZW50aXR5EisKBm1vZHVsZRgOIAEoCzITLmZrX2FwcC5RdWVyeU1vZHVsZVIGbW9kdWxl');
@$core.Deprecated('Use errorDescriptor instead')
const Error$json = const {
  '1': 'Error',
  '2': const [
    const {'1': 'message', '3': 1, '4': 1, '5': 9, '10': 'message'},
    const {'1': 'delay', '3': 2, '4': 1, '5': 13, '10': 'delay'},
  ],
};

/// Descriptor for `Error`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List errorDescriptor = $convert.base64Decode('CgVFcnJvchIYCgdtZXNzYWdlGAEgASgJUgdtZXNzYWdlEhQKBWRlbGF5GAIgASgNUgVkZWxheQ==');
@$core.Deprecated('Use wireMessageReplyDescriptor instead')
const WireMessageReply$json = const {
  '1': 'WireMessageReply',
  '2': const [
    const {'1': 'type', '3': 1, '4': 1, '5': 14, '6': '.fk_app.ReplyType', '10': 'type'},
    const {'1': 'errors', '3': 2, '4': 3, '5': 11, '6': '.fk_app.Error', '10': 'errors'},
    const {'1': 'capabilities', '3': 3, '4': 1, '5': 11, '6': '.fk_app.Capabilities', '10': 'capabilities'},
    const {'1': 'liveData', '3': 6, '4': 1, '5': 11, '6': '.fk_app.LiveData', '10': 'liveData'},
    const {'1': 'files', '3': 8, '4': 1, '5': 11, '6': '.fk_app.Files', '10': 'files'},
    const {'1': 'fileData', '3': 9, '4': 1, '5': 11, '6': '.fk_app.FileData', '10': 'fileData'},
    const {'1': 'networkSettings', '3': 10, '4': 1, '5': 11, '6': '.fk_app.NetworkSettings', '10': 'networkSettings'},
    const {'1': 'identity', '3': 11, '4': 1, '5': 11, '6': '.fk_app.Identity', '10': 'identity'},
    const {'1': 'status', '3': 12, '4': 1, '5': 11, '6': '.fk_app.DeviceStatus', '10': 'status'},
    const {'1': 'module', '3': 13, '4': 1, '5': 11, '6': '.fk_app.ModuleReply', '10': 'module'},
  ],
};

/// Descriptor for `WireMessageReply`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List wireMessageReplyDescriptor = $convert.base64Decode('ChBXaXJlTWVzc2FnZVJlcGx5EiUKBHR5cGUYASABKA4yES5ma19hcHAuUmVwbHlUeXBlUgR0eXBlEiUKBmVycm9ycxgCIAMoCzINLmZrX2FwcC5FcnJvclIGZXJyb3JzEjgKDGNhcGFiaWxpdGllcxgDIAEoCzIULmZrX2FwcC5DYXBhYmlsaXRpZXNSDGNhcGFiaWxpdGllcxIsCghsaXZlRGF0YRgGIAEoCzIQLmZrX2FwcC5MaXZlRGF0YVIIbGl2ZURhdGESIwoFZmlsZXMYCCABKAsyDS5ma19hcHAuRmlsZXNSBWZpbGVzEiwKCGZpbGVEYXRhGAkgASgLMhAuZmtfYXBwLkZpbGVEYXRhUghmaWxlRGF0YRJBCg9uZXR3b3JrU2V0dGluZ3MYCiABKAsyFy5ma19hcHAuTmV0d29ya1NldHRpbmdzUg9uZXR3b3JrU2V0dGluZ3MSLAoIaWRlbnRpdHkYCyABKAsyEC5ma19hcHAuSWRlbnRpdHlSCGlkZW50aXR5EiwKBnN0YXR1cxgMIAEoCzIULmZrX2FwcC5EZXZpY2VTdGF0dXNSBnN0YXR1cxIrCgZtb2R1bGUYDSABKAsyEy5ma19hcHAuTW9kdWxlUmVwbHlSBm1vZHVsZQ==');
@$core.Deprecated('Use intervalDescriptor instead')
const Interval$json = const {
  '1': 'Interval',
  '2': const [
    const {'1': 'start', '3': 1, '4': 1, '5': 4, '10': 'start'},
    const {'1': 'end', '3': 2, '4': 1, '5': 4, '10': 'end'},
    const {'1': 'interval', '3': 3, '4': 1, '5': 13, '10': 'interval'},
  ],
};

/// Descriptor for `Interval`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List intervalDescriptor = $convert.base64Decode('CghJbnRlcnZhbBIUCgVzdGFydBgBIAEoBFIFc3RhcnQSEAoDZW5kGAIgASgEUgNlbmQSGgoIaW50ZXJ2YWwYAyABKA1SCGludGVydmFs');
@$core.Deprecated('Use scheduleDescriptor instead')
const Schedule$json = const {
  '1': 'Schedule',
  '2': const [
    const {'1': 'cron', '3': 1, '4': 1, '5': 12, '10': 'cron'},
    const {'1': 'interval', '3': 2, '4': 1, '5': 13, '10': 'interval'},
    const {'1': 'repeated', '3': 3, '4': 1, '5': 13, '10': 'repeated'},
    const {'1': 'duration', '3': 4, '4': 1, '5': 13, '10': 'duration'},
    const {'1': 'jitter', '3': 5, '4': 1, '5': 13, '10': 'jitter'},
    const {'1': 'intervals', '3': 6, '4': 3, '5': 11, '6': '.fk_app.Interval', '10': 'intervals'},
  ],
};

/// Descriptor for `Schedule`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List scheduleDescriptor = $convert.base64Decode('CghTY2hlZHVsZRISCgRjcm9uGAEgASgMUgRjcm9uEhoKCGludGVydmFsGAIgASgNUghpbnRlcnZhbBIaCghyZXBlYXRlZBgDIAEoDVIIcmVwZWF0ZWQSGgoIZHVyYXRpb24YBCABKA1SCGR1cmF0aW9uEhYKBmppdHRlchgFIAEoDVIGaml0dGVyEi4KCWludGVydmFscxgGIAMoCzIQLmZrX2FwcC5JbnRlcnZhbFIJaW50ZXJ2YWxz');
@$core.Deprecated('Use schedulesDescriptor instead')
const Schedules$json = const {
  '1': 'Schedules',
  '2': const [
    const {'1': 'modifying', '3': 1, '4': 1, '5': 8, '10': 'modifying'},
    const {'1': 'readings', '3': 2, '4': 1, '5': 11, '6': '.fk_app.Schedule', '10': 'readings'},
    const {'1': 'lora', '3': 3, '4': 1, '5': 11, '6': '.fk_app.Schedule', '10': 'lora'},
    const {'1': 'network', '3': 4, '4': 1, '5': 11, '6': '.fk_app.Schedule', '10': 'network'},
    const {'1': 'gps', '3': 5, '4': 1, '5': 11, '6': '.fk_app.Schedule', '10': 'gps'},
  ],
};

/// Descriptor for `Schedules`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List schedulesDescriptor = $convert.base64Decode('CglTY2hlZHVsZXMSHAoJbW9kaWZ5aW5nGAEgASgIUgltb2RpZnlpbmcSLAoIcmVhZGluZ3MYAiABKAsyEC5ma19hcHAuU2NoZWR1bGVSCHJlYWRpbmdzEiQKBGxvcmEYAyABKAsyEC5ma19hcHAuU2NoZWR1bGVSBGxvcmESKgoHbmV0d29yaxgEIAEoCzIQLmZrX2FwcC5TY2hlZHVsZVIHbmV0d29yaxIiCgNncHMYBSABKAsyEC5ma19hcHAuU2NoZWR1bGVSA2dwcw==');
@$core.Deprecated('Use hardwareStatusDescriptor instead')
const HardwareStatus$json = const {
  '1': 'HardwareStatus',
};

/// Descriptor for `HardwareStatus`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List hardwareStatusDescriptor = $convert.base64Decode('Cg5IYXJkd2FyZVN0YXR1cw==');
@$core.Deprecated('Use gpsStatusDescriptor instead')
const GpsStatus$json = const {
  '1': 'GpsStatus',
  '2': const [
    const {'1': 'enabled', '3': 7, '4': 1, '5': 13, '10': 'enabled'},
    const {'1': 'fix', '3': 1, '4': 1, '5': 13, '10': 'fix'},
    const {'1': 'time', '3': 2, '4': 1, '5': 4, '10': 'time'},
    const {'1': 'satellites', '3': 3, '4': 1, '5': 13, '10': 'satellites'},
    const {'1': 'longitude', '3': 4, '4': 1, '5': 2, '10': 'longitude'},
    const {'1': 'latitude', '3': 5, '4': 1, '5': 2, '10': 'latitude'},
    const {'1': 'altitude', '3': 6, '4': 1, '5': 2, '10': 'altitude'},
  ],
};

/// Descriptor for `GpsStatus`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List gpsStatusDescriptor = $convert.base64Decode('CglHcHNTdGF0dXMSGAoHZW5hYmxlZBgHIAEoDVIHZW5hYmxlZBIQCgNmaXgYASABKA1SA2ZpeBISCgR0aW1lGAIgASgEUgR0aW1lEh4KCnNhdGVsbGl0ZXMYAyABKA1SCnNhdGVsbGl0ZXMSHAoJbG9uZ2l0dWRlGAQgASgCUglsb25naXR1ZGUSGgoIbGF0aXR1ZGUYBSABKAJSCGxhdGl0dWRlEhoKCGFsdGl0dWRlGAYgASgCUghhbHRpdHVkZQ==');
@$core.Deprecated('Use memoryStatusDescriptor instead')
const MemoryStatus$json = const {
  '1': 'MemoryStatus',
  '2': const [
    const {'1': 'sramAvailable', '3': 1, '4': 1, '5': 13, '10': 'sramAvailable'},
    const {'1': 'programFlashAvailable', '3': 2, '4': 1, '5': 13, '10': 'programFlashAvailable'},
    const {'1': 'extendedMemoryAvailable', '3': 3, '4': 1, '5': 13, '10': 'extendedMemoryAvailable'},
    const {'1': 'dataMemoryInstalled', '3': 4, '4': 1, '5': 13, '10': 'dataMemoryInstalled'},
    const {'1': 'dataMemoryUsed', '3': 5, '4': 1, '5': 13, '10': 'dataMemoryUsed'},
    const {'1': 'dataMemoryConsumption', '3': 6, '4': 1, '5': 2, '10': 'dataMemoryConsumption'},
    const {'1': 'firmware', '3': 7, '4': 3, '5': 11, '6': '.fk_app.Firmware', '10': 'firmware'},
  ],
};

/// Descriptor for `MemoryStatus`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List memoryStatusDescriptor = $convert.base64Decode('CgxNZW1vcnlTdGF0dXMSJAoNc3JhbUF2YWlsYWJsZRgBIAEoDVINc3JhbUF2YWlsYWJsZRI0ChVwcm9ncmFtRmxhc2hBdmFpbGFibGUYAiABKA1SFXByb2dyYW1GbGFzaEF2YWlsYWJsZRI4ChdleHRlbmRlZE1lbW9yeUF2YWlsYWJsZRgDIAEoDVIXZXh0ZW5kZWRNZW1vcnlBdmFpbGFibGUSMAoTZGF0YU1lbW9yeUluc3RhbGxlZBgEIAEoDVITZGF0YU1lbW9yeUluc3RhbGxlZBImCg5kYXRhTWVtb3J5VXNlZBgFIAEoDVIOZGF0YU1lbW9yeVVzZWQSNAoVZGF0YU1lbW9yeUNvbnN1bXB0aW9uGAYgASgCUhVkYXRhTWVtb3J5Q29uc3VtcHRpb24SLAoIZmlybXdhcmUYByADKAsyEC5ma19hcHAuRmlybXdhcmVSCGZpcm13YXJl');
@$core.Deprecated('Use batteryStatusDescriptor instead')
const BatteryStatus$json = const {
  '1': 'BatteryStatus',
  '2': const [
    const {'1': 'voltage', '3': 1, '4': 1, '5': 13, '10': 'voltage'},
    const {'1': 'percentage', '3': 2, '4': 1, '5': 13, '10': 'percentage'},
  ],
};

/// Descriptor for `BatteryStatus`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List batteryStatusDescriptor = $convert.base64Decode('Cg1CYXR0ZXJ5U3RhdHVzEhgKB3ZvbHRhZ2UYASABKA1SB3ZvbHRhZ2USHgoKcGVyY2VudGFnZRgCIAEoDVIKcGVyY2VudGFnZQ==');
@$core.Deprecated('Use solarStatusDescriptor instead')
const SolarStatus$json = const {
  '1': 'SolarStatus',
  '2': const [
    const {'1': 'voltage', '3': 1, '4': 1, '5': 13, '10': 'voltage'},
  ],
};

/// Descriptor for `SolarStatus`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List solarStatusDescriptor = $convert.base64Decode('CgtTb2xhclN0YXR1cxIYCgd2b2x0YWdlGAEgASgNUgd2b2x0YWdl');
@$core.Deprecated('Use powerStatusDescriptor instead')
const PowerStatus$json = const {
  '1': 'PowerStatus',
  '2': const [
    const {'1': 'battery', '3': 1, '4': 1, '5': 11, '6': '.fk_app.BatteryStatus', '10': 'battery'},
    const {'1': 'solar', '3': 2, '4': 1, '5': 11, '6': '.fk_app.SolarStatus', '10': 'solar'},
  ],
};

/// Descriptor for `PowerStatus`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List powerStatusDescriptor = $convert.base64Decode('CgtQb3dlclN0YXR1cxIvCgdiYXR0ZXJ5GAEgASgLMhUuZmtfYXBwLkJhdHRlcnlTdGF0dXNSB2JhdHRlcnkSKQoFc29sYXIYAiABKAsyEy5ma19hcHAuU29sYXJTdGF0dXNSBXNvbGFy');
@$core.Deprecated('Use statusDescriptor instead')
const Status$json = const {
  '1': 'Status',
  '2': const [
    const {'1': 'version', '3': 1, '4': 1, '5': 13, '10': 'version'},
    const {'1': 'uptime', '3': 2, '4': 1, '5': 13, '10': 'uptime'},
    const {'1': 'identity', '3': 3, '4': 1, '5': 11, '6': '.fk_app.Identity', '10': 'identity'},
    const {'1': 'hardware', '3': 4, '4': 1, '5': 11, '6': '.fk_app.HardwareStatus', '10': 'hardware'},
    const {'1': 'power', '3': 5, '4': 1, '5': 11, '6': '.fk_app.PowerStatus', '10': 'power'},
    const {'1': 'memory', '3': 6, '4': 1, '5': 11, '6': '.fk_app.MemoryStatus', '10': 'memory'},
    const {'1': 'gps', '3': 7, '4': 1, '5': 11, '6': '.fk_app.GpsStatus', '10': 'gps'},
    const {'1': 'schedules', '3': 8, '4': 1, '5': 11, '6': '.fk_app.Schedules', '10': 'schedules'},
    const {'1': 'recording', '3': 9, '4': 1, '5': 11, '6': '.fk_app.Recording', '10': 'recording'},
    const {'1': 'network', '3': 10, '4': 1, '5': 11, '6': '.fk_app.NetworkSettings', '10': 'network'},
    const {'1': 'time', '3': 11, '4': 1, '5': 4, '10': 'time'},
    const {'1': 'firmware', '3': 12, '4': 1, '5': 11, '6': '.fk_app.Firmware', '10': 'firmware'},
    const {'1': 'logs', '3': 13, '4': 1, '5': 9, '10': 'logs'},
  ],
};

/// Descriptor for `Status`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List statusDescriptor = $convert.base64Decode('CgZTdGF0dXMSGAoHdmVyc2lvbhgBIAEoDVIHdmVyc2lvbhIWCgZ1cHRpbWUYAiABKA1SBnVwdGltZRIsCghpZGVudGl0eRgDIAEoCzIQLmZrX2FwcC5JZGVudGl0eVIIaWRlbnRpdHkSMgoIaGFyZHdhcmUYBCABKAsyFi5ma19hcHAuSGFyZHdhcmVTdGF0dXNSCGhhcmR3YXJlEikKBXBvd2VyGAUgASgLMhMuZmtfYXBwLlBvd2VyU3RhdHVzUgVwb3dlchIsCgZtZW1vcnkYBiABKAsyFC5ma19hcHAuTWVtb3J5U3RhdHVzUgZtZW1vcnkSIwoDZ3BzGAcgASgLMhEuZmtfYXBwLkdwc1N0YXR1c1IDZ3BzEi8KCXNjaGVkdWxlcxgIIAEoCzIRLmZrX2FwcC5TY2hlZHVsZXNSCXNjaGVkdWxlcxIvCglyZWNvcmRpbmcYCSABKAsyES5ma19hcHAuUmVjb3JkaW5nUglyZWNvcmRpbmcSMQoHbmV0d29yaxgKIAEoCzIXLmZrX2FwcC5OZXR3b3JrU2V0dGluZ3NSB25ldHdvcmsSEgoEdGltZRgLIAEoBFIEdGltZRIsCghmaXJtd2FyZRgMIAEoCzIQLmZrX2FwcC5GaXJtd2FyZVIIZmlybXdhcmUSEgoEbG9ncxgNIAEoCVIEbG9ncw==');
@$core.Deprecated('Use rangeDescriptor instead')
const Range$json = const {
  '1': 'Range',
  '2': const [
    const {'1': 'start', '3': 1, '4': 1, '5': 13, '10': 'start'},
    const {'1': 'end', '3': 2, '4': 1, '5': 13, '10': 'end'},
  ],
};

/// Descriptor for `Range`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List rangeDescriptor = $convert.base64Decode('CgVSYW5nZRIUCgVzdGFydBgBIAEoDVIFc3RhcnQSEAoDZW5kGAIgASgNUgNlbmQ=');
@$core.Deprecated('Use downloadQueryDescriptor instead')
const DownloadQuery$json = const {
  '1': 'DownloadQuery',
  '2': const [
    const {'1': 'stream', '3': 1, '4': 1, '5': 13, '10': 'stream'},
    const {'1': 'ranges', '3': 3, '4': 3, '5': 11, '6': '.fk_app.Range', '10': 'ranges'},
    const {'1': 'blocks', '3': 4, '4': 3, '5': 13, '10': 'blocks'},
  ],
};

/// Descriptor for `DownloadQuery`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List downloadQueryDescriptor = $convert.base64Decode('Cg1Eb3dubG9hZFF1ZXJ5EhYKBnN0cmVhbRgBIAEoDVIGc3RyZWFtEiUKBnJhbmdlcxgDIAMoCzINLmZrX2FwcC5SYW5nZVIGcmFuZ2VzEhYKBmJsb2NrcxgEIAMoDVIGYmxvY2tz');
@$core.Deprecated('Use recordingDescriptor instead')
const Recording$json = const {
  '1': 'Recording',
  '2': const [
    const {'1': 'modifying', '3': 1, '4': 1, '5': 8, '10': 'modifying'},
    const {'1': 'enabled', '3': 2, '4': 1, '5': 8, '10': 'enabled'},
    const {'1': 'startedTime', '3': 3, '4': 1, '5': 4, '10': 'startedTime'},
    const {'1': 'location', '3': 4, '4': 1, '5': 11, '6': '.fk_app.Location', '10': 'location'},
  ],
};

/// Descriptor for `Recording`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List recordingDescriptor = $convert.base64Decode('CglSZWNvcmRpbmcSHAoJbW9kaWZ5aW5nGAEgASgIUgltb2RpZnlpbmcSGAoHZW5hYmxlZBgCIAEoCFIHZW5hYmxlZBIgCgtzdGFydGVkVGltZRgDIAEoBFILc3RhcnRlZFRpbWUSLAoIbG9jYXRpb24YBCABKAsyEC5ma19hcHAuTG9jYXRpb25SCGxvY2F0aW9u');
@$core.Deprecated('Use loraSettingsDescriptor instead')
const LoraSettings$json = const {
  '1': 'LoraSettings',
  '2': const [
    const {'1': 'available', '3': 1, '4': 1, '5': 8, '10': 'available'},
    const {'1': 'modifying', '3': 2, '4': 1, '5': 8, '10': 'modifying'},
    const {'1': 'clearing', '3': 3, '4': 1, '5': 8, '10': 'clearing'},
    const {'1': 'frequencyBand', '3': 4, '4': 1, '5': 13, '10': 'frequencyBand'},
    const {'1': 'deviceEui', '3': 5, '4': 1, '5': 12, '10': 'deviceEui'},
    const {'1': 'appKey', '3': 6, '4': 1, '5': 12, '10': 'appKey'},
    const {'1': 'joinEui', '3': 7, '4': 1, '5': 12, '10': 'joinEui'},
    const {'1': 'deviceAddress', '3': 8, '4': 1, '5': 12, '10': 'deviceAddress'},
    const {'1': 'networkSessionKey', '3': 9, '4': 1, '5': 12, '10': 'networkSessionKey'},
    const {'1': 'appSessionKey', '3': 10, '4': 1, '5': 12, '10': 'appSessionKey'},
  ],
};

/// Descriptor for `LoraSettings`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List loraSettingsDescriptor = $convert.base64Decode('CgxMb3JhU2V0dGluZ3MSHAoJYXZhaWxhYmxlGAEgASgIUglhdmFpbGFibGUSHAoJbW9kaWZ5aW5nGAIgASgIUgltb2RpZnlpbmcSGgoIY2xlYXJpbmcYAyABKAhSCGNsZWFyaW5nEiQKDWZyZXF1ZW5jeUJhbmQYBCABKA1SDWZyZXF1ZW5jeUJhbmQSHAoJZGV2aWNlRXVpGAUgASgMUglkZXZpY2VFdWkSFgoGYXBwS2V5GAYgASgMUgZhcHBLZXkSGAoHam9pbkV1aRgHIAEoDFIHam9pbkV1aRIkCg1kZXZpY2VBZGRyZXNzGAggASgMUg1kZXZpY2VBZGRyZXNzEiwKEW5ldHdvcmtTZXNzaW9uS2V5GAkgASgMUhFuZXR3b3JrU2Vzc2lvbktleRIkCg1hcHBTZXNzaW9uS2V5GAogASgMUg1hcHBTZXNzaW9uS2V5');
@$core.Deprecated('Use locationDescriptor instead')
const Location$json = const {
  '1': 'Location',
  '2': const [
    const {'1': 'modifying', '3': 1, '4': 1, '5': 8, '10': 'modifying'},
    const {'1': 'longitude', '3': 2, '4': 1, '5': 2, '10': 'longitude'},
    const {'1': 'latitude', '3': 3, '4': 1, '5': 2, '10': 'latitude'},
    const {'1': 'time', '3': 4, '4': 1, '5': 4, '10': 'time'},
  ],
};

/// Descriptor for `Location`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List locationDescriptor = $convert.base64Decode('CghMb2NhdGlvbhIcCgltb2RpZnlpbmcYASABKAhSCW1vZGlmeWluZxIcCglsb25naXR1ZGUYAiABKAJSCWxvbmdpdHVkZRIaCghsYXRpdHVkZRgDIAEoAlIIbGF0aXR1ZGUSEgoEdGltZRgEIAEoBFIEdGltZQ==');
@$core.Deprecated('Use wifiTransmissionDescriptor instead')
const WifiTransmission$json = const {
  '1': 'WifiTransmission',
  '2': const [
    const {'1': 'modifying', '3': 1, '4': 1, '5': 8, '10': 'modifying'},
    const {'1': 'url', '3': 2, '4': 1, '5': 9, '10': 'url'},
    const {'1': 'token', '3': 3, '4': 1, '5': 9, '10': 'token'},
    const {'1': 'enabled', '3': 4, '4': 1, '5': 8, '10': 'enabled'},
  ],
};

/// Descriptor for `WifiTransmission`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List wifiTransmissionDescriptor = $convert.base64Decode('ChBXaWZpVHJhbnNtaXNzaW9uEhwKCW1vZGlmeWluZxgBIAEoCFIJbW9kaWZ5aW5nEhAKA3VybBgCIAEoCVIDdXJsEhQKBXRva2VuGAMgASgJUgV0b2tlbhIYCgdlbmFibGVkGAQgASgIUgdlbmFibGVk');
@$core.Deprecated('Use transmissionDescriptor instead')
const Transmission$json = const {
  '1': 'Transmission',
  '2': const [
    const {'1': 'wifi', '3': 1, '4': 1, '5': 11, '6': '.fk_app.WifiTransmission', '10': 'wifi'},
  ],
};

/// Descriptor for `Transmission`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transmissionDescriptor = $convert.base64Decode('CgxUcmFuc21pc3Npb24SLAoEd2lmaRgBIAEoCzIYLmZrX2FwcC5XaWZpVHJhbnNtaXNzaW9uUgR3aWZp');
@$core.Deprecated('Use listDirectoryDescriptor instead')
const ListDirectory$json = const {
  '1': 'ListDirectory',
  '2': const [
    const {'1': 'path', '3': 1, '4': 1, '5': 9, '10': 'path'},
    const {'1': 'page', '3': 2, '4': 1, '5': 13, '10': 'page'},
  ],
};

/// Descriptor for `ListDirectory`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List listDirectoryDescriptor = $convert.base64Decode('Cg1MaXN0RGlyZWN0b3J5EhIKBHBhdGgYASABKAlSBHBhdGgSEgoEcGFnZRgCIAEoDVIEcGFnZQ==');
@$core.Deprecated('Use httpQueryDescriptor instead')
const HttpQuery$json = const {
  '1': 'HttpQuery',
  '2': const [
    const {'1': 'type', '3': 1, '4': 1, '5': 14, '6': '.fk_app.QueryType', '10': 'type'},
    const {'1': 'identity', '3': 2, '4': 1, '5': 11, '6': '.fk_app.Identity', '10': 'identity'},
    const {'1': 'recording', '3': 3, '4': 1, '5': 11, '6': '.fk_app.Recording', '10': 'recording'},
    const {'1': 'schedules', '3': 4, '4': 1, '5': 11, '6': '.fk_app.Schedules', '10': 'schedules'},
    const {'1': 'networkSettings', '3': 6, '4': 1, '5': 11, '6': '.fk_app.NetworkSettings', '10': 'networkSettings'},
    const {'1': 'loraSettings', '3': 7, '4': 1, '5': 11, '6': '.fk_app.LoraSettings', '10': 'loraSettings'},
    const {'1': 'locate', '3': 9, '4': 1, '5': 11, '6': '.fk_app.Location', '10': 'locate'},
    const {'1': 'transmission', '3': 10, '4': 1, '5': 11, '6': '.fk_app.Transmission', '10': 'transmission'},
    const {'1': 'directory', '3': 11, '4': 1, '5': 11, '6': '.fk_app.ListDirectory', '10': 'directory'},
    const {'1': 'flags', '3': 5, '4': 1, '5': 13, '10': 'flags'},
    const {'1': 'time', '3': 8, '4': 1, '5': 4, '10': 'time'},
    const {'1': 'counter', '3': 12, '4': 1, '5': 13, '10': 'counter'},
  ],
};

/// Descriptor for `HttpQuery`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List httpQueryDescriptor = $convert.base64Decode('CglIdHRwUXVlcnkSJQoEdHlwZRgBIAEoDjIRLmZrX2FwcC5RdWVyeVR5cGVSBHR5cGUSLAoIaWRlbnRpdHkYAiABKAsyEC5ma19hcHAuSWRlbnRpdHlSCGlkZW50aXR5Ei8KCXJlY29yZGluZxgDIAEoCzIRLmZrX2FwcC5SZWNvcmRpbmdSCXJlY29yZGluZxIvCglzY2hlZHVsZXMYBCABKAsyES5ma19hcHAuU2NoZWR1bGVzUglzY2hlZHVsZXMSQQoPbmV0d29ya1NldHRpbmdzGAYgASgLMhcuZmtfYXBwLk5ldHdvcmtTZXR0aW5nc1IPbmV0d29ya1NldHRpbmdzEjgKDGxvcmFTZXR0aW5ncxgHIAEoCzIULmZrX2FwcC5Mb3JhU2V0dGluZ3NSDGxvcmFTZXR0aW5ncxIoCgZsb2NhdGUYCSABKAsyEC5ma19hcHAuTG9jYXRpb25SBmxvY2F0ZRI4Cgx0cmFuc21pc3Npb24YCiABKAsyFC5ma19hcHAuVHJhbnNtaXNzaW9uUgx0cmFuc21pc3Npb24SMwoJZGlyZWN0b3J5GAsgASgLMhUuZmtfYXBwLkxpc3REaXJlY3RvcnlSCWRpcmVjdG9yeRIUCgVmbGFncxgFIAEoDVIFZmxhZ3MSEgoEdGltZRgIIAEoBFIEdGltZRIYCgdjb3VudGVyGAwgASgNUgdjb3VudGVy');
@$core.Deprecated('Use dataStreamDescriptor instead')
const DataStream$json = const {
  '1': 'DataStream',
  '2': const [
    const {'1': 'id', '3': 1, '4': 1, '5': 13, '10': 'id'},
    const {'1': 'time', '3': 2, '4': 1, '5': 4, '10': 'time'},
    const {'1': 'size', '3': 3, '4': 1, '5': 4, '10': 'size'},
    const {'1': 'version', '3': 4, '4': 1, '5': 13, '10': 'version'},
    const {'1': 'block', '3': 5, '4': 1, '5': 4, '10': 'block'},
    const {'1': 'hash', '3': 6, '4': 1, '5': 12, '10': 'hash'},
    const {'1': 'name', '3': 7, '4': 1, '5': 9, '10': 'name'},
    const {'1': 'path', '3': 8, '4': 1, '5': 9, '10': 'path'},
  ],
};

/// Descriptor for `DataStream`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List dataStreamDescriptor = $convert.base64Decode('CgpEYXRhU3RyZWFtEg4KAmlkGAEgASgNUgJpZBISCgR0aW1lGAIgASgEUgR0aW1lEhIKBHNpemUYAyABKARSBHNpemUSGAoHdmVyc2lvbhgEIAEoDVIHdmVyc2lvbhIUCgVibG9jaxgFIAEoBFIFYmxvY2sSEgoEaGFzaBgGIAEoDFIEaGFzaBISCgRuYW1lGAcgASgJUgRuYW1lEhIKBHBhdGgYCCABKAlSBHBhdGg=');
@$core.Deprecated('Use liveSensorReadingDescriptor instead')
const LiveSensorReading$json = const {
  '1': 'LiveSensorReading',
  '2': const [
    const {'1': 'sensor', '3': 1, '4': 1, '5': 11, '6': '.fk_app.SensorCapabilities', '10': 'sensor'},
    const {'1': 'value', '3': 2, '4': 1, '5': 2, '10': 'value'},
    const {'1': 'uncalibrated', '3': 3, '4': 1, '5': 2, '10': 'uncalibrated'},
    const {'1': 'factory', '3': 4, '4': 1, '5': 2, '10': 'factory'},
    const {'1': 'adc', '3': 5, '4': 1, '5': 12, '10': 'adc'},
  ],
};

/// Descriptor for `LiveSensorReading`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List liveSensorReadingDescriptor = $convert.base64Decode('ChFMaXZlU2Vuc29yUmVhZGluZxIyCgZzZW5zb3IYASABKAsyGi5ma19hcHAuU2Vuc29yQ2FwYWJpbGl0aWVzUgZzZW5zb3ISFAoFdmFsdWUYAiABKAJSBXZhbHVlEiIKDHVuY2FsaWJyYXRlZBgDIAEoAlIMdW5jYWxpYnJhdGVkEhgKB2ZhY3RvcnkYBCABKAJSB2ZhY3RvcnkSEAoDYWRjGAUgASgMUgNhZGM=');
@$core.Deprecated('Use liveModuleReadingsDescriptor instead')
const LiveModuleReadings$json = const {
  '1': 'LiveModuleReadings',
  '2': const [
    const {'1': 'module', '3': 1, '4': 1, '5': 11, '6': '.fk_app.ModuleCapabilities', '10': 'module'},
    const {'1': 'readings', '3': 2, '4': 3, '5': 11, '6': '.fk_app.LiveSensorReading', '10': 'readings'},
  ],
};

/// Descriptor for `LiveModuleReadings`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List liveModuleReadingsDescriptor = $convert.base64Decode('ChJMaXZlTW9kdWxlUmVhZGluZ3MSMgoGbW9kdWxlGAEgASgLMhouZmtfYXBwLk1vZHVsZUNhcGFiaWxpdGllc1IGbW9kdWxlEjUKCHJlYWRpbmdzGAIgAygLMhkuZmtfYXBwLkxpdmVTZW5zb3JSZWFkaW5nUghyZWFkaW5ncw==');
@$core.Deprecated('Use liveReadingsDescriptor instead')
const LiveReadings$json = const {
  '1': 'LiveReadings',
  '2': const [
    const {'1': 'time', '3': 1, '4': 1, '5': 4, '10': 'time'},
    const {'1': 'modules', '3': 2, '4': 3, '5': 11, '6': '.fk_app.LiveModuleReadings', '10': 'modules'},
  ],
};

/// Descriptor for `LiveReadings`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List liveReadingsDescriptor = $convert.base64Decode('CgxMaXZlUmVhZGluZ3MSEgoEdGltZRgBIAEoBFIEdGltZRI0Cgdtb2R1bGVzGAIgAygLMhouZmtfYXBwLkxpdmVNb2R1bGVSZWFkaW5nc1IHbW9kdWxlcw==');
@$core.Deprecated('Use directoryEntryDescriptor instead')
const DirectoryEntry$json = const {
  '1': 'DirectoryEntry',
  '2': const [
    const {'1': 'name', '3': 1, '4': 1, '5': 9, '10': 'name'},
    const {'1': 'path', '3': 2, '4': 1, '5': 9, '10': 'path'},
    const {'1': 'size', '3': 3, '4': 1, '5': 13, '10': 'size'},
    const {'1': 'directory', '3': 4, '4': 1, '5': 8, '10': 'directory'},
  ],
};

/// Descriptor for `DirectoryEntry`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List directoryEntryDescriptor = $convert.base64Decode('Cg5EaXJlY3RvcnlFbnRyeRISCgRuYW1lGAEgASgJUgRuYW1lEhIKBHBhdGgYAiABKAlSBHBhdGgSEgoEc2l6ZRgDIAEoDVIEc2l6ZRIcCglkaXJlY3RvcnkYBCABKAhSCWRpcmVjdG9yeQ==');
@$core.Deprecated('Use directoryListingDescriptor instead')
const DirectoryListing$json = const {
  '1': 'DirectoryListing',
  '2': const [
    const {'1': 'path', '3': 1, '4': 1, '5': 9, '10': 'path'},
    const {'1': 'totalEntries', '3': 3, '4': 1, '5': 13, '10': 'totalEntries'},
    const {'1': 'entries', '3': 2, '4': 3, '5': 11, '6': '.fk_app.DirectoryEntry', '10': 'entries'},
  ],
};

/// Descriptor for `DirectoryListing`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List directoryListingDescriptor = $convert.base64Decode('ChBEaXJlY3RvcnlMaXN0aW5nEhIKBHBhdGgYASABKAlSBHBhdGgSIgoMdG90YWxFbnRyaWVzGAMgASgNUgx0b3RhbEVudHJpZXMSMAoHZW50cmllcxgCIAMoCzIWLmZrX2FwcC5EaXJlY3RvcnlFbnRyeVIHZW50cmllcw==');
@$core.Deprecated('Use nearbyNetworkDescriptor instead')
const NearbyNetwork$json = const {
  '1': 'NearbyNetwork',
  '2': const [
    const {'1': 'ssid', '3': 1, '4': 1, '5': 9, '10': 'ssid'},
  ],
};

/// Descriptor for `NearbyNetwork`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List nearbyNetworkDescriptor = $convert.base64Decode('Cg1OZWFyYnlOZXR3b3JrEhIKBHNzaWQYASABKAlSBHNzaWQ=');
@$core.Deprecated('Use nearbyNetworksDescriptor instead')
const NearbyNetworks$json = const {
  '1': 'NearbyNetworks',
  '2': const [
    const {'1': 'networks', '3': 1, '4': 3, '5': 11, '6': '.fk_app.NearbyNetwork', '10': 'networks'},
  ],
};

/// Descriptor for `NearbyNetworks`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List nearbyNetworksDescriptor = $convert.base64Decode('Cg5OZWFyYnlOZXR3b3JrcxIxCghuZXR3b3JrcxgBIAMoCzIVLmZrX2FwcC5OZWFyYnlOZXR3b3JrUghuZXR3b3Jrcw==');
@$core.Deprecated('Use faultDescriptor instead')
const Fault$json = const {
  '1': 'Fault',
  '2': const [
    const {'1': 'time', '3': 1, '4': 1, '5': 13, '10': 'time'},
    const {'1': 'code', '3': 2, '4': 1, '5': 13, '10': 'code'},
    const {'1': 'description', '3': 3, '4': 1, '5': 9, '10': 'description'},
    const {'1': 'debug', '3': 4, '4': 1, '5': 12, '10': 'debug'},
  ],
};

/// Descriptor for `Fault`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List faultDescriptor = $convert.base64Decode('CgVGYXVsdBISCgR0aW1lGAEgASgNUgR0aW1lEhIKBGNvZGUYAiABKA1SBGNvZGUSIAoLZGVzY3JpcHRpb24YAyABKAlSC2Rlc2NyaXB0aW9uEhQKBWRlYnVnGAQgASgMUgVkZWJ1Zw==');
@$core.Deprecated('Use httpReplyDescriptor instead')
const HttpReply$json = const {
  '1': 'HttpReply',
  '2': const [
    const {'1': 'type', '3': 1, '4': 1, '5': 14, '6': '.fk_app.ReplyType', '10': 'type'},
    const {'1': 'errors', '3': 2, '4': 3, '5': 11, '6': '.fk_app.Error', '10': 'errors'},
    const {'1': 'status', '3': 3, '4': 1, '5': 11, '6': '.fk_app.Status', '10': 'status'},
    const {'1': 'networkSettings', '3': 4, '4': 1, '5': 11, '6': '.fk_app.NetworkSettings', '10': 'networkSettings'},
    const {'1': 'loraSettings', '3': 8, '4': 1, '5': 11, '6': '.fk_app.LoraSettings', '10': 'loraSettings'},
    const {'1': 'modules', '3': 5, '4': 3, '5': 11, '6': '.fk_app.ModuleCapabilities', '10': 'modules'},
    const {'1': 'streams', '3': 6, '4': 3, '5': 11, '6': '.fk_app.DataStream', '10': 'streams'},
    const {'1': 'liveReadings', '3': 7, '4': 3, '5': 11, '6': '.fk_app.LiveReadings', '10': 'liveReadings'},
    const {'1': 'schedules', '3': 9, '4': 1, '5': 11, '6': '.fk_app.Schedules', '10': 'schedules'},
    const {'1': 'transmission', '3': 10, '4': 1, '5': 11, '6': '.fk_app.Transmission', '10': 'transmission'},
    const {'1': 'listing', '3': 11, '4': 1, '5': 11, '6': '.fk_app.DirectoryListing', '10': 'listing'},
    const {'1': 'nearbyNetworks', '3': 12, '4': 1, '5': 11, '6': '.fk_app.NearbyNetworks', '10': 'nearbyNetworks'},
    const {'1': 'faults', '3': 13, '4': 3, '5': 11, '6': '.fk_app.Fault', '10': 'faults'},
  ],
};

/// Descriptor for `HttpReply`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List httpReplyDescriptor = $convert.base64Decode('CglIdHRwUmVwbHkSJQoEdHlwZRgBIAEoDjIRLmZrX2FwcC5SZXBseVR5cGVSBHR5cGUSJQoGZXJyb3JzGAIgAygLMg0uZmtfYXBwLkVycm9yUgZlcnJvcnMSJgoGc3RhdHVzGAMgASgLMg4uZmtfYXBwLlN0YXR1c1IGc3RhdHVzEkEKD25ldHdvcmtTZXR0aW5ncxgEIAEoCzIXLmZrX2FwcC5OZXR3b3JrU2V0dGluZ3NSD25ldHdvcmtTZXR0aW5ncxI4Cgxsb3JhU2V0dGluZ3MYCCABKAsyFC5ma19hcHAuTG9yYVNldHRpbmdzUgxsb3JhU2V0dGluZ3MSNAoHbW9kdWxlcxgFIAMoCzIaLmZrX2FwcC5Nb2R1bGVDYXBhYmlsaXRpZXNSB21vZHVsZXMSLAoHc3RyZWFtcxgGIAMoCzISLmZrX2FwcC5EYXRhU3RyZWFtUgdzdHJlYW1zEjgKDGxpdmVSZWFkaW5ncxgHIAMoCzIULmZrX2FwcC5MaXZlUmVhZGluZ3NSDGxpdmVSZWFkaW5ncxIvCglzY2hlZHVsZXMYCSABKAsyES5ma19hcHAuU2NoZWR1bGVzUglzY2hlZHVsZXMSOAoMdHJhbnNtaXNzaW9uGAogASgLMhQuZmtfYXBwLlRyYW5zbWlzc2lvblIMdHJhbnNtaXNzaW9uEjIKB2xpc3RpbmcYCyABKAsyGC5ma19hcHAuRGlyZWN0b3J5TGlzdGluZ1IHbGlzdGluZxI+Cg5uZWFyYnlOZXR3b3JrcxgMIAEoCzIWLmZrX2FwcC5OZWFyYnlOZXR3b3Jrc1IObmVhcmJ5TmV0d29ya3MSJQoGZmF1bHRzGA0gAygLMg0uZmtfYXBwLkZhdWx0UgZmYXVsdHM=');
@$core.Deprecated('Use moduleHttpQueryDescriptor instead')
const ModuleHttpQuery$json = const {
  '1': 'ModuleHttpQuery',
  '2': const [
    const {'1': 'type', '3': 1, '4': 1, '5': 14, '6': '.fk_app.ModuleQueryType', '10': 'type'},
    const {'1': 'errors', '3': 2, '4': 3, '5': 11, '6': '.fk_app.Error', '10': 'errors'},
    const {'1': 'configuration', '3': 3, '4': 1, '5': 12, '10': 'configuration'},
  ],
};

/// Descriptor for `ModuleHttpQuery`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List moduleHttpQueryDescriptor = $convert.base64Decode('Cg9Nb2R1bGVIdHRwUXVlcnkSKwoEdHlwZRgBIAEoDjIXLmZrX2FwcC5Nb2R1bGVRdWVyeVR5cGVSBHR5cGUSJQoGZXJyb3JzGAIgAygLMg0uZmtfYXBwLkVycm9yUgZlcnJvcnMSJAoNY29uZmlndXJhdGlvbhgDIAEoDFINY29uZmlndXJhdGlvbg==');
@$core.Deprecated('Use moduleHttpReplyDescriptor instead')
const ModuleHttpReply$json = const {
  '1': 'ModuleHttpReply',
  '2': const [
    const {'1': 'type', '3': 1, '4': 1, '5': 14, '6': '.fk_app.ModuleReplyType', '10': 'type'},
    const {'1': 'errors', '3': 2, '4': 3, '5': 11, '6': '.fk_app.Error', '10': 'errors'},
    const {'1': 'configuration', '3': 3, '4': 1, '5': 12, '10': 'configuration'},
  ],
};

/// Descriptor for `ModuleHttpReply`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List moduleHttpReplyDescriptor = $convert.base64Decode('Cg9Nb2R1bGVIdHRwUmVwbHkSKwoEdHlwZRgBIAEoDjIXLmZrX2FwcC5Nb2R1bGVSZXBseVR5cGVSBHR5cGUSJQoGZXJyb3JzGAIgAygLMg0uZmtfYXBwLkVycm9yUgZlcnJvcnMSJAoNY29uZmlndXJhdGlvbhgDIAEoDFINY29uZmlndXJhdGlvbg==');
@$core.Deprecated('Use udpMessageDescriptor instead')
const UdpMessage$json = const {
  '1': 'UdpMessage',
  '2': const [
    const {'1': 'deviceId', '3': 1, '4': 1, '5': 12, '10': 'deviceId'},
    const {'1': 'status', '3': 2, '4': 1, '5': 14, '6': '.fk_app.UdpStatus', '10': 'status'},
    const {'1': 'counter', '3': 3, '4': 1, '5': 13, '10': 'counter'},
  ],
};

/// Descriptor for `UdpMessage`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List udpMessageDescriptor = $convert.base64Decode('CgpVZHBNZXNzYWdlEhoKCGRldmljZUlkGAEgASgMUghkZXZpY2VJZBIpCgZzdGF0dXMYAiABKA4yES5ma19hcHAuVWRwU3RhdHVzUgZzdGF0dXMSGAoHY291bnRlchgDIAEoDVIHY291bnRlcg==');
