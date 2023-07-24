//
//  Generated code. Do not modify.
//  source: fk-app.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use queryFlagsDescriptor instead')
const QueryFlags$json = {
  '1': 'QueryFlags',
  '2': [
    {'1': 'QUERY_FLAGS_NONE', '2': 0},
    {'1': 'QUERY_FLAGS_LOGS', '2': 1},
  ],
};

/// Descriptor for `QueryFlags`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List queryFlagsDescriptor = $convert.base64Decode(
    'CgpRdWVyeUZsYWdzEhQKEFFVRVJZX0ZMQUdTX05PTkUQABIUChBRVUVSWV9GTEFHU19MT0dTEA'
    'E=');

@$core.Deprecated('Use queryTypeDescriptor instead')
const QueryType$json = {
  '1': 'QueryType',
  '2': [
    {'1': 'QUERY_NONE', '2': 0},
    {'1': 'QUERY_CAPABILITIES', '2': 1},
    {'1': 'QUERY_CONFIGURE_SENSOR', '2': 2},
    {'1': 'QUERY_LIVE_DATA_POLL', '2': 7},
    {'1': 'QUERY_SCHEDULES', '2': 8},
    {'1': 'QUERY_CONFIGURE_SCHEDULES', '2': 9},
    {'1': 'QUERY_FILES_SD', '2': 10},
    {'1': 'QUERY_DOWNLOAD_FILE', '2': 11},
    {'1': 'QUERY_ERASE_FILE', '2': 12},
    {'1': 'QUERY_RESET', '2': 13},
    {'1': 'QUERY_NETWORK_SETTINGS', '2': 14},
    {'1': 'QUERY_CONFIGURE_NETWORK_SETTINGS', '2': 15},
    {'1': 'QUERY_SCAN_MODULES', '2': 16},
    {'1': 'QUERY_CONFIGURE_IDENTITY', '2': 17},
    {'1': 'QUERY_STATUS', '2': 18},
    {'1': 'QUERY_MODULE', '2': 19},
    {'1': 'QUERY_METADATA', '2': 20},
    {'1': 'QUERY_FORMAT', '2': 21},
    {'1': 'QUERY_GET_READINGS', '2': 22},
    {'1': 'QUERY_TAKE_READINGS', '2': 23},
    {'1': 'QUERY_RECORDING_CONTROL', '2': 24},
    {'1': 'QUERY_CONFIGURE', '2': 25},
    {'1': 'QUERY_SCAN_NETWORKS', '2': 26},
    {'1': 'QUERY_FILES_SPI', '2': 27},
    {'1': 'QUERY_FILES_QSPI', '2': 28},
  ],
};

/// Descriptor for `QueryType`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List queryTypeDescriptor = $convert.base64Decode(
    'CglRdWVyeVR5cGUSDgoKUVVFUllfTk9ORRAAEhYKElFVRVJZX0NBUEFCSUxJVElFUxABEhoKFl'
    'FVRVJZX0NPTkZJR1VSRV9TRU5TT1IQAhIYChRRVUVSWV9MSVZFX0RBVEFfUE9MTBAHEhMKD1FV'
    'RVJZX1NDSEVEVUxFUxAIEh0KGVFVRVJZX0NPTkZJR1VSRV9TQ0hFRFVMRVMQCRISCg5RVUVSWV'
    '9GSUxFU19TRBAKEhcKE1FVRVJZX0RPV05MT0FEX0ZJTEUQCxIUChBRVUVSWV9FUkFTRV9GSUxF'
    'EAwSDwoLUVVFUllfUkVTRVQQDRIaChZRVUVSWV9ORVRXT1JLX1NFVFRJTkdTEA4SJAogUVVFUl'
    'lfQ09ORklHVVJFX05FVFdPUktfU0VUVElOR1MQDxIWChJRVUVSWV9TQ0FOX01PRFVMRVMQEBIc'
    'ChhRVUVSWV9DT05GSUdVUkVfSURFTlRJVFkQERIQCgxRVUVSWV9TVEFUVVMQEhIQCgxRVUVSWV'
    '9NT0RVTEUQExISCg5RVUVSWV9NRVRBREFUQRAUEhAKDFFVRVJZX0ZPUk1BVBAVEhYKElFVRVJZ'
    'X0dFVF9SRUFESU5HUxAWEhcKE1FVRVJZX1RBS0VfUkVBRElOR1MQFxIbChdRVUVSWV9SRUNPUk'
    'RJTkdfQ09OVFJPTBAYEhMKD1FVRVJZX0NPTkZJR1VSRRAZEhcKE1FVRVJZX1NDQU5fTkVUV09S'
    'S1MQGhITCg9RVUVSWV9GSUxFU19TUEkQGxIUChBRVUVSWV9GSUxFU19RU1BJEBw=');

@$core.Deprecated('Use replyTypeDescriptor instead')
const ReplyType$json = {
  '1': 'ReplyType',
  '2': [
    {'1': 'REPLY_NONE', '2': 0},
    {'1': 'REPLY_SUCCESS', '2': 1},
    {'1': 'REPLY_BUSY', '2': 2},
    {'1': 'REPLY_ERROR', '2': 3},
    {'1': 'REPLY_CAPABILITIES', '2': 4},
    {'1': 'REPLY_LIVE_DATA_POLL', '2': 8},
    {'1': 'REPLY_SCHEDULES', '2': 9},
    {'1': 'REPLY_FILES', '2': 10},
    {'1': 'REPLY_DOWNLOAD_FILE', '2': 11},
    {'1': 'REPLY_RESET', '2': 12},
    {'1': 'REPLY_NETWORK_SETTINGS', '2': 13},
    {'1': 'REPLY_IDENTITY', '2': 14},
    {'1': 'REPLY_STATUS', '2': 15},
    {'1': 'REPLY_MODULE', '2': 16},
    {'1': 'REPLY_METADATA', '2': 17},
    {'1': 'REPLY_READINGS', '2': 18},
    {'1': 'REPLY_NETWORKS', '2': 19},
  ],
};

/// Descriptor for `ReplyType`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List replyTypeDescriptor = $convert.base64Decode(
    'CglSZXBseVR5cGUSDgoKUkVQTFlfTk9ORRAAEhEKDVJFUExZX1NVQ0NFU1MQARIOCgpSRVBMWV'
    '9CVVNZEAISDwoLUkVQTFlfRVJST1IQAxIWChJSRVBMWV9DQVBBQklMSVRJRVMQBBIYChRSRVBM'
    'WV9MSVZFX0RBVEFfUE9MTBAIEhMKD1JFUExZX1NDSEVEVUxFUxAJEg8KC1JFUExZX0ZJTEVTEA'
    'oSFwoTUkVQTFlfRE9XTkxPQURfRklMRRALEg8KC1JFUExZX1JFU0VUEAwSGgoWUkVQTFlfTkVU'
    'V09SS19TRVRUSU5HUxANEhIKDlJFUExZX0lERU5USVRZEA4SEAoMUkVQTFlfU1RBVFVTEA8SEA'
    'oMUkVQTFlfTU9EVUxFEBASEgoOUkVQTFlfTUVUQURBVEEQERISCg5SRVBMWV9SRUFESU5HUxAS'
    'EhIKDlJFUExZX05FVFdPUktTEBM=');

@$core.Deprecated('Use downloadFlagsDescriptor instead')
const DownloadFlags$json = {
  '1': 'DownloadFlags',
  '2': [
    {'1': 'DOWNLOAD_FLAG_NONE', '2': 0},
    {'1': 'DOWNLOAD_FLAG_METADATA_PREPEND', '2': 1},
    {'1': 'DOWNLOAD_FLAG_METADATA_ONLY', '2': 2},
  ],
};

/// Descriptor for `DownloadFlags`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List downloadFlagsDescriptor = $convert.base64Decode(
    'Cg1Eb3dubG9hZEZsYWdzEhYKEkRPV05MT0FEX0ZMQUdfTk9ORRAAEiIKHkRPV05MT0FEX0ZMQU'
    'dfTUVUQURBVEFfUFJFUEVORBABEh8KG0RPV05MT0FEX0ZMQUdfTUVUQURBVEFfT05MWRAC');

@$core.Deprecated('Use moduleFlagsDescriptor instead')
const ModuleFlags$json = {
  '1': 'ModuleFlags',
  '2': [
    {'1': 'MODULE_FLAG_NONE', '2': 0},
    {'1': 'MODULE_FLAG_INTERNAL', '2': 1},
  ],
};

/// Descriptor for `ModuleFlags`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List moduleFlagsDescriptor = $convert.base64Decode(
    'CgtNb2R1bGVGbGFncxIUChBNT0RVTEVfRkxBR19OT05FEAASGAoUTU9EVUxFX0ZMQUdfSU5URV'
    'JOQUwQAQ==');

@$core.Deprecated('Use sensorFlagsDescriptor instead')
const SensorFlags$json = {
  '1': 'SensorFlags',
  '2': [
    {'1': 'SENSOR_FLAG_NONE', '2': 0},
  ],
};

/// Descriptor for `SensorFlags`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List sensorFlagsDescriptor = $convert.base64Decode(
    'CgtTZW5zb3JGbGFncxIUChBTRU5TT1JfRkxBR19OT05FEAA=');

@$core.Deprecated('Use moduleQueryTypeDescriptor instead')
const ModuleQueryType$json = {
  '1': 'ModuleQueryType',
  '2': [
    {'1': 'MODULE_QUERY_NONE', '2': 0},
    {'1': 'MODULE_QUERY_STATUS', '2': 1},
    {'1': 'MODULE_QUERY_CONFIGURE', '2': 2},
    {'1': 'MODULE_QUERY_RESET', '2': 3},
  ],
};

/// Descriptor for `ModuleQueryType`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List moduleQueryTypeDescriptor = $convert.base64Decode(
    'Cg9Nb2R1bGVRdWVyeVR5cGUSFQoRTU9EVUxFX1FVRVJZX05PTkUQABIXChNNT0RVTEVfUVVFUl'
    'lfU1RBVFVTEAESGgoWTU9EVUxFX1FVRVJZX0NPTkZJR1VSRRACEhYKEk1PRFVMRV9RVUVSWV9S'
    'RVNFVBAD');

@$core.Deprecated('Use moduleReplyTypeDescriptor instead')
const ModuleReplyType$json = {
  '1': 'ModuleReplyType',
  '2': [
    {'1': 'MODULE_REPLY_NONE', '2': 0},
    {'1': 'MODULE_REPLY_SUCCESS', '2': 1},
    {'1': 'MODULE_REPLY_BUSY', '2': 2},
    {'1': 'MODULE_REPLY_ERROR', '2': 3},
  ],
};

