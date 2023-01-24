///
//  Generated code. Do not modify.
//  source: fk-data.proto
//
// @dart = 2.12
// ignore_for_file: annotate_overrides,camel_case_types,constant_identifier_names,deprecated_member_use_from_same_package,directives_ordering,library_prefixes,non_constant_identifier_names,prefer_final_fields,return_of_invalid_type,unnecessary_const,unnecessary_import,unnecessary_this,unused_import,unused_shown_name

import 'dart:core' as $core;
import 'dart:convert' as $convert;
import 'dart:typed_data' as $typed_data;
@$core.Deprecated('Use downloadFlagsDescriptor instead')
const DownloadFlags$json = const {
  '1': 'DownloadFlags',
  '2': const [
    const {'1': 'READING_FLAGS_NONE', '2': 0},
    const {'1': 'READING_FLAGS_NOT_RECORDING', '2': 1},
    const {'1': 'READING_FLAGS_MANUAL', '2': 2},
  ],
};

/// Descriptor for `DownloadFlags`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List downloadFlagsDescriptor = $convert.base64Decode('Cg1Eb3dubG9hZEZsYWdzEhYKElJFQURJTkdfRkxBR1NfTk9ORRAAEh8KG1JFQURJTkdfRkxBR1NfTk9UX1JFQ09SRElORxABEhgKFFJFQURJTkdfRkxBR1NfTUFOVUFMEAI=');
@$core.Deprecated('Use conditionFlagsDescriptor instead')
const ConditionFlags$json = const {
  '1': 'ConditionFlags',
  '2': const [
    const {'1': 'CONDITION_FLAGS_NONE', '2': 0},
    const {'1': 'CONDITION_FLAGS_RECORDING', '2': 1},
  ],
};

/// Descriptor for `ConditionFlags`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List conditionFlagsDescriptor = $convert.base64Decode('Cg5Db25kaXRpb25GbGFncxIYChRDT05ESVRJT05fRkxBR1NfTk9ORRAAEh0KGUNPTkRJVElPTl9GTEFHU19SRUNPUkRJTkcQAQ==');
@$core.Deprecated('Use signedRecordKindDescriptor instead')
const SignedRecordKind$json = const {
  '1': 'SignedRecordKind',
  '2': const [
    const {'1': 'SIGNED_RECORD_KIND_NONE', '2': 0},
    const {'1': 'SIGNED_RECORD_KIND_MODULES', '2': 1},
    const {'1': 'SIGNED_RECORD_KIND_SCHEDULE', '2': 2},
    const {'1': 'SIGNED_RECORD_KIND_STATE', '2': 3},
    const {'1': 'SIGNED_RECORD_KIND_RAW_STATE', '2': 4},
    const {'1': 'SIGNED_RECORD_KIND_FAULTS', '2': 5},
    const {'1': 'SIGNED_RECORD_KIND_OTHER', '2': 255},
  ],
};

/// Descriptor for `SignedRecordKind`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List signedRecordKindDescriptor = $convert.base64Decode('ChBTaWduZWRSZWNvcmRLaW5kEhsKF1NJR05FRF9SRUNPUkRfS0lORF9OT05FEAASHgoaU0lHTkVEX1JFQ09SRF9LSU5EX01PRFVMRVMQARIfChtTSUdORURfUkVDT1JEX0tJTkRfU0NIRURVTEUQAhIcChhTSUdORURfUkVDT1JEX0tJTkRfU1RBVEUQAxIgChxTSUdORURfUkVDT1JEX0tJTkRfUkFXX1NUQVRFEAQSHQoZU0lHTkVEX1JFQ09SRF9LSU5EX0ZBVUxUUxAFEh0KGFNJR05FRF9SRUNPUkRfS0lORF9PVEhFUhD/AQ==');
@$core.Deprecated('Use curveTypeDescriptor instead')
const CurveType$json = const {
  '1': 'CurveType',
  '2': const [
    const {'1': 'CURVE_NONE', '2': 0},
    const {'1': 'CURVE_LINEAR', '2': 1},
    const {'1': 'CURVE_POWER', '2': 2},
    const {'1': 'CURVE_LOGARITHMIC', '2': 3},
    const {'1': 'CURVE_EXPONENTIAL', '2': 4},
  ],
};

/// Descriptor for `CurveType`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List curveTypeDescriptor = $convert.base64Decode('CglDdXJ2ZVR5cGUSDgoKQ1VSVkVfTk9ORRAAEhAKDENVUlZFX0xJTkVBUhABEg8KC0NVUlZFX1BPV0VSEAISFQoRQ1VSVkVfTE9HQVJJVEhNSUMQAxIVChFDVVJWRV9FWFBPTkVOVElBTBAE');
@$core.Deprecated('Use deviceLocationDescriptor instead')
const DeviceLocation$json = const {
  '1': 'DeviceLocation',
  '2': const [
    const {'1': 'enabled', '3': 7, '4': 1, '5': 13, '10': 'enabled'},
    const {'1': 'fix', '3': 1, '4': 1, '5': 13, '10': 'fix'},
    const {'1': 'time', '3': 2, '4': 1, '5': 3, '10': 'time'},
    const {'1': 'longitude', '3': 3, '4': 1, '5': 2, '10': 'longitude'},
    const {'1': 'latitude', '3': 4, '4': 1, '5': 2, '10': 'latitude'},
    const {'1': 'altitude', '3': 5, '4': 1, '5': 2, '10': 'altitude'},
    const {'1': 'coordinates', '3': 6, '4': 3, '5': 2, '10': 'coordinates'},
    const {'1': 'satellites', '3': 8, '4': 1, '5': 13, '10': 'satellites'},
    const {'1': 'hdop', '3': 9, '4': 1, '5': 13, '10': 'hdop'},
  ],
};

/// Descriptor for `DeviceLocation`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List deviceLocationDescriptor = $convert.base64Decode('Cg5EZXZpY2VMb2NhdGlvbhIYCgdlbmFibGVkGAcgASgNUgdlbmFibGVkEhAKA2ZpeBgBIAEoDVIDZml4EhIKBHRpbWUYAiABKANSBHRpbWUSHAoJbG9uZ2l0dWRlGAMgASgCUglsb25naXR1ZGUSGgoIbGF0aXR1ZGUYBCABKAJSCGxhdGl0dWRlEhoKCGFsdGl0dWRlGAUgASgCUghhbHRpdHVkZRIgCgtjb29yZGluYXRlcxgGIAMoAlILY29vcmRpbmF0ZXMSHgoKc2F0ZWxsaXRlcxgIIAEoDVIKc2F0ZWxsaXRlcxISCgRoZG9wGAkgASgNUgRoZG9w');
@$core.Deprecated('Use sensorReadingDescriptor instead')
const SensorReading$json = const {
  '1': 'SensorReading',
  '2': const [
    const {'1': 'reading', '3': 1, '4': 1, '5': 4, '10': 'reading'},
    const {'1': 'time', '3': 2, '4': 1, '5': 3, '10': 'time'},
    const {'1': 'sensor', '3': 3, '4': 1, '5': 13, '10': 'sensor'},
    const {'1': 'value', '3': 4, '4': 1, '5': 2, '10': 'value'},
  ],
};

/// Descriptor for `SensorReading`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List sensorReadingDescriptor = $convert.base64Decode('Cg1TZW5zb3JSZWFkaW5nEhgKB3JlYWRpbmcYASABKARSB3JlYWRpbmcSEgoEdGltZRgCIAEoA1IEdGltZRIWCgZzZW5zb3IYAyABKA1SBnNlbnNvchIUCgV2YWx1ZRgEIAEoAlIFdmFsdWU=');
@$core.Deprecated('Use loggedReadingDescriptor instead')
const LoggedReading$json = const {
  '1': 'LoggedReading',
  '2': const [
    const {'1': 'version', '3': 1, '4': 1, '5': 13, '10': 'version'},
    const {'1': 'location', '3': 2, '4': 1, '5': 11, '6': '.fk_data.DeviceLocation', '10': 'location'},
    const {'1': 'reading', '3': 3, '4': 1, '5': 11, '6': '.fk_data.SensorReading', '10': 'reading'},
  ],
};

/// Descriptor for `LoggedReading`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List loggedReadingDescriptor = $convert.base64Decode('Cg1Mb2dnZWRSZWFkaW5nEhgKB3ZlcnNpb24YASABKA1SB3ZlcnNpb24SMwoIbG9jYXRpb24YAiABKAsyFy5ma19kYXRhLkRldmljZUxvY2F0aW9uUghsb2NhdGlvbhIwCgdyZWFkaW5nGAMgASgLMhYuZmtfZGF0YS5TZW5zb3JSZWFkaW5nUgdyZWFkaW5n');
@$core.Deprecated('Use sensorAndValueDescriptor instead')
const SensorAndValue$json = const {
  '1': 'SensorAndValue',
  '2': const [
    const {'1': 'sensor', '3': 1, '4': 1, '5': 13, '10': 'sensor'},
    const {'1': 'value', '3': 2, '4': 1, '5': 2, '10': 'value'},
    const {'1': 'uncalibrated', '3': 3, '4': 1, '5': 2, '10': 'uncalibrated'},
  ],
};

/// Descriptor for `SensorAndValue`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List sensorAndValueDescriptor = $convert.base64Decode('Cg5TZW5zb3JBbmRWYWx1ZRIWCgZzZW5zb3IYASABKA1SBnNlbnNvchIUCgV2YWx1ZRgCIAEoAlIFdmFsdWUSIgoMdW5jYWxpYnJhdGVkGAMgASgCUgx1bmNhbGlicmF0ZWQ=');
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
@$core.Deprecated('Use moduleInfoDescriptor instead')
const ModuleInfo$json = const {
  '1': 'ModuleInfo',
  '2': const [
    const {'1': 'position', '3': 1, '4': 1, '5': 13, '10': 'position'},
    const {'1': 'address', '3': 2, '4': 1, '5': 13, '10': 'address'},
    const {'1': 'name', '3': 3, '4': 1, '5': 9, '10': 'name'},
    const {'1': 'header', '3': 4, '4': 1, '5': 11, '6': '.fk_data.ModuleHeader', '10': 'header'},
    const {'1': 'firmware', '3': 5, '4': 1, '5': 11, '6': '.fk_data.Firmware', '10': 'firmware'},
    const {'1': 'sensors', '3': 6, '4': 3, '5': 11, '6': '.fk_data.SensorInfo', '10': 'sensors'},
    const {'1': 'id', '3': 7, '4': 1, '5': 12, '10': 'id'},
    const {'1': 'flags', '3': 8, '4': 1, '5': 13, '10': 'flags'},
    const {'1': 'configuration', '3': 9, '4': 1, '5': 12, '10': 'configuration'},
  ],
};