/// Descriptor for `ModuleReplyType`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List moduleReplyTypeDescriptor = $convert.base64Decode(
    'Cg9Nb2R1bGVSZXBseVR5cGUSFQoRTU9EVUxFX1JFUExZX05PTkUQABIYChRNT0RVTEVfUkVQTF'
    'lfU1VDQ0VTUxABEhUKEU1PRFVMRV9SRVBMWV9CVVNZEAISFgoSTU9EVUxFX1JFUExZX0VSUk9S'
    'EAM=');

@$core.Deprecated('Use udpStatusDescriptor instead')
const UdpStatus$json = {
  '1': 'UdpStatus',
  '2': [
    {'1': 'UDP_STATUS_ONLINE', '2': 0},
    {'1': 'UDP_STATUS_BYE', '2': 1},
  ],
};

/// Descriptor for `UdpStatus`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List udpStatusDescriptor = $convert.base64Decode(
    'CglVZHBTdGF0dXMSFQoRVURQX1NUQVRVU19PTkxJTkUQABISCg5VRFBfU1RBVFVTX0JZRRAB');

@$core.Deprecated('Use queryCapabilitiesDescriptor instead')
const QueryCapabilities$json = {
  '1': 'QueryCapabilities',
  '2': [
    {'1': 'version', '3': 1, '4': 1, '5': 13, '10': 'version'},
    {'1': 'callerTime', '3': 2, '4': 1, '5': 13, '10': 'callerTime'},
  ],
};

/// Descriptor for `QueryCapabilities`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List queryCapabilitiesDescriptor = $convert.base64Decode(
    'ChFRdWVyeUNhcGFiaWxpdGllcxIYCgd2ZXJzaW9uGAEgASgNUgd2ZXJzaW9uEh4KCmNhbGxlcl'
    'RpbWUYAiABKA1SCmNhbGxlclRpbWU=');

@$core.Deprecated('Use liveValueDescriptor instead')
const LiveValue$json = {
  '1': 'LiveValue',
  '2': [
    {'1': 'valid', '3': 1, '4': 1, '5': 8, '10': 'valid'},
    {'1': 'value', '3': 2, '4': 1, '5': 2, '10': 'value'},
    {'1': 'uncalibrated', '3': 3, '4': 1, '5': 2, '10': 'uncalibrated'},
  ],
};

/// Descriptor for `LiveValue`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List liveValueDescriptor = $convert.base64Decode(
    'CglMaXZlVmFsdWUSFAoFdmFsaWQYASABKAhSBXZhbGlkEhQKBXZhbHVlGAIgASgCUgV2YWx1ZR'
    'IiCgx1bmNhbGlicmF0ZWQYAyABKAJSDHVuY2FsaWJyYXRlZA==');

@$core.Deprecated('Use sensorCapabilitiesDescriptor instead')
const SensorCapabilities$json = {
  '1': 'SensorCapabilities',
  '2': [
    {'1': 'number', '3': 1, '4': 1, '5': 13, '10': 'number'},
    {'1': 'module', '3': 2, '4': 1, '5': 13, '10': 'module'},
    {'1': 'name', '3': 3, '4': 1, '5': 9, '10': 'name'},
    {'1': 'frequency', '3': 4, '4': 1, '5': 13, '10': 'frequency'},
    {'1': 'unitOfMeasure', '3': 5, '4': 1, '5': 9, '10': 'unitOfMeasure'},
    {'1': 'uncalibratedUnitOfMeasure', '3': 9, '4': 1, '5': 9, '10': 'uncalibratedUnitOfMeasure'},
    {'1': 'path', '3': 6, '4': 1, '5': 9, '10': 'path'},
    {'1': 'flags', '3': 7, '4': 1, '5': 13, '10': 'flags'},
    {'1': 'value', '3': 8, '4': 1, '5': 11, '6': '.fk_app.LiveValue', '10': 'value'},
  ],
};

/// Descriptor for `SensorCapabilities`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List sensorCapabilitiesDescriptor = $convert.base64Decode(
    'ChJTZW5zb3JDYXBhYmlsaXRpZXMSFgoGbnVtYmVyGAEgASgNUgZudW1iZXISFgoGbW9kdWxlGA'
    'IgASgNUgZtb2R1bGUSEgoEbmFtZRgDIAEoCVIEbmFtZRIcCglmcmVxdWVuY3kYBCABKA1SCWZy'
    'ZXF1ZW5jeRIkCg11bml0T2ZNZWFzdXJlGAUgASgJUg11bml0T2ZNZWFzdXJlEjwKGXVuY2FsaW'
    'JyYXRlZFVuaXRPZk1lYXN1cmUYCSABKAlSGXVuY2FsaWJyYXRlZFVuaXRPZk1lYXN1cmUSEgoE'
    'cGF0aBgGIAEoCVIEcGF0aBIUCgVmbGFncxgHIAEoDVIFZmxhZ3MSJwoFdmFsdWUYCCABKAsyES'
    '5ma19hcHAuTGl2ZVZhbHVlUgV2YWx1ZQ==');

@$core.Deprecated('Use moduleHeaderDescriptor instead')
const ModuleHeader$json = {
  '1': 'ModuleHeader',
  '2': [
    {'1': 'manufacturer', '3': 1, '4': 1, '5': 13, '10': 'manufacturer'},
    {'1': 'kind', '3': 2, '4': 1, '5': 13, '10': 'kind'},
    {'1': 'version', '3': 3, '4': 1, '5': 13, '10': 'version'},
  ],
};

/// Descriptor for `ModuleHeader`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List moduleHeaderDescriptor = $convert.base64Decode(
    'CgxNb2R1bGVIZWFkZXISIgoMbWFudWZhY3R1cmVyGAEgASgNUgxtYW51ZmFjdHVyZXISEgoEa2'
    'luZBgCIAEoDVIEa2luZBIYCgd2ZXJzaW9uGAMgASgNUgd2ZXJzaW9u');

@$core.Deprecated('Use moduleCapabilitiesDescriptor instead')
const ModuleCapabilities$json = {
  '1': 'ModuleCapabilities',
  '2': [
    {'1': 'position', '3': 1, '4': 1, '5': 13, '10': 'position'},
    {'1': 'name', '3': 2, '4': 1, '5': 9, '10': 'name'},
    {'1': 'sensors', '3': 3, '4': 3, '5': 11, '6': '.fk_app.SensorCapabilities', '10': 'sensors'},
    {'1': 'path', '3': 4, '4': 1, '5': 9, '10': 'path'},
    {'1': 'flags', '3': 5, '4': 1, '5': 13, '10': 'flags'},
    {'1': 'id', '3': 6, '4': 1, '5': 12, '10': 'id'},
    {'1': 'header', '3': 7, '4': 1, '5': 11, '6': '.fk_app.ModuleHeader', '10': 'header'},
    {'1': 'configuration', '3': 8, '4': 1, '5': 12, '10': 'configuration'},
  ],
};

/// Descriptor for `ModuleCapabilities`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List moduleCapabilitiesDescriptor = $convert.base64Decode(
    'ChJNb2R1bGVDYXBhYmlsaXRpZXMSGgoIcG9zaXRpb24YASABKA1SCHBvc2l0aW9uEhIKBG5hbW'
    'UYAiABKAlSBG5hbWUSNAoHc2Vuc29ycxgDIAMoCzIaLmZrX2FwcC5TZW5zb3JDYXBhYmlsaXRp'
    'ZXNSB3NlbnNvcnMSEgoEcGF0aBgEIAEoCVIEcGF0aBIUCgVmbGFncxgFIAEoDVIFZmxhZ3MSDg'
    'oCaWQYBiABKAxSAmlkEiwKBmhlYWRlchgHIAEoCzIULmZrX2FwcC5Nb2R1bGVIZWFkZXJSBmhl'
    'YWRlchIkCg1jb25maWd1cmF0aW9uGAggASgMUg1jb25maWd1cmF0aW9u');

@$core.Deprecated('Use capabilitiesDescriptor instead')
const Capabilities$json = {
  '1': 'Capabilities',
  '2': [
    {'1': 'version', '3': 1, '4': 1, '5': 13, '10': 'version'},
    {'1': 'deviceId', '3': 2, '4': 1, '5': 12, '10': 'deviceId'},
    {'1': 'name', '3': 3, '4': 1, '5': 9, '10': 'name'},
    {'1': 'modules', '3': 4, '4': 3, '5': 11, '6': '.fk_app.ModuleCapabilities', '10': 'modules'},
    {'1': 'sensors', '3': 5, '4': 3, '5': 11, '6': '.fk_app.SensorCapabilities', '10': 'sensors'},
  ],
};

/// Descriptor for `Capabilities`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List capabilitiesDescriptor = $convert.base64Decode(
    'CgxDYXBhYmlsaXRpZXMSGAoHdmVyc2lvbhgBIAEoDVIHdmVyc2lvbhIaCghkZXZpY2VJZBgCIA'
    'EoDFIIZGV2aWNlSWQSEgoEbmFtZRgDIAEoCVIEbmFtZRI0Cgdtb2R1bGVzGAQgAygLMhouZmtf'
    'YXBwLk1vZHVsZUNhcGFiaWxpdGllc1IHbW9kdWxlcxI0CgdzZW5zb3JzGAUgAygLMhouZmtfYX'
    'BwLlNlbnNvckNhcGFiaWxpdGllc1IHc2Vuc29ycw==');

@$core.Deprecated('Use networkInfoDescriptor instead')
const NetworkInfo$json = {
  '1': 'NetworkInfo',
  '2': [
    {'1': 'ssid', '3': 1, '4': 1, '5': 9, '10': 'ssid'},
    {'1': 'password', '3': 2, '4': 1, '5': 9, '10': 'password'},
    {'1': 'create', '3': 3, '4': 1, '5': 8, '10': 'create'},
    {'1': 'preferred', '3': 4, '4': 1, '5': 8, '10': 'preferred'},
    {'1': 'keeping', '3': 5, '4': 1, '5': 8, '10': 'keeping'},
  ],
};

/// Descriptor for `NetworkInfo`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List networkInfoDescriptor = $convert.base64Decode(
    'CgtOZXR3b3JrSW5mbxISCgRzc2lkGAEgASgJUgRzc2lkEhoKCHBhc3N3b3JkGAIgASgJUghwYX'
    'Nzd29yZBIWCgZjcmVhdGUYAyABKAhSBmNyZWF0ZRIcCglwcmVmZXJyZWQYBCABKAhSCXByZWZl'
    'cnJlZBIYCgdrZWVwaW5nGAUgASgIUgdrZWVwaW5n');