/// Descriptor for `ModuleInfo`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List moduleInfoDescriptor = $convert.base64Decode('CgpNb2R1bGVJbmZvEhoKCHBvc2l0aW9uGAEgASgNUghwb3NpdGlvbhIYCgdhZGRyZXNzGAIgASgNUgdhZGRyZXNzEhIKBG5hbWUYAyABKAlSBG5hbWUSLQoGaGVhZGVyGAQgASgLMhUuZmtfZGF0YS5Nb2R1bGVIZWFkZXJSBmhlYWRlchItCghmaXJtd2FyZRgFIAEoCzIRLmZrX2RhdGEuRmlybXdhcmVSCGZpcm13YXJlEi0KB3NlbnNvcnMYBiADKAsyEy5ma19kYXRhLlNlbnNvckluZm9SB3NlbnNvcnMSDgoCaWQYByABKAxSAmlkEhQKBWZsYWdzGAggASgNUgVmbGFncxIkCg1jb25maWd1cmF0aW9uGAkgASgMUg1jb25maWd1cmF0aW9u');
@$core.Deprecated('Use sensorInfoDescriptor instead')
const SensorInfo$json = const {
  '1': 'SensorInfo',
  '2': const [
    const {'1': 'number', '3': 1, '4': 1, '5': 13, '10': 'number'},
    const {'1': 'name', '3': 2, '4': 1, '5': 9, '10': 'name'},
    const {'1': 'unitOfMeasure', '3': 3, '4': 1, '5': 9, '10': 'unitOfMeasure'},
    const {'1': 'uncalibratedUnitOfMeasure', '3': 5, '4': 1, '5': 9, '10': 'uncalibratedUnitOfMeasure'},
    const {'1': 'flags', '3': 4, '4': 1, '5': 13, '10': 'flags'},
  ],
};

/// Descriptor for `SensorInfo`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List sensorInfoDescriptor = $convert.base64Decode('CgpTZW5zb3JJbmZvEhYKBm51bWJlchgBIAEoDVIGbnVtYmVyEhIKBG5hbWUYAiABKAlSBG5hbWUSJAoNdW5pdE9mTWVhc3VyZRgDIAEoCVINdW5pdE9mTWVhc3VyZRI8Chl1bmNhbGlicmF0ZWRVbml0T2ZNZWFzdXJlGAUgASgJUhl1bmNhbGlicmF0ZWRVbml0T2ZNZWFzdXJlEhQKBWZsYWdzGAQgASgNUgVmbGFncw==');
@$core.Deprecated('Use firmwareDescriptor instead')
const Firmware$json = const {
  '1': 'Firmware',
  '2': const [
    const {'1': 'version', '3': 1, '4': 1, '5': 9, '10': 'version'},
    const {'1': 'build', '3': 2, '4': 1, '5': 9, '10': 'build'},
    const {'1': 'number', '3': 3, '4': 1, '5': 9, '10': 'number'},
    const {'1': 'timestamp', '3': 4, '4': 1, '5': 4, '10': 'timestamp'},
    const {'1': 'hash', '3': 5, '4': 1, '5': 9, '10': 'hash'},
  ],
};

/// Descriptor for `Firmware`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List firmwareDescriptor = $convert.base64Decode('CghGaXJtd2FyZRIYCgd2ZXJzaW9uGAEgASgJUgd2ZXJzaW9uEhQKBWJ1aWxkGAIgASgJUgVidWlsZBIWCgZudW1iZXIYAyABKAlSBm51bWJlchIcCgl0aW1lc3RhbXAYBCABKARSCXRpbWVzdGFtcBISCgRoYXNoGAUgASgJUgRoYXNo');
@$core.Deprecated('Use metadataDescriptor instead')
const Metadata$json = const {
  '1': 'Metadata',
  '2': const [
    const {'1': 'deviceId', '3': 1, '4': 1, '5': 12, '10': 'deviceId'},
    const {'1': 'time', '3': 2, '4': 1, '5': 3, '10': 'time'},
    const {'1': 'git', '3': 3, '4': 1, '5': 9, '10': 'git'},
    const {'1': 'build', '3': 7, '4': 1, '5': 9, '10': 'build'},
    const {'1': 'resetCause', '3': 4, '4': 1, '5': 13, '10': 'resetCause'},
    const {'1': 'sensors', '3': 5, '4': 3, '5': 11, '6': '.fk_data.SensorInfo', '10': 'sensors'},
    const {'1': 'modules', '3': 6, '4': 3, '5': 11, '6': '.fk_data.ModuleInfo', '10': 'modules'},
    const {'1': 'firmware', '3': 8, '4': 1, '5': 11, '6': '.fk_data.Firmware', '10': 'firmware'},
    const {'1': 'generation', '3': 9, '4': 1, '5': 12, '10': 'generation'},
    const {'1': 'record', '3': 10, '4': 1, '5': 4, '10': 'record'},
  ],
};