@$core.Deprecated('Use networkSettingsDescriptor instead')
const NetworkSettings$json = {
  '1': 'NetworkSettings',
  '2': [
    {'1': 'createAccessPoint', '3': 1, '4': 1, '5': 5, '10': 'createAccessPoint'},
    {'1': 'connected', '3': 3, '4': 1, '5': 11, '6': '.fk_app.NetworkInfo', '10': 'connected'},
    {'1': 'macAddress', '3': 4, '4': 1, '5': 9, '10': 'macAddress'},
    {'1': 'modifying', '3': 5, '4': 1, '5': 8, '10': 'modifying'},
    {'1': 'supportsUdp', '3': 6, '4': 1, '5': 8, '10': 'supportsUdp'},
    {'1': 'networks', '3': 2, '4': 3, '5': 11, '6': '.fk_app.NetworkInfo', '10': 'networks'},
  ],
};

/// Descriptor for `NetworkSettings`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List networkSettingsDescriptor = $convert.base64Decode(
    'Cg9OZXR3b3JrU2V0dGluZ3MSLAoRY3JlYXRlQWNjZXNzUG9pbnQYASABKAVSEWNyZWF0ZUFjY2'
    'Vzc1BvaW50EjEKCWNvbm5lY3RlZBgDIAEoCzITLmZrX2FwcC5OZXR3b3JrSW5mb1IJY29ubmVj'
    'dGVkEh4KCm1hY0FkZHJlc3MYBCABKAlSCm1hY0FkZHJlc3MSHAoJbW9kaWZ5aW5nGAUgASgIUg'
    'ltb2RpZnlpbmcSIAoLc3VwcG9ydHNVZHAYBiABKAhSC3N1cHBvcnRzVWRwEi8KCG5ldHdvcmtz'
    'GAIgAygLMhMuZmtfYXBwLk5ldHdvcmtJbmZvUghuZXR3b3Jrcw==');

@$core.Deprecated('Use firmwareDescriptor instead')
const Firmware$json = {
  '1': 'Firmware',
  '2': [
    {'1': 'version', '3': 1, '4': 1, '5': 9, '10': 'version'},
    {'1': 'build', '3': 2, '4': 1, '5': 9, '10': 'build'},
    {'1': 'number', '3': 3, '4': 1, '5': 9, '10': 'number'},
    {'1': 'timestamp', '3': 4, '4': 1, '5': 4, '10': 'timestamp'},
    {'1': 'hash', '3': 5, '4': 1, '5': 9, '10': 'hash'},
    {'1': 'logical_address', '3': 6, '4': 1, '5': 4, '10': 'logicalAddress'},
    {'1': 'name', '3': 7, '4': 1, '5': 9, '10': 'name'},
  ],
};

/// Descriptor for `Firmware`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List firmwareDescriptor = $convert.base64Decode(
    'CghGaXJtd2FyZRIYCgd2ZXJzaW9uGAEgASgJUgd2ZXJzaW9uEhQKBWJ1aWxkGAIgASgJUgVidW'
    'lsZBIWCgZudW1iZXIYAyABKAlSBm51bWJlchIcCgl0aW1lc3RhbXAYBCABKARSCXRpbWVzdGFt'
    'cBISCgRoYXNoGAUgASgJUgRoYXNoEicKD2xvZ2ljYWxfYWRkcmVzcxgGIAEoBFIObG9naWNhbE'
    'FkZHJlc3MSEgoEbmFtZRgHIAEoCVIEbmFtZQ==');

@$core.Deprecated('Use identityDescriptor instead')
const Identity$json = {
  '1': 'Identity',
  '2': [
    {'1': 'device', '3': 1, '4': 1, '5': 9, '10': 'device'},
    {'1': 'stream', '3': 2, '4': 1, '5': 9, '10': 'stream'},
    {'1': 'deviceId', '3': 3, '4': 1, '5': 12, '10': 'deviceId'},
    {'1': 'firmware', '3': 4, '4': 1, '5': 9, '10': 'firmware'},
    {'1': 'build', '3': 5, '4': 1, '5': 9, '10': 'build'},
    {'1': 'number', '3': 8, '4': 1, '5': 9, '10': 'number'},
    {'1': 'name', '3': 6, '4': 1, '5': 9, '10': 'name'},
    {'1': 'generationId', '3': 7, '4': 1, '5': 12, '10': 'generationId'},
  ],
};

/// Descriptor for `Identity`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List identityDescriptor = $convert.base64Decode(
    'CghJZGVudGl0eRIWCgZkZXZpY2UYASABKAlSBmRldmljZRIWCgZzdHJlYW0YAiABKAlSBnN0cm'
    'VhbRIaCghkZXZpY2VJZBgDIAEoDFIIZGV2aWNlSWQSGgoIZmlybXdhcmUYBCABKAlSCGZpcm13'
    'YXJlEhQKBWJ1aWxkGAUgASgJUgVidWlsZBIWCgZudW1iZXIYCCABKAlSBm51bWJlchISCgRuYW'
    '1lGAYgASgJUgRuYW1lEiIKDGdlbmVyYXRpb25JZBgHIAEoDFIMZ2VuZXJhdGlvbklk');

@$core.Deprecated('Use configureSensorQueryDescriptor instead')
const ConfigureSensorQuery$json = {
  '1': 'ConfigureSensorQuery',
  '2': [
    {'1': 'id', '3': 1, '4': 1, '5': 13, '10': 'id'},
    {'1': 'frequency', '3': 2, '4': 1, '5': 13, '10': 'frequency'},
  ],
};

/// Descriptor for `ConfigureSensorQuery`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List configureSensorQueryDescriptor = $convert.base64Decode(
    'ChRDb25maWd1cmVTZW5zb3JRdWVyeRIOCgJpZBgBIAEoDVICaWQSHAoJZnJlcXVlbmN5GAIgAS'
    'gNUglmcmVxdWVuY3k=');

@$core.Deprecated('Use liveDataPollDescriptor instead')
const LiveDataPoll$json = {
  '1': 'LiveDataPoll',
  '2': [
    {'1': 'interval', '3': 1, '4': 1, '5': 13, '10': 'interval'},
  ],
};

/// Descriptor for `LiveDataPoll`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List liveDataPollDescriptor = $convert.base64Decode(
    'CgxMaXZlRGF0YVBvbGwSGgoIaW50ZXJ2YWwYASABKA1SCGludGVydmFs');

@$core.Deprecated('Use liveDataSampleDescriptor instead')
const LiveDataSample$json = {
  '1': 'LiveDataSample',
  '2': [
    {'1': 'sensor', '3': 1, '4': 1, '5': 13, '10': 'sensor'},
    {'1': 'time', '3': 2, '4': 1, '5': 4, '10': 'time'},
    {'1': 'value', '3': 3, '4': 1, '5': 2, '10': 'value'},
  ],
};

/// Descriptor for `LiveDataSample`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List liveDataSampleDescriptor = $convert.base64Decode(
    'Cg5MaXZlRGF0YVNhbXBsZRIWCgZzZW5zb3IYASABKA1SBnNlbnNvchISCgR0aW1lGAIgASgEUg'
    'R0aW1lEhQKBXZhbHVlGAMgASgCUgV2YWx1ZQ==');

@$core.Deprecated('Use liveDataDescriptor instead')
const LiveData$json = {
  '1': 'LiveData',
  '2': [
    {'1': 'samples', '3': 1, '4': 3, '5': 11, '6': '.fk_app.LiveDataSample', '10': 'samples'},
  ],
};

/// Descriptor for `LiveData`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List liveDataDescriptor = $convert.base64Decode(
    'CghMaXZlRGF0YRIwCgdzYW1wbGVzGAEgAygLMhYuZmtfYXBwLkxpdmVEYXRhU2FtcGxlUgdzYW'
    '1wbGVz');

@$core.Deprecated('Use fileDescriptor instead')
const File$json = {
  '1': 'File',
  '2': [
    {'1': 'id', '3': 1, '4': 1, '5': 13, '10': 'id'},
    {'1': 'time', '3': 2, '4': 1, '5': 4, '10': 'time'},
    {'1': 'size', '3': 3, '4': 1, '5': 4, '10': 'size'},
    {'1': 'version', '3': 4, '4': 1, '5': 13, '10': 'version'},
    {'1': 'name', '3': 5, '4': 1, '5': 9, '10': 'name'},
    {'1': 'maximum', '3': 6, '4': 1, '5': 4, '10': 'maximum'},
  ],
};

/// Descriptor for `File`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List fileDescriptor = $convert.base64Decode(
    'CgRGaWxlEg4KAmlkGAEgASgNUgJpZBISCgR0aW1lGAIgASgEUgR0aW1lEhIKBHNpemUYAyABKA'
    'RSBHNpemUSGAoHdmVyc2lvbhgEIAEoDVIHdmVyc2lvbhISCgRuYW1lGAUgASgJUgRuYW1lEhgK'
    'B21heGltdW0YBiABKARSB21heGltdW0=');

@$core.Deprecated('Use filesDescriptor instead')
const Files$json = {
  '1': 'Files',
  '2': [
    {'1': 'files', '3': 1, '4': 3, '5': 11, '6': '.fk_app.File', '10': 'files'},
  ],
};

/// Descriptor for `Files`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List filesDescriptor = $convert.base64Decode(
    'CgVGaWxlcxIiCgVmaWxlcxgBIAMoCzIMLmZrX2FwcC5GaWxlUgVmaWxlcw==');

@$core.Deprecated('Use downloadFileDescriptor instead')
const DownloadFile$json = {
  '1': 'DownloadFile',
  '2': [
    {'1': 'id', '3': 1, '4': 1, '5': 13, '10': 'id'},
    {'1': 'offset', '3': 2, '4': 1, '5': 13, '10': 'offset'},
    {'1': 'length', '3': 3, '4': 1, '5': 13, '10': 'length'},
    {'1': 'flags', '3': 4, '4': 1, '5': 13, '10': 'flags'},
  ],
};

/// Descriptor for `DownloadFile`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List downloadFileDescriptor = $convert.base64Decode(
    'CgxEb3dubG9hZEZpbGUSDgoCaWQYASABKA1SAmlkEhYKBm9mZnNldBgCIAEoDVIGb2Zmc2V0Eh'
    'YKBmxlbmd0aBgDIAEoDVIGbGVuZ3RoEhQKBWZsYWdzGAQgASgNUgVmbGFncw==');

@$core.Deprecated('Use eraseFileDescriptor instead')
const EraseFile$json = {
  '1': 'EraseFile',
  '2': [
    {'1': 'id', '3': 1, '4': 1, '5': 13, '10': 'id'},
  ],
};

/// Descriptor for `EraseFile`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List eraseFileDescriptor = $convert.base64Decode(
    'CglFcmFzZUZpbGUSDgoCaWQYASABKA1SAmlk');