/// Descriptor for `Metadata`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List metadataDescriptor = $convert.base64Decode('CghNZXRhZGF0YRIaCghkZXZpY2VJZBgBIAEoDFIIZGV2aWNlSWQSEgoEdGltZRgCIAEoA1IEdGltZRIQCgNnaXQYAyABKAlSA2dpdBIUCgVidWlsZBgHIAEoCVIFYnVpbGQSHgoKcmVzZXRDYXVzZRgEIAEoDVIKcmVzZXRDYXVzZRItCgdzZW5zb3JzGAUgAygLMhMuZmtfZGF0YS5TZW5zb3JJbmZvUgdzZW5zb3JzEi0KB21vZHVsZXMYBiADKAsyEy5ma19kYXRhLk1vZHVsZUluZm9SB21vZHVsZXMSLQoIZmlybXdhcmUYCCABKAsyES5ma19kYXRhLkZpcm13YXJlUghmaXJtd2FyZRIeCgpnZW5lcmF0aW9uGAkgASgMUgpnZW5lcmF0aW9uEhYKBnJlY29yZBgKIAEoBFIGcmVjb3Jk');
@$core.Deprecated('Use statusDescriptor instead')
const Status$json = const {
  '1': 'Status',
  '2': const [
    const {'1': 'time', '3': 1, '4': 1, '5': 3, '10': 'time'},
    const {'1': 'uptime', '3': 2, '4': 1, '5': 13, '10': 'uptime'},
    const {'1': 'battery', '3': 3, '4': 1, '5': 2, '10': 'battery'},
    const {'1': 'memory', '3': 4, '4': 1, '5': 13, '10': 'memory'},
    const {'1': 'busy', '3': 5, '4': 1, '5': 4, '10': 'busy'},
  ],
};

/// Descriptor for `Status`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List statusDescriptor = $convert.base64Decode('CgZTdGF0dXMSEgoEdGltZRgBIAEoA1IEdGltZRIWCgZ1cHRpbWUYAiABKA1SBnVwdGltZRIYCgdiYXR0ZXJ5GAMgASgCUgdiYXR0ZXJ5EhYKBm1lbW9yeRgEIAEoDVIGbWVtb3J5EhIKBGJ1c3kYBSABKARSBGJ1c3k=');
@$core.Deprecated('Use logMessageDescriptor instead')
const LogMessage$json = const {
  '1': 'LogMessage',
  '2': const [
    const {'1': 'time', '3': 1, '4': 1, '5': 3, '10': 'time'},
    const {'1': 'uptime', '3': 2, '4': 1, '5': 13, '10': 'uptime'},
    const {'1': 'level', '3': 3, '4': 1, '5': 13, '10': 'level'},
    const {'1': 'facility', '3': 4, '4': 1, '5': 9, '10': 'facility'},
    const {'1': 'message', '3': 5, '4': 1, '5': 9, '10': 'message'},
  ],
};

/// Descriptor for `LogMessage`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List logMessageDescriptor = $convert.base64Decode('CgpMb2dNZXNzYWdlEhIKBHRpbWUYASABKANSBHRpbWUSFgoGdXB0aW1lGAIgASgNUgZ1cHRpbWUSFAoFbGV2ZWwYAyABKA1SBWxldmVsEhoKCGZhY2lsaXR5GAQgASgJUghmYWNpbGl0eRIYCgdtZXNzYWdlGAUgASgJUgdtZXNzYWdl');
@$core.Deprecated('Use sensorGroupDescriptor instead')
const SensorGroup$json = const {
  '1': 'SensorGroup',
  '2': const [
    const {'1': 'module', '3': 1, '4': 1, '5': 13, '10': 'module'},
    const {'1': 'time', '3': 3, '4': 1, '5': 3, '10': 'time'},
    const {'1': 'readings', '3': 2, '4': 3, '5': 11, '6': '.fk_data.SensorAndValue', '10': 'readings'},
  ],
};

/// Descriptor for `SensorGroup`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List sensorGroupDescriptor = $convert.base64Decode('CgtTZW5zb3JHcm91cBIWCgZtb2R1bGUYASABKA1SBm1vZHVsZRISCgR0aW1lGAMgASgDUgR0aW1lEjMKCHJlYWRpbmdzGAIgAygLMhcuZmtfZGF0YS5TZW5zb3JBbmRWYWx1ZVIIcmVhZGluZ3M=');
@$core.Deprecated('Use readingsDescriptor instead')
const Readings$json = const {
  '1': 'Readings',
  '2': const [
    const {'1': 'time', '3': 1, '4': 1, '5': 3, '10': 'time'},
    const {'1': 'reading', '3': 2, '4': 1, '5': 4, '10': 'reading'},
    const {'1': 'flags', '3': 3, '4': 1, '5': 13, '10': 'flags'},
    const {'1': 'meta', '3': 6, '4': 1, '5': 4, '10': 'meta'},
    const {'1': 'uptime', '3': 7, '4': 1, '5': 13, '10': 'uptime'},
    const {'1': 'location', '3': 4, '4': 1, '5': 11, '6': '.fk_data.DeviceLocation', '10': 'location'},
    const {'1': 'sensorGroups', '3': 5, '4': 3, '5': 11, '6': '.fk_data.SensorGroup', '10': 'sensorGroups'},
  ],
};