@$core.Deprecated('Use fileDataDescriptor instead')
const FileData$json = {
  '1': 'FileData',
  '2': [
    {'1': 'offset', '3': 1, '4': 1, '5': 13, '10': 'offset'},
    {'1': 'data', '3': 2, '4': 1, '5': 12, '10': 'data'},
    {'1': 'size', '3': 3, '4': 1, '5': 13, '10': 'size'},
    {'1': 'hash', '3': 4, '4': 1, '5': 13, '10': 'hash'},
    {'1': 'version', '3': 5, '4': 1, '5': 13, '10': 'version'},
    {'1': 'id', '3': 6, '4': 1, '5': 13, '10': 'id'},
  ],
};

/// Descriptor for `FileData`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List fileDataDescriptor = $convert.base64Decode(
    'CghGaWxlRGF0YRIWCgZvZmZzZXQYASABKA1SBm9mZnNldBISCgRkYXRhGAIgASgMUgRkYXRhEh'
    'IKBHNpemUYAyABKA1SBHNpemUSEgoEaGFzaBgEIAEoDVIEaGFzaBIYCgd2ZXJzaW9uGAUgASgN'
    'Ugd2ZXJzaW9uEg4KAmlkGAYgASgNUgJpZA==');

@$core.Deprecated('Use deviceStatusDescriptor instead')
const DeviceStatus$json = {
  '1': 'DeviceStatus',
  '2': [
    {'1': 'uptime', '3': 1, '4': 1, '5': 13, '10': 'uptime'},
    {'1': 'batteryPercentage', '3': 2, '4': 1, '5': 2, '10': 'batteryPercentage'},
    {'1': 'batteryVoltage', '3': 3, '4': 1, '5': 2, '10': 'batteryVoltage'},
    {'1': 'gpsHasFix', '3': 4, '4': 1, '5': 13, '10': 'gpsHasFix'},
    {'1': 'gpsSatellites', '3': 5, '4': 1, '5': 13, '10': 'gpsSatellites'},
  ],
};

/// Descriptor for `DeviceStatus`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List deviceStatusDescriptor = $convert.base64Decode(
    'CgxEZXZpY2VTdGF0dXMSFgoGdXB0aW1lGAEgASgNUgZ1cHRpbWUSLAoRYmF0dGVyeVBlcmNlbn'
    'RhZ2UYAiABKAJSEWJhdHRlcnlQZXJjZW50YWdlEiYKDmJhdHRlcnlWb2x0YWdlGAMgASgCUg5i'
    'YXR0ZXJ5Vm9sdGFnZRIcCglncHNIYXNGaXgYBCABKA1SCWdwc0hhc0ZpeBIkCg1ncHNTYXRlbG'
    'xpdGVzGAUgASgNUg1ncHNTYXRlbGxpdGVz');

@$core.Deprecated('Use queryModuleDescriptor instead')
const QueryModule$json = {
  '1': 'QueryModule',
  '2': [
    {'1': 'id', '3': 1, '4': 1, '5': 13, '10': 'id'},
    {'1': 'address', '3': 2, '4': 1, '5': 13, '10': 'address'},
    {'1': 'message', '3': 3, '4': 1, '5': 12, '10': 'message'},
  ],
};

/// Descriptor for `QueryModule`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List queryModuleDescriptor = $convert.base64Decode(
    'CgtRdWVyeU1vZHVsZRIOCgJpZBgBIAEoDVICaWQSGAoHYWRkcmVzcxgCIAEoDVIHYWRkcmVzcx'
    'IYCgdtZXNzYWdlGAMgASgMUgdtZXNzYWdl');

@$core.Deprecated('Use moduleReplyDescriptor instead')
const ModuleReply$json = {
  '1': 'ModuleReply',
  '2': [
    {'1': 'id', '3': 1, '4': 1, '5': 13, '10': 'id'},
    {'1': 'address', '3': 2, '4': 1, '5': 13, '10': 'address'},
    {'1': 'message', '3': 3, '4': 1, '5': 12, '10': 'message'},
  ],
};

/// Descriptor for `ModuleReply`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List moduleReplyDescriptor = $convert.base64Decode(
    'CgtNb2R1bGVSZXBseRIOCgJpZBgBIAEoDVICaWQSGAoHYWRkcmVzcxgCIAEoDVIHYWRkcmVzcx'
    'IYCgdtZXNzYWdlGAMgASgMUgdtZXNzYWdl');

@$core.Deprecated('Use wireMessageQueryDescriptor instead')
const WireMessageQuery$json = {
  '1': 'WireMessageQuery',
  '2': [
    {'1': 'type', '3': 1, '4': 1, '5': 14, '6': '.fk_app.QueryType', '10': 'type'},
    {'1': 'queryCapabilities', '3': 2, '4': 1, '5': 11, '6': '.fk_app.QueryCapabilities', '10': 'queryCapabilities'},
    {'1': 'configureSensor', '3': 3, '4': 1, '5': 11, '6': '.fk_app.ConfigureSensorQuery', '10': 'configureSensor'},
    {'1': 'liveDataPoll', '3': 8, '4': 1, '5': 11, '6': '.fk_app.LiveDataPoll', '10': 'liveDataPoll'},
    {'1': 'downloadFile', '3': 10, '4': 1, '5': 11, '6': '.fk_app.DownloadFile', '10': 'downloadFile'},
    {'1': 'eraseFile', '3': 11, '4': 1, '5': 11, '6': '.fk_app.EraseFile', '10': 'eraseFile'},
    {'1': 'networkSettings', '3': 12, '4': 1, '5': 11, '6': '.fk_app.NetworkSettings', '10': 'networkSettings'},
    {'1': 'identity', '3': 13, '4': 1, '5': 11, '6': '.fk_app.Identity', '10': 'identity'},
    {'1': 'module', '3': 14, '4': 1, '5': 11, '6': '.fk_app.QueryModule', '10': 'module'},
  ],
};

/// Descriptor for `WireMessageQuery`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List wireMessageQueryDescriptor = $convert.base64Decode(
    'ChBXaXJlTWVzc2FnZVF1ZXJ5EiUKBHR5cGUYASABKA4yES5ma19hcHAuUXVlcnlUeXBlUgR0eX'
    'BlEkcKEXF1ZXJ5Q2FwYWJpbGl0aWVzGAIgASgLMhkuZmtfYXBwLlF1ZXJ5Q2FwYWJpbGl0aWVz'
    'UhFxdWVyeUNhcGFiaWxpdGllcxJGCg9jb25maWd1cmVTZW5zb3IYAyABKAsyHC5ma19hcHAuQ2'
    '9uZmlndXJlU2Vuc29yUXVlcnlSD2NvbmZpZ3VyZVNlbnNvchI4CgxsaXZlRGF0YVBvbGwYCCAB'
    'KAsyFC5ma19hcHAuTGl2ZURhdGFQb2xsUgxsaXZlRGF0YVBvbGwSOAoMZG93bmxvYWRGaWxlGA'
    'ogASgLMhQuZmtfYXBwLkRvd25sb2FkRmlsZVIMZG93bmxvYWRGaWxlEi8KCWVyYXNlRmlsZRgL'
    'IAEoCzIRLmZrX2FwcC5FcmFzZUZpbGVSCWVyYXNlRmlsZRJBCg9uZXR3b3JrU2V0dGluZ3MYDC'
    'ABKAsyFy5ma19hcHAuTmV0d29ya1NldHRpbmdzUg9uZXR3b3JrU2V0dGluZ3MSLAoIaWRlbnRp'
    'dHkYDSABKAsyEC5ma19hcHAuSWRlbnRpdHlSCGlkZW50aXR5EisKBm1vZHVsZRgOIAEoCzITLm'
    'ZrX2FwcC5RdWVyeU1vZHVsZVIGbW9kdWxl');

@$core.Deprecated('Use errorDescriptor instead')
const Error$json = {
  '1': 'Error',
  '2': [
    {'1': 'message', '3': 1, '4': 1, '5': 9, '10': 'message'},
    {'1': 'delay', '3': 2, '4': 1, '5': 13, '10': 'delay'},
  ],
};

/// Descriptor for `Error`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List errorDescriptor = $convert.base64Decode(
    'CgVFcnJvchIYCgdtZXNzYWdlGAEgASgJUgdtZXNzYWdlEhQKBWRlbGF5GAIgASgNUgVkZWxheQ'
    '==');

@$core.Deprecated('Use wireMessageReplyDescriptor instead')
const WireMessageReply$json = {
  '1': 'WireMessageReply',
  '2': [
    {'1': 'type', '3': 1, '4': 1, '5': 14, '6': '.fk_app.ReplyType', '10': 'type'},
    {'1': 'errors', '3': 2, '4': 3, '5': 11, '6': '.fk_app.Error', '10': 'errors'},
    {'1': 'capabilities', '3': 3, '4': 1, '5': 11, '6': '.fk_app.Capabilities', '10': 'capabilities'},
    {'1': 'liveData', '3': 6, '4': 1, '5': 11, '6': '.fk_app.LiveData', '10': 'liveData'},
    {'1': 'files', '3': 8, '4': 1, '5': 11, '6': '.fk_app.Files', '10': 'files'},
    {'1': 'fileData', '3': 9, '4': 1, '5': 11, '6': '.fk_app.FileData', '10': 'fileData'},
    {'1': 'networkSettings', '3': 10, '4': 1, '5': 11, '6': '.fk_app.NetworkSettings', '10': 'networkSettings'},
    {'1': 'identity', '3': 11, '4': 1, '5': 11, '6': '.fk_app.Identity', '10': 'identity'},
    {'1': 'status', '3': 12, '4': 1, '5': 11, '6': '.fk_app.DeviceStatus', '10': 'status'},
    {'1': 'module', '3': 13, '4': 1, '5': 11, '6': '.fk_app.ModuleReply', '10': 'module'},
  ],
};

/// Descriptor for `WireMessageReply`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List wireMessageReplyDescriptor = $convert.base64Decode(
    'ChBXaXJlTWVzc2FnZVJlcGx5EiUKBHR5cGUYASABKA4yES5ma19hcHAuUmVwbHlUeXBlUgR0eX'
    'BlEiUKBmVycm9ycxgCIAMoCzINLmZrX2FwcC5FcnJvclIGZXJyb3JzEjgKDGNhcGFiaWxpdGll'
    'cxgDIAEoCzIULmZrX2FwcC5DYXBhYmlsaXRpZXNSDGNhcGFiaWxpdGllcxIsCghsaXZlRGF0YR'
    'gGIAEoCzIQLmZrX2FwcC5MaXZlRGF0YVIIbGl2ZURhdGESIwoFZmlsZXMYCCABKAsyDS5ma19h'
    'cHAuRmlsZXNSBWZpbGVzEiwKCGZpbGVEYXRhGAkgASgLMhAuZmtfYXBwLkZpbGVEYXRhUghmaW'
    'xlRGF0YRJBCg9uZXR3b3JrU2V0dGluZ3MYCiABKAsyFy5ma19hcHAuTmV0d29ya1NldHRpbmdz'
    'Ug9uZXR3b3JrU2V0dGluZ3MSLAoIaWRlbnRpdHkYCyABKAsyEC5ma19hcHAuSWRlbnRpdHlSCG'
    'lkZW50aXR5EiwKBnN0YXR1cxgMIAEoCzIULmZrX2FwcC5EZXZpY2VTdGF0dXNSBnN0YXR1cxIr'
    'CgZtb2R1bGUYDSABKAsyEy5ma19hcHAuTW9kdWxlUmVwbHlSBm1vZHVsZQ==');