/// Descriptor for `Readings`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List readingsDescriptor = $convert.base64Decode('CghSZWFkaW5ncxISCgR0aW1lGAEgASgDUgR0aW1lEhgKB3JlYWRpbmcYAiABKARSB3JlYWRpbmcSFAoFZmxhZ3MYAyABKA1SBWZsYWdzEhIKBG1ldGEYBiABKARSBG1ldGESFgoGdXB0aW1lGAcgASgNUgZ1cHRpbWUSMwoIbG9jYXRpb24YBCABKAsyFy5ma19kYXRhLkRldmljZUxvY2F0aW9uUghsb2NhdGlvbhI4CgxzZW5zb3JHcm91cHMYBSADKAsyFC5ma19kYXRhLlNlbnNvckdyb3VwUgxzZW5zb3JHcm91cHM=');
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
@$core.Deprecated('Use jobScheduleDescriptor instead')
const JobSchedule$json = const {
  '1': 'JobSchedule',
  '2': const [
    const {'1': 'cron', '3': 1, '4': 1, '5': 12, '10': 'cron'},
    const {'1': 'interval', '3': 2, '4': 1, '5': 13, '10': 'interval'},
    const {'1': 'repeated', '3': 3, '4': 1, '5': 13, '10': 'repeated'},
    const {'1': 'duration', '3': 4, '4': 1, '5': 13, '10': 'duration'},
    const {'1': 'jitter', '3': 5, '4': 1, '5': 13, '10': 'jitter'},
    const {'1': 'intervals', '3': 6, '4': 3, '5': 11, '6': '.fk_data.Interval', '10': 'intervals'},
  ],
};

/// Descriptor for `JobSchedule`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List jobScheduleDescriptor = $convert.base64Decode('CgtKb2JTY2hlZHVsZRISCgRjcm9uGAEgASgMUgRjcm9uEhoKCGludGVydmFsGAIgASgNUghpbnRlcnZhbBIaCghyZXBlYXRlZBgDIAEoDVIIcmVwZWF0ZWQSGgoIZHVyYXRpb24YBCABKA1SCGR1cmF0aW9uEhYKBmppdHRlchgFIAEoDVIGaml0dGVyEi8KCWludGVydmFscxgGIAMoCzIRLmZrX2RhdGEuSW50ZXJ2YWxSCWludGVydmFscw==');
@$core.Deprecated('Use scheduleDescriptor instead')
const Schedule$json = const {
  '1': 'Schedule',
  '2': const [
    const {'1': 'readings', '3': 1, '4': 1, '5': 11, '6': '.fk_data.JobSchedule', '10': 'readings'},
    const {'1': 'network', '3': 2, '4': 1, '5': 11, '6': '.fk_data.JobSchedule', '10': 'network'},
    const {'1': 'lora', '3': 3, '4': 1, '5': 11, '6': '.fk_data.JobSchedule', '10': 'lora'},
    const {'1': 'gps', '3': 4, '4': 1, '5': 11, '6': '.fk_data.JobSchedule', '10': 'gps'},
  ],
};

/// Descriptor for `Schedule`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List scheduleDescriptor = $convert.base64Decode('CghTY2hlZHVsZRIwCghyZWFkaW5ncxgBIAEoCzIULmZrX2RhdGEuSm9iU2NoZWR1bGVSCHJlYWRpbmdzEi4KB25ldHdvcmsYAiABKAsyFC5ma19kYXRhLkpvYlNjaGVkdWxlUgduZXR3b3JrEigKBGxvcmEYAyABKAsyFC5ma19kYXRhLkpvYlNjaGVkdWxlUgRsb3JhEiYKA2dwcxgEIAEoCzIULmZrX2RhdGEuSm9iU2NoZWR1bGVSA2dwcw==');
@$core.Deprecated('Use identityDescriptor instead')
const Identity$json = const {
  '1': 'Identity',
  '2': const [
    const {'1': 'name', '3': 1, '4': 1, '5': 9, '10': 'name'},
  ],
};

/// Descriptor for `Identity`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List identityDescriptor = $convert.base64Decode('CghJZGVudGl0eRISCgRuYW1lGAEgASgJUgRuYW1l');
@$core.Deprecated('Use conditionDescriptor instead')
const Condition$json = const {
  '1': 'Condition',
  '2': const [
    const {'1': 'flags', '3': 1, '4': 1, '5': 13, '10': 'flags'},
    const {'1': 'recording', '3': 2, '4': 1, '5': 13, '10': 'recording'},
  ],
};

/// Descriptor for `Condition`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List conditionDescriptor = $convert.base64Decode('CglDb25kaXRpb24SFAoFZmxhZ3MYASABKA1SBWZsYWdzEhwKCXJlY29yZGluZxgCIAEoDVIJcmVjb3JkaW5n');
@$core.Deprecated('Use networkInfoDescriptor instead')
const NetworkInfo$json = const {
  '1': 'NetworkInfo',
  '2': const [
    const {'1': 'ssid', '3': 1, '4': 1, '5': 9, '10': 'ssid'},
    const {'1': 'password', '3': 2, '4': 1, '5': 9, '10': 'password'},
    const {'1': 'create', '3': 3, '4': 1, '5': 8, '10': 'create'},
    const {'1': 'preferred', '3': 4, '4': 1, '5': 8, '10': 'preferred'},
  ],
};

/// Descriptor for `NetworkInfo`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List networkInfoDescriptor = $convert.base64Decode('CgtOZXR3b3JrSW5mbxISCgRzc2lkGAEgASgJUgRzc2lkEhoKCHBhc3N3b3JkGAIgASgJUghwYXNzd29yZBIWCgZjcmVhdGUYAyABKAhSBmNyZWF0ZRIcCglwcmVmZXJyZWQYBCABKAhSCXByZWZlcnJlZA==');
@$core.Deprecated('Use wifiTransmissionDescriptor instead')
const WifiTransmission$json = const {
  '1': 'WifiTransmission',
  '2': const [
    const {'1': 'url', '3': 1, '4': 1, '5': 9, '10': 'url'},
    const {'1': 'token', '3': 2, '4': 1, '5': 9, '10': 'token'},
    const {'1': 'enabled', '3': 3, '4': 1, '5': 8, '10': 'enabled'},
  ],
};