@$core.Deprecated('Use intervalDescriptor instead')
const Interval$json = {
  '1': 'Interval',
  '2': [
    {'1': 'start', '3': 1, '4': 1, '5': 4, '10': 'start'},
    {'1': 'end', '3': 2, '4': 1, '5': 4, '10': 'end'},
    {'1': 'interval', '3': 3, '4': 1, '5': 13, '10': 'interval'},
  ],
};

/// Descriptor for `Interval`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List intervalDescriptor = $convert.base64Decode(
    'CghJbnRlcnZhbBIUCgVzdGFydBgBIAEoBFIFc3RhcnQSEAoDZW5kGAIgASgEUgNlbmQSGgoIaW'
    '50ZXJ2YWwYAyABKA1SCGludGVydmFs');

@$core.Deprecated('Use scheduleDescriptor instead')
const Schedule$json = {
  '1': 'Schedule',
  '2': [
    {'1': 'cron', '3': 1, '4': 1, '5': 12, '10': 'cron'},
    {'1': 'interval', '3': 2, '4': 1, '5': 13, '10': 'interval'},
    {'1': 'repeated', '3': 3, '4': 1, '5': 13, '10': 'repeated'},
    {'1': 'duration', '3': 4, '4': 1, '5': 13, '10': 'duration'},
    {'1': 'jitter', '3': 5, '4': 1, '5': 13, '10': 'jitter'},
    {'1': 'intervals', '3': 6, '4': 3, '5': 11, '6': '.fk_app.Interval', '10': 'intervals'},
  ],
};

/// Descriptor for `Schedule`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List scheduleDescriptor = $convert.base64Decode(
    'CghTY2hlZHVsZRISCgRjcm9uGAEgASgMUgRjcm9uEhoKCGludGVydmFsGAIgASgNUghpbnRlcn'
    'ZhbBIaCghyZXBlYXRlZBgDIAEoDVIIcmVwZWF0ZWQSGgoIZHVyYXRpb24YBCABKA1SCGR1cmF0'
    'aW9uEhYKBmppdHRlchgFIAEoDVIGaml0dGVyEi4KCWludGVydmFscxgGIAMoCzIQLmZrX2FwcC'
    '5JbnRlcnZhbFIJaW50ZXJ2YWxz');

@$core.Deprecated('Use schedulesDescriptor instead')
const Schedules$json = {
  '1': 'Schedules',
  '2': [
    {'1': 'modifying', '3': 1, '4': 1, '5': 8, '10': 'modifying'},
    {'1': 'readings', '3': 2, '4': 1, '5': 11, '6': '.fk_app.Schedule', '10': 'readings'},
    {'1': 'lora', '3': 3, '4': 1, '5': 11, '6': '.fk_app.Schedule', '10': 'lora'},
    {'1': 'network', '3': 4, '4': 1, '5': 11, '6': '.fk_app.Schedule', '10': 'network'},
    {'1': 'gps', '3': 5, '4': 1, '5': 11, '6': '.fk_app.Schedule', '10': 'gps'},
  ],
};

/// Descriptor for `Schedules`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List schedulesDescriptor = $convert.base64Decode(
    'CglTY2hlZHVsZXMSHAoJbW9kaWZ5aW5nGAEgASgIUgltb2RpZnlpbmcSLAoIcmVhZGluZ3MYAi'
    'ABKAsyEC5ma19hcHAuU2NoZWR1bGVSCHJlYWRpbmdzEiQKBGxvcmEYAyABKAsyEC5ma19hcHAu'
    'U2NoZWR1bGVSBGxvcmESKgoHbmV0d29yaxgEIAEoCzIQLmZrX2FwcC5TY2hlZHVsZVIHbmV0d2'
    '9yaxIiCgNncHMYBSABKAsyEC5ma19hcHAuU2NoZWR1bGVSA2dwcw==');

@$core.Deprecated('Use hardwareStatusDescriptor instead')
const HardwareStatus$json = {
  '1': 'HardwareStatus',
};

/// Descriptor for `HardwareStatus`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List hardwareStatusDescriptor = $convert.base64Decode(
    'Cg5IYXJkd2FyZVN0YXR1cw==');

@$core.Deprecated('Use gpsStatusDescriptor instead')
const GpsStatus$json = {
  '1': 'GpsStatus',
  '2': [
    {'1': 'enabled', '3': 7, '4': 1, '5': 13, '10': 'enabled'},
    {'1': 'fix', '3': 1, '4': 1, '5': 13, '10': 'fix'},
    {'1': 'time', '3': 2, '4': 1, '5': 4, '10': 'time'},
    {'1': 'satellites', '3': 3, '4': 1, '5': 13, '10': 'satellites'},
    {'1': 'longitude', '3': 4, '4': 1, '5': 2, '10': 'longitude'},
    {'1': 'latitude', '3': 5, '4': 1, '5': 2, '10': 'latitude'},
    {'1': 'altitude', '3': 6, '4': 1, '5': 2, '10': 'altitude'},
  ],
};

/// Descriptor for `GpsStatus`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List gpsStatusDescriptor = $convert.base64Decode(
    'CglHcHNTdGF0dXMSGAoHZW5hYmxlZBgHIAEoDVIHZW5hYmxlZBIQCgNmaXgYASABKA1SA2ZpeB'
    'ISCgR0aW1lGAIgASgEUgR0aW1lEh4KCnNhdGVsbGl0ZXMYAyABKA1SCnNhdGVsbGl0ZXMSHAoJ'
    'bG9uZ2l0dWRlGAQgASgCUglsb25naXR1ZGUSGgoIbGF0aXR1ZGUYBSABKAJSCGxhdGl0dWRlEh'
    'oKCGFsdGl0dWRlGAYgASgCUghhbHRpdHVkZQ==');

@$core.Deprecated('Use memoryStatusDescriptor instead')
const MemoryStatus$json = {
  '1': 'MemoryStatus',
  '2': [
    {'1': 'sramAvailable', '3': 1, '4': 1, '5': 13, '10': 'sramAvailable'},
    {'1': 'programFlashAvailable', '3': 2, '4': 1, '5': 13, '10': 'programFlashAvailable'},
    {'1': 'extendedMemoryAvailable', '3': 3, '4': 1, '5': 13, '10': 'extendedMemoryAvailable'},
    {'1': 'dataMemoryInstalled', '3': 4, '4': 1, '5': 13, '10': 'dataMemoryInstalled'},
    {'1': 'dataMemoryUsed', '3': 5, '4': 1, '5': 13, '10': 'dataMemoryUsed'},
    {'1': 'dataMemoryConsumption', '3': 6, '4': 1, '5': 2, '10': 'dataMemoryConsumption'},
    {'1': 'firmware', '3': 7, '4': 3, '5': 11, '6': '.fk_app.Firmware', '10': 'firmware'},
  ],
};

/// Descriptor for `MemoryStatus`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List memoryStatusDescriptor = $convert.base64Decode(
    'CgxNZW1vcnlTdGF0dXMSJAoNc3JhbUF2YWlsYWJsZRgBIAEoDVINc3JhbUF2YWlsYWJsZRI0Ch'
    'Vwcm9ncmFtRmxhc2hBdmFpbGFibGUYAiABKA1SFXByb2dyYW1GbGFzaEF2YWlsYWJsZRI4Chdl'
    'eHRlbmRlZE1lbW9yeUF2YWlsYWJsZRgDIAEoDVIXZXh0ZW5kZWRNZW1vcnlBdmFpbGFibGUSMA'
    'oTZGF0YU1lbW9yeUluc3RhbGxlZBgEIAEoDVITZGF0YU1lbW9yeUluc3RhbGxlZBImCg5kYXRh'
    'TWVtb3J5VXNlZBgFIAEoDVIOZGF0YU1lbW9yeVVzZWQSNAoVZGF0YU1lbW9yeUNvbnN1bXB0aW'
    '9uGAYgASgCUhVkYXRhTWVtb3J5Q29uc3VtcHRpb24SLAoIZmlybXdhcmUYByADKAsyEC5ma19h'
    'cHAuRmlybXdhcmVSCGZpcm13YXJl');

@$core.Deprecated('Use batteryStatusDescriptor instead')
const BatteryStatus$json = {
  '1': 'BatteryStatus',
  '2': [
    {'1': 'voltage', '3': 1, '4': 1, '5': 13, '10': 'voltage'},
    {'1': 'percentage', '3': 2, '4': 1, '5': 13, '10': 'percentage'},
  ],
};

/// Descriptor for `BatteryStatus`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List batteryStatusDescriptor = $convert.base64Decode(
    'Cg1CYXR0ZXJ5U3RhdHVzEhgKB3ZvbHRhZ2UYASABKA1SB3ZvbHRhZ2USHgoKcGVyY2VudGFnZR'
    'gCIAEoDVIKcGVyY2VudGFnZQ==');

@$core.Deprecated('Use solarStatusDescriptor instead')
const SolarStatus$json = {
  '1': 'SolarStatus',
  '2': [
    {'1': 'voltage', '3': 1, '4': 1, '5': 13, '10': 'voltage'},
  ],
};

/// Descriptor for `SolarStatus`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List solarStatusDescriptor = $convert.base64Decode(
    'CgtTb2xhclN0YXR1cxIYCgd2b2x0YWdlGAEgASgNUgd2b2x0YWdl');

@$core.Deprecated('Use powerStatusDescriptor instead')
const PowerStatus$json = {
  '1': 'PowerStatus',
  '2': [
    {'1': 'battery', '3': 1, '4': 1, '5': 11, '6': '.fk_app.BatteryStatus', '10': 'battery'},
    {'1': 'solar', '3': 2, '4': 1, '5': 11, '6': '.fk_app.SolarStatus', '10': 'solar'},
  ],
};

/// Descriptor for `PowerStatus`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List powerStatusDescriptor = $convert.base64Decode(
    'CgtQb3dlclN0YXR1cxIvCgdiYXR0ZXJ5GAEgASgLMhUuZmtfYXBwLkJhdHRlcnlTdGF0dXNSB2'
    'JhdHRlcnkSKQoFc29sYXIYAiABKAsyEy5ma19hcHAuU29sYXJTdGF0dXNSBXNvbGFy');

@$core.Deprecated('Use statusDescriptor instead')
const Status$json = {
  '1': 'Status',
  '2': [
    {'1': 'version', '3': 1, '4': 1, '5': 13, '10': 'version'},
    {'1': 'uptime', '3': 2, '4': 1, '5': 13, '10': 'uptime'},
    {'1': 'identity', '3': 3, '4': 1, '5': 11, '6': '.fk_app.Identity', '10': 'identity'},
    {'1': 'hardware', '3': 4, '4': 1, '5': 11, '6': '.fk_app.HardwareStatus', '10': 'hardware'},
    {'1': 'power', '3': 5, '4': 1, '5': 11, '6': '.fk_app.PowerStatus', '10': 'power'},
    {'1': 'memory', '3': 6, '4': 1, '5': 11, '6': '.fk_app.MemoryStatus', '10': 'memory'},
    {'1': 'gps', '3': 7, '4': 1, '5': 11, '6': '.fk_app.GpsStatus', '10': 'gps'},
    {'1': 'schedules', '3': 8, '4': 1, '5': 11, '6': '.fk_app.Schedules', '10': 'schedules'},
    {'1': 'recording', '3': 9, '4': 1, '5': 11, '6': '.fk_app.Recording', '10': 'recording'},
    {'1': 'network', '3': 10, '4': 1, '5': 11, '6': '.fk_app.NetworkSettings', '10': 'network'},
    {'1': 'time', '3': 11, '4': 1, '5': 4, '10': 'time'},
    {'1': 'firmware', '3': 12, '4': 1, '5': 11, '6': '.fk_app.Firmware', '10': 'firmware'},
    {'1': 'logs', '3': 13, '4': 1, '5': 9, '10': 'logs'},
  ],
};

/// Descriptor for `Status`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List statusDescriptor = $convert.base64Decode(
    'CgZTdGF0dXMSGAoHdmVyc2lvbhgBIAEoDVIHdmVyc2lvbhIWCgZ1cHRpbWUYAiABKA1SBnVwdG'
    'ltZRIsCghpZGVudGl0eRgDIAEoCzIQLmZrX2FwcC5JZGVudGl0eVIIaWRlbnRpdHkSMgoIaGFy'
    'ZHdhcmUYBCABKAsyFi5ma19hcHAuSGFyZHdhcmVTdGF0dXNSCGhhcmR3YXJlEikKBXBvd2VyGA'
    'UgASgLMhMuZmtfYXBwLlBvd2VyU3RhdHVzUgVwb3dlchIsCgZtZW1vcnkYBiABKAsyFC5ma19h'
    'cHAuTWVtb3J5U3RhdHVzUgZtZW1vcnkSIwoDZ3BzGAcgASgLMhEuZmtfYXBwLkdwc1N0YXR1c1'
    'IDZ3BzEi8KCXNjaGVkdWxlcxgIIAEoCzIRLmZrX2FwcC5TY2hlZHVsZXNSCXNjaGVkdWxlcxIv'
    'CglyZWNvcmRpbmcYCSABKAsyES5ma19hcHAuUmVjb3JkaW5nUglyZWNvcmRpbmcSMQoHbmV0d2'
    '9yaxgKIAEoCzIXLmZrX2FwcC5OZXR3b3JrU2V0dGluZ3NSB25ldHdvcmsSEgoEdGltZRgLIAEo'
    'BFIEdGltZRIsCghmaXJtd2FyZRgMIAEoCzIQLmZrX2FwcC5GaXJtd2FyZVIIZmlybXdhcmUSEg'
    'oEbG9ncxgNIAEoCVIEbG9ncw==');

@$core.Deprecated('Use rangeDescriptor instead')
const Range$json = {
  '1': 'Range',
  '2': [
    {'1': 'start', '3': 1, '4': 1, '5': 13, '10': 'start'},
    {'1': 'end', '3': 2, '4': 1, '5': 13, '10': 'end'},
  ],
};

/// Descriptor for `Range`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List rangeDescriptor = $convert.base64Decode(
    'CgVSYW5nZRIUCgVzdGFydBgBIAEoDVIFc3RhcnQSEAoDZW5kGAIgASgNUgNlbmQ=');

@$core.Deprecated('Use downloadQueryDescriptor instead')
const DownloadQuery$json = {
  '1': 'DownloadQuery',
  '2': [
    {'1': 'stream', '3': 1, '4': 1, '5': 13, '10': 'stream'},
    {'1': 'ranges', '3': 3, '4': 3, '5': 11, '6': '.fk_app.Range', '10': 'ranges'},
    {'1': 'blocks', '3': 4, '4': 3, '5': 13, '10': 'blocks'},
  ],
};

/// Descriptor for `DownloadQuery`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List downloadQueryDescriptor = $convert.base64Decode(
    'Cg1Eb3dubG9hZFF1ZXJ5EhYKBnN0cmVhbRgBIAEoDVIGc3RyZWFtEiUKBnJhbmdlcxgDIAMoCz'
    'INLmZrX2FwcC5SYW5nZVIGcmFuZ2VzEhYKBmJsb2NrcxgEIAMoDVIGYmxvY2tz');

@$core.Deprecated('Use recordingDescriptor instead')
const Recording$json = {
  '1': 'Recording',
  '2': [
    {'1': 'modifying', '3': 1, '4': 1, '5': 8, '10': 'modifying'},
    {'1': 'enabled', '3': 2, '4': 1, '5': 8, '10': 'enabled'},
    {'1': 'startedTime', '3': 3, '4': 1, '5': 4, '10': 'startedTime'},
    {'1': 'location', '3': 4, '4': 1, '5': 11, '6': '.fk_app.Location', '10': 'location'},
  ],
};

/// Descriptor for `Recording`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List recordingDescriptor = $convert.base64Decode(
    'CglSZWNvcmRpbmcSHAoJbW9kaWZ5aW5nGAEgASgIUgltb2RpZnlpbmcSGAoHZW5hYmxlZBgCIA'
    'EoCFIHZW5hYmxlZBIgCgtzdGFydGVkVGltZRgDIAEoBFILc3RhcnRlZFRpbWUSLAoIbG9jYXRp'
    'b24YBCABKAsyEC5ma19hcHAuTG9jYXRpb25SCGxvY2F0aW9u');

@$core.Deprecated('Use loraSettingsDescriptor instead')
const LoraSettings$json = {
  '1': 'LoraSettings',
  '2': [
    {'1': 'available', '3': 1, '4': 1, '5': 8, '10': 'available'},
    {'1': 'modifying', '3': 2, '4': 1, '5': 8, '10': 'modifying'},
    {'1': 'clearing', '3': 3, '4': 1, '5': 8, '10': 'clearing'},
    {'1': 'frequencyBand', '3': 4, '4': 1, '5': 13, '10': 'frequencyBand'},
    {'1': 'deviceEui', '3': 5, '4': 1, '5': 12, '10': 'deviceEui'},
    {'1': 'appKey', '3': 6, '4': 1, '5': 12, '10': 'appKey'},
    {'1': 'joinEui', '3': 7, '4': 1, '5': 12, '10': 'joinEui'},
    {'1': 'deviceAddress', '3': 8, '4': 1, '5': 12, '10': 'deviceAddress'},
    {'1': 'networkSessionKey', '3': 9, '4': 1, '5': 12, '10': 'networkSessionKey'},
    {'1': 'appSessionKey', '3': 10, '4': 1, '5': 12, '10': 'appSessionKey'},
  ],
};

/// Descriptor for `LoraSettings`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List loraSettingsDescriptor = $convert.base64Decode(
    'CgxMb3JhU2V0dGluZ3MSHAoJYXZhaWxhYmxlGAEgASgIUglhdmFpbGFibGUSHAoJbW9kaWZ5aW'
    '5nGAIgASgIUgltb2RpZnlpbmcSGgoIY2xlYXJpbmcYAyABKAhSCGNsZWFyaW5nEiQKDWZyZXF1'
    'ZW5jeUJhbmQYBCABKA1SDWZyZXF1ZW5jeUJhbmQSHAoJZGV2aWNlRXVpGAUgASgMUglkZXZpY2'
    'VFdWkSFgoGYXBwS2V5GAYgASgMUgZhcHBLZXkSGAoHam9pbkV1aRgHIAEoDFIHam9pbkV1aRIk'
    'Cg1kZXZpY2VBZGRyZXNzGAggASgMUg1kZXZpY2VBZGRyZXNzEiwKEW5ldHdvcmtTZXNzaW9uS2'
    'V5GAkgASgMUhFuZXR3b3JrU2Vzc2lvbktleRIkCg1hcHBTZXNzaW9uS2V5GAogASgMUg1hcHBT'
    'ZXNzaW9uS2V5');

@$core.Deprecated('Use locationDescriptor instead')
const Location$json = {
  '1': 'Location',
  '2': [
    {'1': 'modifying', '3': 1, '4': 1, '5': 8, '10': 'modifying'},
    {'1': 'longitude', '3': 2, '4': 1, '5': 2, '10': 'longitude'},
    {'1': 'latitude', '3': 3, '4': 1, '5': 2, '10': 'latitude'},
    {'1': 'time', '3': 4, '4': 1, '5': 4, '10': 'time'},
  ],
};

/// Descriptor for `Location`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List locationDescriptor = $convert.base64Decode(
    'CghMb2NhdGlvbhIcCgltb2RpZnlpbmcYASABKAhSCW1vZGlmeWluZxIcCglsb25naXR1ZGUYAi'
    'ABKAJSCWxvbmdpdHVkZRIaCghsYXRpdHVkZRgDIAEoAlIIbGF0aXR1ZGUSEgoEdGltZRgEIAEo'
    'BFIEdGltZQ==');

@$core.Deprecated('Use wifiTransmissionDescriptor instead')
const WifiTransmission$json = {
  '1': 'WifiTransmission',
  '2': [
    {'1': 'modifying', '3': 1, '4': 1, '5': 8, '10': 'modifying'},
    {'1': 'url', '3': 2, '4': 1, '5': 9, '10': 'url'},
    {'1': 'token', '3': 3, '4': 1, '5': 9, '10': 'token'},
    {'1': 'enabled', '3': 4, '4': 1, '5': 8, '10': 'enabled'},
  ],
};

/// Descriptor for `WifiTransmission`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List wifiTransmissionDescriptor = $convert.base64Decode(
    'ChBXaWZpVHJhbnNtaXNzaW9uEhwKCW1vZGlmeWluZxgBIAEoCFIJbW9kaWZ5aW5nEhAKA3VybB'
    'gCIAEoCVIDdXJsEhQKBXRva2VuGAMgASgJUgV0b2tlbhIYCgdlbmFibGVkGAQgASgIUgdlbmFi'
    'bGVk');