/// Descriptor for `WifiTransmission`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List wifiTransmissionDescriptor = $convert.base64Decode('ChBXaWZpVHJhbnNtaXNzaW9uEhAKA3VybBgBIAEoCVIDdXJsEhQKBXRva2VuGAIgASgJUgV0b2tlbhIYCgdlbmFibGVkGAMgASgIUgdlbmFibGVk');
@$core.Deprecated('Use transmissionSettingsDescriptor instead')
const TransmissionSettings$json = const {
  '1': 'TransmissionSettings',
  '2': const [
    const {'1': 'wifi', '3': 1, '4': 1, '5': 11, '6': '.fk_data.WifiTransmission', '10': 'wifi'},
  ],
};

/// Descriptor for `TransmissionSettings`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transmissionSettingsDescriptor = $convert.base64Decode('ChRUcmFuc21pc3Npb25TZXR0aW5ncxItCgR3aWZpGAEgASgLMhkuZmtfZGF0YS5XaWZpVHJhbnNtaXNzaW9uUgR3aWZp');
@$core.Deprecated('Use networkSettingsDescriptor instead')
const NetworkSettings$json = const {
  '1': 'NetworkSettings',
  '2': const [
    const {'1': 'networks', '3': 1, '4': 3, '5': 11, '6': '.fk_data.NetworkInfo', '10': 'networks'},
  ],
};

/// Descriptor for `NetworkSettings`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List networkSettingsDescriptor = $convert.base64Decode('Cg9OZXR3b3JrU2V0dGluZ3MSMAoIbmV0d29ya3MYASADKAsyFC5ma19kYXRhLk5ldHdvcmtJbmZvUghuZXR3b3Jrcw==');
@$core.Deprecated('Use loraSettingsDescriptor instead')
const LoraSettings$json = const {
  '1': 'LoraSettings',
  '2': const [
    const {'1': 'deviceEui', '3': 1, '4': 1, '5': 12, '10': 'deviceEui'},
    const {'1': 'appKey', '3': 2, '4': 1, '5': 12, '10': 'appKey'},
    const {'1': 'joinEui', '3': 3, '4': 1, '5': 12, '10': 'joinEui'},
    const {'1': 'frequencyBand', '3': 4, '4': 1, '5': 13, '10': 'frequencyBand'},
    const {'1': 'deviceAddress', '3': 5, '4': 1, '5': 12, '10': 'deviceAddress'},
    const {'1': 'networkSessionKey', '3': 6, '4': 1, '5': 12, '10': 'networkSessionKey'},
    const {'1': 'appSessionKey', '3': 7, '4': 1, '5': 12, '10': 'appSessionKey'},
    const {'1': 'uplinkCounter', '3': 8, '4': 1, '5': 13, '10': 'uplinkCounter'},
    const {'1': 'downlinkCounter', '3': 9, '4': 1, '5': 13, '10': 'downlinkCounter'},
    const {'1': 'rxDelay1', '3': 10, '4': 1, '5': 13, '10': 'rxDelay1'},
    const {'1': 'rxDelay2', '3': 11, '4': 1, '5': 13, '10': 'rxDelay2'},
  ],
};

/// Descriptor for `LoraSettings`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List loraSettingsDescriptor = $convert.base64Decode('CgxMb3JhU2V0dGluZ3MSHAoJZGV2aWNlRXVpGAEgASgMUglkZXZpY2VFdWkSFgoGYXBwS2V5GAIgASgMUgZhcHBLZXkSGAoHam9pbkV1aRgDIAEoDFIHam9pbkV1aRIkCg1mcmVxdWVuY3lCYW5kGAQgASgNUg1mcmVxdWVuY3lCYW5kEiQKDWRldmljZUFkZHJlc3MYBSABKAxSDWRldmljZUFkZHJlc3MSLAoRbmV0d29ya1Nlc3Npb25LZXkYBiABKAxSEW5ldHdvcmtTZXNzaW9uS2V5EiQKDWFwcFNlc3Npb25LZXkYByABKAxSDWFwcFNlc3Npb25LZXkSJAoNdXBsaW5rQ291bnRlchgIIAEoDVINdXBsaW5rQ291bnRlchIoCg9kb3dubGlua0NvdW50ZXIYCSABKA1SD2Rvd25saW5rQ291bnRlchIaCghyeERlbGF5MRgKIAEoDVIIcnhEZWxheTESGgoIcnhEZWxheTIYCyABKA1SCHJ4RGVsYXky');
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
@$core.Deprecated('Use dataRecordDescriptor instead')
const DataRecord$json = const {
  '1': 'DataRecord',
  '2': const [
    const {'1': 'loggedReading', '3': 1, '4': 1, '5': 11, '6': '.fk_data.LoggedReading', '10': 'loggedReading'},
    const {'1': 'metadata', '3': 2, '4': 1, '5': 11, '6': '.fk_data.Metadata', '10': 'metadata'},
    const {'1': 'log', '3': 3, '4': 1, '5': 11, '6': '.fk_data.LogMessage', '10': 'log'},
    const {'1': 'logs', '3': 13, '4': 3, '5': 11, '6': '.fk_data.LogMessage', '10': 'logs'},
    const {'1': 'status', '3': 4, '4': 1, '5': 11, '6': '.fk_data.Status', '10': 'status'},
    const {'1': 'readings', '3': 5, '4': 1, '5': 11, '6': '.fk_data.Readings', '10': 'readings'},
    const {'1': 'modules', '3': 6, '4': 3, '5': 11, '6': '.fk_data.ModuleInfo', '10': 'modules'},
    const {'1': 'schedule', '3': 7, '4': 1, '5': 11, '6': '.fk_data.Schedule', '10': 'schedule'},
    const {'1': 'meta', '3': 8, '4': 1, '5': 4, '10': 'meta'},
    const {'1': 'identity', '3': 9, '4': 1, '5': 11, '6': '.fk_data.Identity', '10': 'identity'},
    const {'1': 'condition', '3': 10, '4': 1, '5': 11, '6': '.fk_data.Condition', '10': 'condition'},
    const {'1': 'lora', '3': 11, '4': 1, '5': 11, '6': '.fk_data.LoraSettings', '10': 'lora'},
    const {'1': 'network', '3': 12, '4': 1, '5': 11, '6': '.fk_data.NetworkSettings', '10': 'network'},
    const {'1': 'transmission', '3': 14, '4': 1, '5': 11, '6': '.fk_data.TransmissionSettings', '10': 'transmission'},
    const {'1': 'faults', '3': 15, '4': 3, '5': 11, '6': '.fk_data.Fault', '10': 'faults'},
  ],
};