@$core.Deprecated('Use transmissionDescriptor instead')
const Transmission$json = {
  '1': 'Transmission',
  '2': [
    {'1': 'wifi', '3': 1, '4': 1, '5': 11, '6': '.fk_app.WifiTransmission', '10': 'wifi'},
  ],
};

/// Descriptor for `Transmission`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transmissionDescriptor = $convert.base64Decode(
    'CgxUcmFuc21pc3Npb24SLAoEd2lmaRgBIAEoCzIYLmZrX2FwcC5XaWZpVHJhbnNtaXNzaW9uUg'
    'R3aWZp');

@$core.Deprecated('Use listDirectoryDescriptor instead')
const ListDirectory$json = {
  '1': 'ListDirectory',
  '2': [
    {'1': 'path', '3': 1, '4': 1, '5': 9, '10': 'path'},
    {'1': 'page', '3': 2, '4': 1, '5': 13, '10': 'page'},
  ],
};

/// Descriptor for `ListDirectory`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List listDirectoryDescriptor = $convert.base64Decode(
    'Cg1MaXN0RGlyZWN0b3J5EhIKBHBhdGgYASABKAlSBHBhdGgSEgoEcGFnZRgCIAEoDVIEcGFnZQ'
    '==');

@$core.Deprecated('Use httpQueryDescriptor instead')
const HttpQuery$json = {
  '1': 'HttpQuery',
  '2': [
    {'1': 'type', '3': 1, '4': 1, '5': 14, '6': '.fk_app.QueryType', '10': 'type'},
    {'1': 'identity', '3': 2, '4': 1, '5': 11, '6': '.fk_app.Identity', '10': 'identity'},
    {'1': 'recording', '3': 3, '4': 1, '5': 11, '6': '.fk_app.Recording', '10': 'recording'},
    {'1': 'schedules', '3': 4, '4': 1, '5': 11, '6': '.fk_app.Schedules', '10': 'schedules'},
    {'1': 'networkSettings', '3': 6, '4': 1, '5': 11, '6': '.fk_app.NetworkSettings', '10': 'networkSettings'},
    {'1': 'loraSettings', '3': 7, '4': 1, '5': 11, '6': '.fk_app.LoraSettings', '10': 'loraSettings'},
    {'1': 'locate', '3': 9, '4': 1, '5': 11, '6': '.fk_app.Location', '10': 'locate'},
    {'1': 'transmission', '3': 10, '4': 1, '5': 11, '6': '.fk_app.Transmission', '10': 'transmission'},
    {'1': 'directory', '3': 11, '4': 1, '5': 11, '6': '.fk_app.ListDirectory', '10': 'directory'},
    {'1': 'flags', '3': 5, '4': 1, '5': 13, '10': 'flags'},
    {'1': 'time', '3': 8, '4': 1, '5': 4, '10': 'time'},
    {'1': 'counter', '3': 12, '4': 1, '5': 13, '10': 'counter'},
  ],
};

/// Descriptor for `HttpQuery`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List httpQueryDescriptor = $convert.base64Decode(
    'CglIdHRwUXVlcnkSJQoEdHlwZRgBIAEoDjIRLmZrX2FwcC5RdWVyeVR5cGVSBHR5cGUSLAoIaW'
    'RlbnRpdHkYAiABKAsyEC5ma19hcHAuSWRlbnRpdHlSCGlkZW50aXR5Ei8KCXJlY29yZGluZxgD'
    'IAEoCzIRLmZrX2FwcC5SZWNvcmRpbmdSCXJlY29yZGluZxIvCglzY2hlZHVsZXMYBCABKAsyES'
    '5ma19hcHAuU2NoZWR1bGVzUglzY2hlZHVsZXMSQQoPbmV0d29ya1NldHRpbmdzGAYgASgLMhcu'
    'ZmtfYXBwLk5ldHdvcmtTZXR0aW5nc1IPbmV0d29ya1NldHRpbmdzEjgKDGxvcmFTZXR0aW5ncx'
    'gHIAEoCzIULmZrX2FwcC5Mb3JhU2V0dGluZ3NSDGxvcmFTZXR0aW5ncxIoCgZsb2NhdGUYCSAB'
    'KAsyEC5ma19hcHAuTG9jYXRpb25SBmxvY2F0ZRI4Cgx0cmFuc21pc3Npb24YCiABKAsyFC5ma1'
    '9hcHAuVHJhbnNtaXNzaW9uUgx0cmFuc21pc3Npb24SMwoJZGlyZWN0b3J5GAsgASgLMhUuZmtf'
    'YXBwLkxpc3REaXJlY3RvcnlSCWRpcmVjdG9yeRIUCgVmbGFncxgFIAEoDVIFZmxhZ3MSEgoEdG'
    'ltZRgIIAEoBFIEdGltZRIYCgdjb3VudGVyGAwgASgNUgdjb3VudGVy');

@$core.Deprecated('Use dataStreamDescriptor instead')
const DataStream$json = {
  '1': 'DataStream',
  '2': [
    {'1': 'id', '3': 1, '4': 1, '5': 13, '10': 'id'},
    {'1': 'time', '3': 2, '4': 1, '5': 4, '10': 'time'},
    {'1': 'size', '3': 3, '4': 1, '5': 4, '10': 'size'},
    {'1': 'version', '3': 4, '4': 1, '5': 13, '10': 'version'},
    {'1': 'block', '3': 5, '4': 1, '5': 4, '10': 'block'},
    {'1': 'hash', '3': 6, '4': 1, '5': 12, '10': 'hash'},
    {'1': 'name', '3': 7, '4': 1, '5': 9, '10': 'name'},
    {'1': 'path', '3': 8, '4': 1, '5': 9, '10': 'path'},
  ],
};

/// Descriptor for `DataStream`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List dataStreamDescriptor = $convert.base64Decode(
    'CgpEYXRhU3RyZWFtEg4KAmlkGAEgASgNUgJpZBISCgR0aW1lGAIgASgEUgR0aW1lEhIKBHNpem'
    'UYAyABKARSBHNpemUSGAoHdmVyc2lvbhgEIAEoDVIHdmVyc2lvbhIUCgVibG9jaxgFIAEoBFIF'
    'YmxvY2sSEgoEaGFzaBgGIAEoDFIEaGFzaBISCgRuYW1lGAcgASgJUgRuYW1lEhIKBHBhdGgYCC'
    'ABKAlSBHBhdGg=');

@$core.Deprecated('Use liveSensorReadingDescriptor instead')
const LiveSensorReading$json = {
  '1': 'LiveSensorReading',
  '2': [
    {'1': 'sensor', '3': 1, '4': 1, '5': 11, '6': '.fk_app.SensorCapabilities', '10': 'sensor'},
    {'1': 'value', '3': 2, '4': 1, '5': 2, '10': 'value'},
    {'1': 'uncalibrated', '3': 3, '4': 1, '5': 2, '10': 'uncalibrated'},
    {'1': 'factory', '3': 4, '4': 1, '5': 2, '10': 'factory'},
    {'1': 'adc', '3': 5, '4': 1, '5': 12, '10': 'adc'},
  ],
};

/// Descriptor for `LiveSensorReading`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List liveSensorReadingDescriptor = $convert.base64Decode(
    'ChFMaXZlU2Vuc29yUmVhZGluZxIyCgZzZW5zb3IYASABKAsyGi5ma19hcHAuU2Vuc29yQ2FwYW'
    'JpbGl0aWVzUgZzZW5zb3ISFAoFdmFsdWUYAiABKAJSBXZhbHVlEiIKDHVuY2FsaWJyYXRlZBgD'
    'IAEoAlIMdW5jYWxpYnJhdGVkEhgKB2ZhY3RvcnkYBCABKAJSB2ZhY3RvcnkSEAoDYWRjGAUgAS'
    'gMUgNhZGM=');

@$core.Deprecated('Use liveModuleReadingsDescriptor instead')
const LiveModuleReadings$json = {
  '1': 'LiveModuleReadings',
  '2': [
    {'1': 'module', '3': 1, '4': 1, '5': 11, '6': '.fk_app.ModuleCapabilities', '10': 'module'},
    {'1': 'readings', '3': 2, '4': 3, '5': 11, '6': '.fk_app.LiveSensorReading', '10': 'readings'},
  ],
};

/// Descriptor for `LiveModuleReadings`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List liveModuleReadingsDescriptor = $convert.base64Decode(
    'ChJMaXZlTW9kdWxlUmVhZGluZ3MSMgoGbW9kdWxlGAEgASgLMhouZmtfYXBwLk1vZHVsZUNhcG'
    'FiaWxpdGllc1IGbW9kdWxlEjUKCHJlYWRpbmdzGAIgAygLMhkuZmtfYXBwLkxpdmVTZW5zb3JS'
    'ZWFkaW5nUghyZWFkaW5ncw==');

@$core.Deprecated('Use liveReadingsDescriptor instead')
const LiveReadings$json = {
  '1': 'LiveReadings',
  '2': [
    {'1': 'time', '3': 1, '4': 1, '5': 4, '10': 'time'},
    {'1': 'modules', '3': 2, '4': 3, '5': 11, '6': '.fk_app.LiveModuleReadings', '10': 'modules'},
  ],
};

/// Descriptor for `LiveReadings`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List liveReadingsDescriptor = $convert.base64Decode(
    'CgxMaXZlUmVhZGluZ3MSEgoEdGltZRgBIAEoBFIEdGltZRI0Cgdtb2R1bGVzGAIgAygLMhouZm'
    'tfYXBwLkxpdmVNb2R1bGVSZWFkaW5nc1IHbW9kdWxlcw==');

@$core.Deprecated('Use directoryEntryDescriptor instead')
const DirectoryEntry$json = {
  '1': 'DirectoryEntry',
  '2': [
    {'1': 'name', '3': 1, '4': 1, '5': 9, '10': 'name'},
    {'1': 'path', '3': 2, '4': 1, '5': 9, '10': 'path'},
    {'1': 'size', '3': 3, '4': 1, '5': 13, '10': 'size'},
    {'1': 'directory', '3': 4, '4': 1, '5': 8, '10': 'directory'},
  ],
};

/// Descriptor for `DirectoryEntry`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List directoryEntryDescriptor = $convert.base64Decode(
    'Cg5EaXJlY3RvcnlFbnRyeRISCgRuYW1lGAEgASgJUgRuYW1lEhIKBHBhdGgYAiABKAlSBHBhdG'
    'gSEgoEc2l6ZRgDIAEoDVIEc2l6ZRIcCglkaXJlY3RvcnkYBCABKAhSCWRpcmVjdG9yeQ==');

@$core.Deprecated('Use directoryListingDescriptor instead')
const DirectoryListing$json = {
  '1': 'DirectoryListing',
  '2': [
    {'1': 'path', '3': 1, '4': 1, '5': 9, '10': 'path'},
    {'1': 'totalEntries', '3': 3, '4': 1, '5': 13, '10': 'totalEntries'},
    {'1': 'entries', '3': 2, '4': 3, '5': 11, '6': '.fk_app.DirectoryEntry', '10': 'entries'},
  ],
};

/// Descriptor for `DirectoryListing`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List directoryListingDescriptor = $convert.base64Decode(
    'ChBEaXJlY3RvcnlMaXN0aW5nEhIKBHBhdGgYASABKAlSBHBhdGgSIgoMdG90YWxFbnRyaWVzGA'
    'MgASgNUgx0b3RhbEVudHJpZXMSMAoHZW50cmllcxgCIAMoCzIWLmZrX2FwcC5EaXJlY3RvcnlF'
    'bnRyeVIHZW50cmllcw==');

@$core.Deprecated('Use nearbyNetworkDescriptor instead')
const NearbyNetwork$json = {
  '1': 'NearbyNetwork',
  '2': [
    {'1': 'ssid', '3': 1, '4': 1, '5': 9, '10': 'ssid'},
  ],
};

/// Descriptor for `NearbyNetwork`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List nearbyNetworkDescriptor = $convert.base64Decode(
    'Cg1OZWFyYnlOZXR3b3JrEhIKBHNzaWQYASABKAlSBHNzaWQ=');

@$core.Deprecated('Use nearbyNetworksDescriptor instead')
const NearbyNetworks$json = {
  '1': 'NearbyNetworks',
  '2': [
    {'1': 'networks', '3': 1, '4': 3, '5': 11, '6': '.fk_app.NearbyNetwork', '10': 'networks'},
  ],
};

/// Descriptor for `NearbyNetworks`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List nearbyNetworksDescriptor = $convert.base64Decode(
    'Cg5OZWFyYnlOZXR3b3JrcxIxCghuZXR3b3JrcxgBIAMoCzIVLmZrX2FwcC5OZWFyYnlOZXR3b3'
    'JrUghuZXR3b3Jrcw==');

@$core.Deprecated('Use faultDescriptor instead')
const Fault$json = {
  '1': 'Fault',
  '2': [
    {'1': 'time', '3': 1, '4': 1, '5': 13, '10': 'time'},
    {'1': 'code', '3': 2, '4': 1, '5': 13, '10': 'code'},
    {'1': 'description', '3': 3, '4': 1, '5': 9, '10': 'description'},
    {'1': 'debug', '3': 4, '4': 1, '5': 12, '10': 'debug'},
  ],
};

/// Descriptor for `Fault`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List faultDescriptor = $convert.base64Decode(
    'CgVGYXVsdBISCgR0aW1lGAEgASgNUgR0aW1lEhIKBGNvZGUYAiABKA1SBGNvZGUSIAoLZGVzY3'
    'JpcHRpb24YAyABKAlSC2Rlc2NyaXB0aW9uEhQKBWRlYnVnGAQgASgMUgVkZWJ1Zw==');

@$core.Deprecated('Use httpReplyDescriptor instead')
const HttpReply$json = {
  '1': 'HttpReply',
  '2': [
    {'1': 'type', '3': 1, '4': 1, '5': 14, '6': '.fk_app.ReplyType', '10': 'type'},
    {'1': 'errors', '3': 2, '4': 3, '5': 11, '6': '.fk_app.Error', '10': 'errors'},
    {'1': 'status', '3': 3, '4': 1, '5': 11, '6': '.fk_app.Status', '10': 'status'},
    {'1': 'networkSettings', '3': 4, '4': 1, '5': 11, '6': '.fk_app.NetworkSettings', '10': 'networkSettings'},
    {'1': 'loraSettings', '3': 8, '4': 1, '5': 11, '6': '.fk_app.LoraSettings', '10': 'loraSettings'},
    {'1': 'modules', '3': 5, '4': 3, '5': 11, '6': '.fk_app.ModuleCapabilities', '10': 'modules'},
    {'1': 'streams', '3': 6, '4': 3, '5': 11, '6': '.fk_app.DataStream', '10': 'streams'},
    {'1': 'liveReadings', '3': 7, '4': 3, '5': 11, '6': '.fk_app.LiveReadings', '10': 'liveReadings'},
    {'1': 'schedules', '3': 9, '4': 1, '5': 11, '6': '.fk_app.Schedules', '10': 'schedules'},
    {'1': 'transmission', '3': 10, '4': 1, '5': 11, '6': '.fk_app.Transmission', '10': 'transmission'},
    {'1': 'listing', '3': 11, '4': 1, '5': 11, '6': '.fk_app.DirectoryListing', '10': 'listing'},
    {'1': 'nearbyNetworks', '3': 12, '4': 1, '5': 11, '6': '.fk_app.NearbyNetworks', '10': 'nearbyNetworks'},
    {'1': 'faults', '3': 13, '4': 3, '5': 11, '6': '.fk_app.Fault', '10': 'faults'},
  ],
};

/// Descriptor for `HttpReply`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List httpReplyDescriptor = $convert.base64Decode(
    'CglIdHRwUmVwbHkSJQoEdHlwZRgBIAEoDjIRLmZrX2FwcC5SZXBseVR5cGVSBHR5cGUSJQoGZX'
    'Jyb3JzGAIgAygLMg0uZmtfYXBwLkVycm9yUgZlcnJvcnMSJgoGc3RhdHVzGAMgASgLMg4uZmtf'
    'YXBwLlN0YXR1c1IGc3RhdHVzEkEKD25ldHdvcmtTZXR0aW5ncxgEIAEoCzIXLmZrX2FwcC5OZX'
    'R3b3JrU2V0dGluZ3NSD25ldHdvcmtTZXR0aW5ncxI4Cgxsb3JhU2V0dGluZ3MYCCABKAsyFC5m'
    'a19hcHAuTG9yYVNldHRpbmdzUgxsb3JhU2V0dGluZ3MSNAoHbW9kdWxlcxgFIAMoCzIaLmZrX2'
    'FwcC5Nb2R1bGVDYXBhYmlsaXRpZXNSB21vZHVsZXMSLAoHc3RyZWFtcxgGIAMoCzISLmZrX2Fw'
    'cC5EYXRhU3RyZWFtUgdzdHJlYW1zEjgKDGxpdmVSZWFkaW5ncxgHIAMoCzIULmZrX2FwcC5MaX'
    'ZlUmVhZGluZ3NSDGxpdmVSZWFkaW5ncxIvCglzY2hlZHVsZXMYCSABKAsyES5ma19hcHAuU2No'
    'ZWR1bGVzUglzY2hlZHVsZXMSOAoMdHJhbnNtaXNzaW9uGAogASgLMhQuZmtfYXBwLlRyYW5zbW'
    'lzc2lvblIMdHJhbnNtaXNzaW9uEjIKB2xpc3RpbmcYCyABKAsyGC5ma19hcHAuRGlyZWN0b3J5'
    'TGlzdGluZ1IHbGlzdGluZxI+Cg5uZWFyYnlOZXR3b3JrcxgMIAEoCzIWLmZrX2FwcC5OZWFyYn'
    'lOZXR3b3Jrc1IObmVhcmJ5TmV0d29ya3MSJQoGZmF1bHRzGA0gAygLMg0uZmtfYXBwLkZhdWx0'
    'UgZmYXVsdHM=');

@$core.Deprecated('Use moduleHttpQueryDescriptor instead')
const ModuleHttpQuery$json = {
  '1': 'ModuleHttpQuery',
  '2': [
    {'1': 'type', '3': 1, '4': 1, '5': 14, '6': '.fk_app.ModuleQueryType', '10': 'type'},
    {'1': 'errors', '3': 2, '4': 3, '5': 11, '6': '.fk_app.Error', '10': 'errors'},
    {'1': 'configuration', '3': 3, '4': 1, '5': 12, '10': 'configuration'},
  ],
};

/// Descriptor for `ModuleHttpQuery`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List moduleHttpQueryDescriptor = $convert.base64Decode(
    'Cg9Nb2R1bGVIdHRwUXVlcnkSKwoEdHlwZRgBIAEoDjIXLmZrX2FwcC5Nb2R1bGVRdWVyeVR5cG'
    'VSBHR5cGUSJQoGZXJyb3JzGAIgAygLMg0uZmtfYXBwLkVycm9yUgZlcnJvcnMSJAoNY29uZmln'
    'dXJhdGlvbhgDIAEoDFINY29uZmlndXJhdGlvbg==');

@$core.Deprecated('Use moduleHttpReplyDescriptor instead')
const ModuleHttpReply$json = {
  '1': 'ModuleHttpReply',
  '2': [
    {'1': 'type', '3': 1, '4': 1, '5': 14, '6': '.fk_app.ModuleReplyType', '10': 'type'},
    {'1': 'errors', '3': 2, '4': 3, '5': 11, '6': '.fk_app.Error', '10': 'errors'},
    {'1': 'configuration', '3': 3, '4': 1, '5': 12, '10': 'configuration'},
  ],
};

/// Descriptor for `ModuleHttpReply`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List moduleHttpReplyDescriptor = $convert.base64Decode(
    'Cg9Nb2R1bGVIdHRwUmVwbHkSKwoEdHlwZRgBIAEoDjIXLmZrX2FwcC5Nb2R1bGVSZXBseVR5cG'
    'VSBHR5cGUSJQoGZXJyb3JzGAIgAygLMg0uZmtfYXBwLkVycm9yUgZlcnJvcnMSJAoNY29uZmln'
    'dXJhdGlvbhgDIAEoDFINY29uZmlndXJhdGlvbg==');

@$core.Deprecated('Use udpMessageDescriptor instead')
const UdpMessage$json = {
  '1': 'UdpMessage',
  '2': [
    {'1': 'deviceId', '3': 1, '4': 1, '5': 12, '10': 'deviceId'},
    {'1': 'status', '3': 2, '4': 1, '5': 14, '6': '.fk_app.UdpStatus', '10': 'status'},
    {'1': 'counter', '3': 3, '4': 1, '5': 13, '10': 'counter'},
    {'1': 'port', '3': 4, '4': 1, '5': 13, '10': 'port'},
  ],
};

/// Descriptor for `UdpMessage`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List udpMessageDescriptor = $convert.base64Decode(
    'CgpVZHBNZXNzYWdlEhoKCGRldmljZUlkGAEgASgMUghkZXZpY2VJZBIpCgZzdGF0dXMYAiABKA'
    '4yES5ma19hcHAuVWRwU3RhdHVzUgZzdGF0dXMSGAoHY291bnRlchgDIAEoDVIHY291bnRlchIS'
    'CgRwb3J0GAQgASgNUgRwb3J0');