/// Descriptor for `DataRecord`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List dataRecordDescriptor = $convert.base64Decode('CgpEYXRhUmVjb3JkEjwKDWxvZ2dlZFJlYWRpbmcYASABKAsyFi5ma19kYXRhLkxvZ2dlZFJlYWRpbmdSDWxvZ2dlZFJlYWRpbmcSLQoIbWV0YWRhdGEYAiABKAsyES5ma19kYXRhLk1ldGFkYXRhUghtZXRhZGF0YRIlCgNsb2cYAyABKAsyEy5ma19kYXRhLkxvZ01lc3NhZ2VSA2xvZxInCgRsb2dzGA0gAygLMhMuZmtfZGF0YS5Mb2dNZXNzYWdlUgRsb2dzEicKBnN0YXR1cxgEIAEoCzIPLmZrX2RhdGEuU3RhdHVzUgZzdGF0dXMSLQoIcmVhZGluZ3MYBSABKAsyES5ma19kYXRhLlJlYWRpbmdzUghyZWFkaW5ncxItCgdtb2R1bGVzGAYgAygLMhMuZmtfZGF0YS5Nb2R1bGVJbmZvUgdtb2R1bGVzEi0KCHNjaGVkdWxlGAcgASgLMhEuZmtfZGF0YS5TY2hlZHVsZVIIc2NoZWR1bGUSEgoEbWV0YRgIIAEoBFIEbWV0YRItCghpZGVudGl0eRgJIAEoCzIRLmZrX2RhdGEuSWRlbnRpdHlSCGlkZW50aXR5EjAKCWNvbmRpdGlvbhgKIAEoCzISLmZrX2RhdGEuQ29uZGl0aW9uUgljb25kaXRpb24SKQoEbG9yYRgLIAEoCzIVLmZrX2RhdGEuTG9yYVNldHRpbmdzUgRsb3JhEjIKB25ldHdvcmsYDCABKAsyGC5ma19kYXRhLk5ldHdvcmtTZXR0aW5nc1IHbmV0d29yaxJBCgx0cmFuc21pc3Npb24YDiABKAsyHS5ma19kYXRhLlRyYW5zbWlzc2lvblNldHRpbmdzUgx0cmFuc21pc3Npb24SJgoGZmF1bHRzGA8gAygLMg4uZmtfZGF0YS5GYXVsdFIGZmF1bHRz');
@$core.Deprecated('Use signedRecordDescriptor instead')
const SignedRecord$json = const {
  '1': 'SignedRecord',
  '2': const [
    const {'1': 'kind', '3': 1, '4': 1, '5': 14, '6': '.fk_data.SignedRecordKind', '10': 'kind'},
    const {'1': 'time', '3': 2, '4': 1, '5': 3, '10': 'time'},
    const {'1': 'data', '3': 3, '4': 1, '5': 12, '10': 'data'},
    const {'1': 'hash', '3': 4, '4': 1, '5': 12, '10': 'hash'},
    const {'1': 'record', '3': 5, '4': 1, '5': 4, '10': 'record'},
  ],
};

/// Descriptor for `SignedRecord`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signedRecordDescriptor = $convert.base64Decode('CgxTaWduZWRSZWNvcmQSLQoEa2luZBgBIAEoDjIZLmZrX2RhdGEuU2lnbmVkUmVjb3JkS2luZFIEa2luZBISCgR0aW1lGAIgASgDUgR0aW1lEhIKBGRhdGEYAyABKAxSBGRhdGESEgoEaGFzaBgEIAEoDFIEaGFzaBIWCgZyZWNvcmQYBSABKARSBnJlY29yZA==');
@$core.Deprecated('Use loraRecordDescriptor instead')
const LoraRecord$json = const {
  '1': 'LoraRecord',
  '2': const [
    const {'1': 'deviceId', '3': 1, '4': 1, '5': 12, '10': 'deviceId'},
    const {'1': 'time', '3': 2, '4': 1, '5': 3, '10': 'time'},
    const {'1': 'number', '3': 3, '4': 1, '5': 4, '10': 'number'},
    const {'1': 'module', '3': 4, '4': 1, '5': 13, '10': 'module'},
    const {'1': 'sensor', '3': 5, '4': 1, '5': 4, '10': 'sensor'},
    const {'1': 'values', '3': 6, '4': 3, '5': 2, '10': 'values'},
    const {'1': 'data', '3': 7, '4': 1, '5': 12, '10': 'data'},
  ],
};

/// Descriptor for `LoraRecord`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List loraRecordDescriptor = $convert.base64Decode('CgpMb3JhUmVjb3JkEhoKCGRldmljZUlkGAEgASgMUghkZXZpY2VJZBISCgR0aW1lGAIgASgDUgR0aW1lEhYKBm51bWJlchgDIAEoBFIGbnVtYmVyEhYKBm1vZHVsZRgEIAEoDVIGbW9kdWxlEhYKBnNlbnNvchgFIAEoBFIGc2Vuc29yEhYKBnZhbHVlcxgGIAMoAlIGdmFsdWVzEhIKBGRhdGEYByABKAxSBGRhdGE=');
@$core.Deprecated('Use calibrationPointDescriptor instead')
const CalibrationPoint$json = const {
  '1': 'CalibrationPoint',
  '2': const [
    const {'1': 'references', '3': 1, '4': 3, '5': 2, '10': 'references'},
    const {'1': 'uncalibrated', '3': 2, '4': 3, '5': 2, '10': 'uncalibrated'},
    const {'1': 'factory', '3': 3, '4': 3, '5': 2, '10': 'factory'},
    const {'1': 'adc', '3': 4, '4': 3, '5': 12, '10': 'adc'},
  ],
};

/// Descriptor for `CalibrationPoint`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List calibrationPointDescriptor = $convert.base64Decode('ChBDYWxpYnJhdGlvblBvaW50Eh4KCnJlZmVyZW5jZXMYASADKAJSCnJlZmVyZW5jZXMSIgoMdW5jYWxpYnJhdGVkGAIgAygCUgx1bmNhbGlicmF0ZWQSGAoHZmFjdG9yeRgDIAMoAlIHZmFjdG9yeRIQCgNhZGMYBCADKAxSA2FkYw==');
@$core.Deprecated('Use calibrationCoefficientsDescriptor instead')
const CalibrationCoefficients$json = const {
  '1': 'CalibrationCoefficients',
  '2': const [
    const {'1': 'values', '3': 1, '4': 3, '5': 2, '10': 'values'},
  ],
};

/// Descriptor for `CalibrationCoefficients`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List calibrationCoefficientsDescriptor = $convert.base64Decode('ChdDYWxpYnJhdGlvbkNvZWZmaWNpZW50cxIWCgZ2YWx1ZXMYASADKAJSBnZhbHVlcw==');
@$core.Deprecated('Use calibrationDescriptor instead')
const Calibration$json = const {
  '1': 'Calibration',
  '2': const [
    const {'1': 'type', '3': 1, '4': 1, '5': 14, '6': '.fk_data.CurveType', '10': 'type'},
    const {'1': 'time', '3': 2, '4': 1, '5': 13, '10': 'time'},
    const {'1': 'kind', '3': 6, '4': 1, '5': 13, '10': 'kind'},
    const {'1': 'points', '3': 3, '4': 3, '5': 11, '6': '.fk_data.CalibrationPoint', '10': 'points'},
    const {'1': 'coefficients', '3': 4, '4': 1, '5': 11, '6': '.fk_data.CalibrationCoefficients', '10': 'coefficients'},
    const {'1': 'firmware', '3': 5, '4': 1, '5': 11, '6': '.fk_data.Firmware', '10': 'firmware'},
  ],
};

/// Descriptor for `Calibration`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List calibrationDescriptor = $convert.base64Decode('CgtDYWxpYnJhdGlvbhImCgR0eXBlGAEgASgOMhIuZmtfZGF0YS5DdXJ2ZVR5cGVSBHR5cGUSEgoEdGltZRgCIAEoDVIEdGltZRISCgRraW5kGAYgASgNUgRraW5kEjEKBnBvaW50cxgDIAMoCzIZLmZrX2RhdGEuQ2FsaWJyYXRpb25Qb2ludFIGcG9pbnRzEkQKDGNvZWZmaWNpZW50cxgEIAEoCzIgLmZrX2RhdGEuQ2FsaWJyYXRpb25Db2VmZmljaWVudHNSDGNvZWZmaWNpZW50cxItCghmaXJtd2FyZRgFIAEoCzIRLmZrX2RhdGEuRmlybXdhcmVSCGZpcm13YXJl');
@$core.Deprecated('Use moduleConfigurationDescriptor instead')
const ModuleConfiguration$json = const {
  '1': 'ModuleConfiguration',
  '2': const [
    const {'1': 'calibration', '3': 1, '4': 1, '5': 11, '6': '.fk_data.Calibration', '10': 'calibration'},
    const {'1': 'calibrations', '3': 2, '4': 3, '5': 11, '6': '.fk_data.Calibration', '10': 'calibrations'},
  ],
};

/// Descriptor for `ModuleConfiguration`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List moduleConfigurationDescriptor = $convert.base64Decode('ChNNb2R1bGVDb25maWd1cmF0aW9uEjYKC2NhbGlicmF0aW9uGAEgASgLMhQuZmtfZGF0YS5DYWxpYnJhdGlvblILY2FsaWJyYXRpb24SOAoMY2FsaWJyYXRpb25zGAIgAygLMhQuZmtfZGF0YS5DYWxpYnJhdGlvblIMY2FsaWJyYXRpb25z');
