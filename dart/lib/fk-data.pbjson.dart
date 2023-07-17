//
//  Generated code. Do not modify.
//  source: fk-data.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use downloadFlagsDescriptor instead')
const DownloadFlags$json = {
  '1': 'DownloadFlags',
  '2': [
    {'1': 'READING_FLAGS_NONE', '2': 0},
    {'1': 'READING_FLAGS_NOT_RECORDING', '2': 1},
    {'1': 'READING_FLAGS_MANUAL', '2': 2},
  ],
};

/// Descriptor for `DownloadFlags`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List downloadFlagsDescriptor = $convert.base64Decode(
    'Cg1Eb3dubG9hZEZsYWdzEhYKElJFQURJTkdfRkxBR1NfTk9ORRAAEh8KG1JFQURJTkdfRkxBR1'
    'NfTk9UX1JFQ09SRElORxABEhgKFFJFQURJTkdfRkxBR1NfTUFOVUFMEAI=');

@$core.Deprecated('Use conditionFlagsDescriptor instead')
const ConditionFlags$json = {
  '1': 'ConditionFlags',
  '2': [
    {'1': 'CONDITION_FLAGS_NONE', '2': 0},
    {'1': 'CONDITION_FLAGS_RECORDING', '2': 1},
  ],
};

/// Descriptor for `ConditionFlags`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List conditionFlagsDescriptor = $convert.base64Decode(
    'Cg5Db25kaXRpb25GbGFncxIYChRDT05ESVRJT05fRkxBR1NfTk9ORRAAEh0KGUNPTkRJVElPTl'
    '9GTEFHU19SRUNPUkRJTkcQAQ==');

@$core.Deprecated('Use signedRecordKindDescriptor instead')
const SignedRecordKind$json = {
  '1': 'SignedRecordKind',
  '2': [
    {'1': 'SIGNED_RECORD_KIND_NONE', '2': 0},
    {'1': 'SIGNED_RECORD_KIND_MODULES', '2': 1},
    {'1': 'SIGNED_RECORD_KIND_SCHEDULE', '2': 2},
    {'1': 'SIGNED_RECORD_KIND_STATE', '2': 3},
    {'1': 'SIGNED_RECORD_KIND_RAW_STATE', '2': 4},
    {'1': 'SIGNED_RECORD_KIND_FAULTS', '2': 5},
    {'1': 'SIGNED_RECORD_KIND_OTHER', '2': 255},
  ],
};

/// Descriptor for `SignedRecordKind`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List signedRecordKindDescriptor = $convert.base64Decode(
    'ChBTaWduZWRSZWNvcmRLaW5kEhsKF1NJR05FRF9SRUNPUkRfS0lORF9OT05FEAASHgoaU0lHTk'
    'VEX1JFQ09SRF9LSU5EX01PRFVMRVMQARIfChtTSUdORURfUkVDT1JEX0tJTkRfU0NIRURVTEUQ'
    'AhIcChhTSUdORURfUkVDT1JEX0tJTkRfU1RBVEUQAxIgChxTSUdORURfUkVDT1JEX0tJTkRfUk'
    'FXX1NUQVRFEAQSHQoZU0lHTkVEX1JFQ09SRF9LSU5EX0ZBVUxUUxAFEh0KGFNJR05FRF9SRUNP'
    'UkRfS0lORF9PVEhFUhD/AQ==');

@$core.Deprecated('Use curveTypeDescriptor instead')
const CurveType$json = {
  '1': 'CurveType',
  '2': [
    {'1': 'CURVE_NONE', '2': 0},
    {'1': 'CURVE_LINEAR', '2': 1},
    {'1': 'CURVE_POWER', '2': 2},
    {'1': 'CURVE_LOGARITHMIC', '2': 3},
    {'1': 'CURVE_EXPONENTIAL', '2': 4},
  ],
};

/// Descriptor for `CurveType`. Decode as a `google.protobuf.EnumDescriptorProto`.
final $typed_data.Uint8List curveTypeDescriptor = $convert.base64Decode(
    'CglDdXJ2ZVR5cGUSDgoKQ1VSVkVfTk9ORRAAEhAKDENVUlZFX0xJTkVBUhABEg8KC0NVUlZFX1'
    'BPV0VSEAISFQoRQ1VSVkVfTE9HQVJJVEhNSUMQAxIVChFDVVJWRV9FWFBPTkVOVElBTBAE');

@$core.Deprecated('Use deviceLocationDescriptor instead')
const DeviceLocation$json = {
  '1': 'DeviceLocation',
  '2': [
    {'1': 'enabled', '3': 7, '4': 1, '5': 13, '10': 'enabled'},
    {'1': 'fix', '3': 1, '4': 1, '5': 13, '10': 'fix'},
    {'1': 'time', '3': 2, '4': 1, '5': 3, '10': 'time'},
    {'1': 'longitude', '3': 3, '4': 1, '5': 2, '10': 'longitude'},
    {'1': 'latitude', '3': 4, '4': 1, '5': 2, '10': 'latitude'},
    {'1': 'altitude', '3': 5, '4': 1, '5': 2, '10': 'altitude'},
    {'1': 'coordinates', '3': 6, '4': 3, '5': 2, '10': 'coordinates'},
    {'1': 'satellites', '3': 8, '4': 1, '5': 13, '10': 'satellites'},
    {'1': 'hdop', '3': 9, '4': 1, '5': 13, '10': 'hdop'},
  ],
};

/// Descriptor for `DeviceLocation`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List deviceLocationDescriptor = $convert.base64Decode(
    'Cg5EZXZpY2VMb2NhdGlvbhIYCgdlbmFibGVkGAcgASgNUgdlbmFibGVkEhAKA2ZpeBgBIAEoDV'
    'IDZml4EhIKBHRpbWUYAiABKANSBHRpbWUSHAoJbG9uZ2l0dWRlGAMgASgCUglsb25naXR1ZGUS'
    'GgoIbGF0aXR1ZGUYBCABKAJSCGxhdGl0dWRlEhoKCGFsdGl0dWRlGAUgASgCUghhbHRpdHVkZR'
    'IgCgtjb29yZGluYXRlcxgGIAMoAlILY29vcmRpbmF0ZXMSHgoKc2F0ZWxsaXRlcxgIIAEoDVIK'
    'c2F0ZWxsaXRlcxISCgRoZG9wGAkgASgNUgRoZG9w');

@$core.Deprecated('Use sensorReadingDescriptor instead')
const SensorReading$json = {
  '1': 'SensorReading',
  '2': [
    {'1': 'reading', '3': 1, '4': 1, '5': 4, '10': 'reading'},
    {'1': 'time', '3': 2, '4': 1, '5': 3, '10': 'time'},
    {'1': 'sensor', '3': 3, '4': 1, '5': 13, '10': 'sensor'},
    {'1': 'value', '3': 4, '4': 1, '5': 2, '10': 'value'},
  ],
};

/// Descriptor for `SensorReading`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List sensorReadingDescriptor = $convert.base64Decode(
    'Cg1TZW5zb3JSZWFkaW5nEhgKB3JlYWRpbmcYASABKARSB3JlYWRpbmcSEgoEdGltZRgCIAEoA1'
    'IEdGltZRIWCgZzZW5zb3IYAyABKA1SBnNlbnNvchIUCgV2YWx1ZRgEIAEoAlIFdmFsdWU=');

@$core.Deprecated('Use loggedReadingDescriptor instead')
const LoggedReading$json = {
  '1': 'LoggedReading',
  '2': [
    {'1': 'version', '3': 1, '4': 1, '5': 13, '10': 'version'},
    {'1': 'location', '3': 2, '4': 1, '5': 11, '6': '.fk_data.DeviceLocation', '10': 'location'},
    {'1': 'reading', '3': 3, '4': 1, '5': 11, '6': '.fk_data.SensorReading', '10': 'reading'},
  ],
};

/// Descriptor for `LoggedReading`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List loggedReadingDescriptor = $convert.base64Decode(
    'Cg1Mb2dnZWRSZWFkaW5nEhgKB3ZlcnNpb24YASABKA1SB3ZlcnNpb24SMwoIbG9jYXRpb24YAi'
    'ABKAsyFy5ma19kYXRhLkRldmljZUxvY2F0aW9uUghsb2NhdGlvbhIwCgdyZWFkaW5nGAMgASgL'
    'MhYuZmtfZGF0YS5TZW5zb3JSZWFkaW5nUgdyZWFkaW5n');

@$core.Deprecated('Use sensorAndValueDescriptor instead')
const SensorAndValue$json = {
  '1': 'SensorAndValue',
  '2': [
    {'1': 'sensor', '3': 1, '4': 1, '5': 13, '10': 'sensor'},
    {'1': 'calibratedNull', '3': 4, '4': 1, '5': 8, '9': 0, '10': 'calibratedNull'},
    {'1': 'calibratedValue', '3': 2, '4': 1, '5': 2, '9': 0, '10': 'calibratedValue'},
    {'1': 'uncalibratedNull', '3': 5, '4': 1, '5': 8, '9': 1, '10': 'uncalibratedNull'},
    {'1': 'uncalibratedValue', '3': 3, '4': 1, '5': 2, '9': 1, '10': 'uncalibratedValue'},
  ],
  '8': [
    {'1': 'calibrated'},
    {'1': 'uncalibrated'},
  ],
};

/// Descriptor for `SensorAndValue`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List sensorAndValueDescriptor = $convert.base64Decode(
    'Cg5TZW5zb3JBbmRWYWx1ZRIWCgZzZW5zb3IYASABKA1SBnNlbnNvchIoCg5jYWxpYnJhdGVkTn'
    'VsbBgEIAEoCEgAUg5jYWxpYnJhdGVkTnVsbBIqCg9jYWxpYnJhdGVkVmFsdWUYAiABKAJIAFIP'
    'Y2FsaWJyYXRlZFZhbHVlEiwKEHVuY2FsaWJyYXRlZE51bGwYBSABKAhIAVIQdW5jYWxpYnJhdG'
    'VkTnVsbBIuChF1bmNhbGlicmF0ZWRWYWx1ZRgDIAEoAkgBUhF1bmNhbGlicmF0ZWRWYWx1ZUIM'
    'CgpjYWxpYnJhdGVkQg4KDHVuY2FsaWJyYXRlZA==');

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

@$core.Deprecated('Use moduleInfoDescriptor instead')
const ModuleInfo$json = {
  '1': 'ModuleInfo',
  '2': [
    {'1': 'position', '3': 1, '4': 1, '5': 13, '10': 'position'},
    {'1': 'address', '3': 2, '4': 1, '5': 13, '10': 'address'},
    {'1': 'name', '3': 3, '4': 1, '5': 9, '10': 'name'},
    {'1': 'header', '3': 4, '4': 1, '5': 11, '6': '.fk_data.ModuleHeader', '10': 'header'},
    {'1': 'firmware', '3': 5, '4': 1, '5': 11, '6': '.fk_data.Firmware', '10': 'firmware'},
    {'1': 'sensors', '3': 6, '4': 3, '5': 11, '6': '.fk_data.SensorInfo', '10': 'sensors'},
    {'1': 'id', '3': 7, '4': 1, '5': 12, '10': 'id'},
    {'1': 'flags', '3': 8, '4': 1, '5': 13, '10': 'flags'},
    {'1': 'configuration', '3': 9, '4': 1, '5': 12, '10': 'configuration'},
  ],
};

/// Descriptor for `ModuleInfo`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List moduleInfoDescriptor = $convert.base64Decode(
    'CgpNb2R1bGVJbmZvEhoKCHBvc2l0aW9uGAEgASgNUghwb3NpdGlvbhIYCgdhZGRyZXNzGAIgAS'
    'gNUgdhZGRyZXNzEhIKBG5hbWUYAyABKAlSBG5hbWUSLQoGaGVhZGVyGAQgASgLMhUuZmtfZGF0'
    'YS5Nb2R1bGVIZWFkZXJSBmhlYWRlchItCghmaXJtd2FyZRgFIAEoCzIRLmZrX2RhdGEuRmlybX'
    'dhcmVSCGZpcm13YXJlEi0KB3NlbnNvcnMYBiADKAsyEy5ma19kYXRhLlNlbnNvckluZm9SB3Nl'
    'bnNvcnMSDgoCaWQYByABKAxSAmlkEhQKBWZsYWdzGAggASgNUgVmbGFncxIkCg1jb25maWd1cm'
    'F0aW9uGAkgASgMUg1jb25maWd1cmF0aW9u');

@$core.Deprecated('Use sensorInfoDescriptor instead')
const SensorInfo$json = {
  '1': 'SensorInfo',
  '2': [
    {'1': 'number', '3': 1, '4': 1, '5': 13, '10': 'number'},
    {'1': 'name', '3': 2, '4': 1, '5': 9, '10': 'name'},
    {'1': 'unitOfMeasure', '3': 3, '4': 1, '5': 9, '10': 'unitOfMeasure'},
    {'1': 'uncalibratedUnitOfMeasure', '3': 5, '4': 1, '5': 9, '10': 'uncalibratedUnitOfMeasure'},
    {'1': 'flags', '3': 4, '4': 1, '5': 13, '10': 'flags'},
  ],
};

/// Descriptor for `SensorInfo`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List sensorInfoDescriptor = $convert.base64Decode(
    'CgpTZW5zb3JJbmZvEhYKBm51bWJlchgBIAEoDVIGbnVtYmVyEhIKBG5hbWUYAiABKAlSBG5hbW'
    'USJAoNdW5pdE9mTWVhc3VyZRgDIAEoCVINdW5pdE9mTWVhc3VyZRI8Chl1bmNhbGlicmF0ZWRV'
    'bml0T2ZNZWFzdXJlGAUgASgJUhl1bmNhbGlicmF0ZWRVbml0T2ZNZWFzdXJlEhQKBWZsYWdzGA'
    'QgASgNUgVmbGFncw==');

@$core.Deprecated('Use firmwareDescriptor instead')
const Firmware$json = {
  '1': 'Firmware',
  '2': [
    {'1': 'version', '3': 1, '4': 1, '5': 9, '10': 'version'},
    {'1': 'build', '3': 2, '4': 1, '5': 9, '10': 'build'},
    {'1': 'number', '3': 3, '4': 1, '5': 9, '10': 'number'},
    {'1': 'timestamp', '3': 4, '4': 1, '5': 4, '10': 'timestamp'},
    {'1': 'hash', '3': 5, '4': 1, '5': 9, '10': 'hash'},
  ],
};

/// Descriptor for `Firmware`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List firmwareDescriptor = $convert.base64Decode(
    'CghGaXJtd2FyZRIYCgd2ZXJzaW9uGAEgASgJUgd2ZXJzaW9uEhQKBWJ1aWxkGAIgASgJUgVidW'
    'lsZBIWCgZudW1iZXIYAyABKAlSBm51bWJlchIcCgl0aW1lc3RhbXAYBCABKARSCXRpbWVzdGFt'
    'cBISCgRoYXNoGAUgASgJUgRoYXNo');

@$core.Deprecated('Use metadataDescriptor instead')
const Metadata$json = {
  '1': 'Metadata',
  '2': [
    {'1': 'deviceId', '3': 1, '4': 1, '5': 12, '10': 'deviceId'},
    {'1': 'time', '3': 2, '4': 1, '5': 3, '10': 'time'},
    {'1': 'git', '3': 3, '4': 1, '5': 9, '10': 'git'},
    {'1': 'build', '3': 7, '4': 1, '5': 9, '10': 'build'},
    {'1': 'resetCause', '3': 4, '4': 1, '5': 13, '10': 'resetCause'},
    {'1': 'sensors', '3': 5, '4': 3, '5': 11, '6': '.fk_data.SensorInfo', '10': 'sensors'},
    {'1': 'modules', '3': 6, '4': 3, '5': 11, '6': '.fk_data.ModuleInfo', '10': 'modules'},
    {'1': 'firmware', '3': 8, '4': 1, '5': 11, '6': '.fk_data.Firmware', '10': 'firmware'},
    {'1': 'generation', '3': 9, '4': 1, '5': 12, '10': 'generation'},
    {'1': 'record', '3': 10, '4': 1, '5': 4, '10': 'record'},
  ],
};

/// Descriptor for `Metadata`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List metadataDescriptor = $convert.base64Decode(
    'CghNZXRhZGF0YRIaCghkZXZpY2VJZBgBIAEoDFIIZGV2aWNlSWQSEgoEdGltZRgCIAEoA1IEdG'
    'ltZRIQCgNnaXQYAyABKAlSA2dpdBIUCgVidWlsZBgHIAEoCVIFYnVpbGQSHgoKcmVzZXRDYXVz'
    'ZRgEIAEoDVIKcmVzZXRDYXVzZRItCgdzZW5zb3JzGAUgAygLMhMuZmtfZGF0YS5TZW5zb3JJbm'
    'ZvUgdzZW5zb3JzEi0KB21vZHVsZXMYBiADKAsyEy5ma19kYXRhLk1vZHVsZUluZm9SB21vZHVs'
    'ZXMSLQoIZmlybXdhcmUYCCABKAsyES5ma19kYXRhLkZpcm13YXJlUghmaXJtd2FyZRIeCgpnZW'
    '5lcmF0aW9uGAkgASgMUgpnZW5lcmF0aW9uEhYKBnJlY29yZBgKIAEoBFIGcmVjb3Jk');

@$core.Deprecated('Use statusDescriptor instead')
const Status$json = {
  '1': 'Status',
  '2': [
    {'1': 'time', '3': 1, '4': 1, '5': 3, '10': 'time'},
    {'1': 'uptime', '3': 2, '4': 1, '5': 13, '10': 'uptime'},
    {'1': 'battery', '3': 3, '4': 1, '5': 2, '10': 'battery'},
    {'1': 'memory', '3': 4, '4': 1, '5': 13, '10': 'memory'},
    {'1': 'busy', '3': 5, '4': 1, '5': 4, '10': 'busy'},
  ],
};

/// Descriptor for `Status`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List statusDescriptor = $convert.base64Decode(
    'CgZTdGF0dXMSEgoEdGltZRgBIAEoA1IEdGltZRIWCgZ1cHRpbWUYAiABKA1SBnVwdGltZRIYCg'
    'diYXR0ZXJ5GAMgASgCUgdiYXR0ZXJ5EhYKBm1lbW9yeRgEIAEoDVIGbWVtb3J5EhIKBGJ1c3kY'
    'BSABKARSBGJ1c3k=');

@$core.Deprecated('Use logMessageDescriptor instead')
const LogMessage$json = {
  '1': 'LogMessage',
  '2': [
    {'1': 'time', '3': 1, '4': 1, '5': 3, '10': 'time'},
    {'1': 'uptime', '3': 2, '4': 1, '5': 13, '10': 'uptime'},
    {'1': 'level', '3': 3, '4': 1, '5': 13, '10': 'level'},
    {'1': 'facility', '3': 4, '4': 1, '5': 9, '10': 'facility'},
    {'1': 'message', '3': 5, '4': 1, '5': 9, '10': 'message'},
  ],
};

/// Descriptor for `LogMessage`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List logMessageDescriptor = $convert.base64Decode(
    'CgpMb2dNZXNzYWdlEhIKBHRpbWUYASABKANSBHRpbWUSFgoGdXB0aW1lGAIgASgNUgZ1cHRpbW'
    'USFAoFbGV2ZWwYAyABKA1SBWxldmVsEhoKCGZhY2lsaXR5GAQgASgJUghmYWNpbGl0eRIYCgdt'
    'ZXNzYWdlGAUgASgJUgdtZXNzYWdl');

@$core.Deprecated('Use sensorGroupDescriptor instead')
const SensorGroup$json = {
  '1': 'SensorGroup',
  '2': [
    {'1': 'module', '3': 1, '4': 1, '5': 13, '10': 'module'},
    {'1': 'time', '3': 3, '4': 1, '5': 3, '10': 'time'},
    {'1': 'readings', '3': 2, '4': 3, '5': 11, '6': '.fk_data.SensorAndValue', '10': 'readings'},
  ],
};

/// Descriptor for `SensorGroup`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List sensorGroupDescriptor = $convert.base64Decode(
    'CgtTZW5zb3JHcm91cBIWCgZtb2R1bGUYASABKA1SBm1vZHVsZRISCgR0aW1lGAMgASgDUgR0aW'
    '1lEjMKCHJlYWRpbmdzGAIgAygLMhcuZmtfZGF0YS5TZW5zb3JBbmRWYWx1ZVIIcmVhZGluZ3M=');

@$core.Deprecated('Use readingsDescriptor instead')
const Readings$json = {
  '1': 'Readings',
  '2': [
    {'1': 'time', '3': 1, '4': 1, '5': 3, '10': 'time'},
    {'1': 'reading', '3': 2, '4': 1, '5': 4, '10': 'reading'},
    {'1': 'flags', '3': 3, '4': 1, '5': 13, '10': 'flags'},
    {'1': 'meta', '3': 6, '4': 1, '5': 4, '10': 'meta'},
    {'1': 'uptime', '3': 7, '4': 1, '5': 13, '10': 'uptime'},
    {'1': 'location', '3': 4, '4': 1, '5': 11, '6': '.fk_data.DeviceLocation', '10': 'location'},
    {'1': 'sensorGroups', '3': 5, '4': 3, '5': 11, '6': '.fk_data.SensorGroup', '10': 'sensorGroups'},
  ],
};

/// Descriptor for `Readings`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List readingsDescriptor = $convert.base64Decode(
    'CghSZWFkaW5ncxISCgR0aW1lGAEgASgDUgR0aW1lEhgKB3JlYWRpbmcYAiABKARSB3JlYWRpbm'
    'cSFAoFZmxhZ3MYAyABKA1SBWZsYWdzEhIKBG1ldGEYBiABKARSBG1ldGESFgoGdXB0aW1lGAcg'
    'ASgNUgZ1cHRpbWUSMwoIbG9jYXRpb24YBCABKAsyFy5ma19kYXRhLkRldmljZUxvY2F0aW9uUg'
    'hsb2NhdGlvbhI4CgxzZW5zb3JHcm91cHMYBSADKAsyFC5ma19kYXRhLlNlbnNvckdyb3VwUgxz'
    'ZW5zb3JHcm91cHM=');

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

@$core.Deprecated('Use jobScheduleDescriptor instead')
const JobSchedule$json = {
  '1': 'JobSchedule',
  '2': [
    {'1': 'cron', '3': 1, '4': 1, '5': 12, '10': 'cron'},
    {'1': 'interval', '3': 2, '4': 1, '5': 13, '10': 'interval'},
    {'1': 'repeated', '3': 3, '4': 1, '5': 13, '10': 'repeated'},
    {'1': 'duration', '3': 4, '4': 1, '5': 13, '10': 'duration'},
    {'1': 'jitter', '3': 5, '4': 1, '5': 13, '10': 'jitter'},
    {'1': 'intervals', '3': 6, '4': 3, '5': 11, '6': '.fk_data.Interval', '10': 'intervals'},
  ],
};

/// Descriptor for `JobSchedule`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List jobScheduleDescriptor = $convert.base64Decode(
    'CgtKb2JTY2hlZHVsZRISCgRjcm9uGAEgASgMUgRjcm9uEhoKCGludGVydmFsGAIgASgNUghpbn'
    'RlcnZhbBIaCghyZXBlYXRlZBgDIAEoDVIIcmVwZWF0ZWQSGgoIZHVyYXRpb24YBCABKA1SCGR1'
    'cmF0aW9uEhYKBmppdHRlchgFIAEoDVIGaml0dGVyEi8KCWludGVydmFscxgGIAMoCzIRLmZrX2'
    'RhdGEuSW50ZXJ2YWxSCWludGVydmFscw==');

@$core.Deprecated('Use scheduleDescriptor instead')
const Schedule$json = {
  '1': 'Schedule',
  '2': [
    {'1': 'readings', '3': 1, '4': 1, '5': 11, '6': '.fk_data.JobSchedule', '10': 'readings'},
    {'1': 'network', '3': 2, '4': 1, '5': 11, '6': '.fk_data.JobSchedule', '10': 'network'},
    {'1': 'lora', '3': 3, '4': 1, '5': 11, '6': '.fk_data.JobSchedule', '10': 'lora'},
    {'1': 'gps', '3': 4, '4': 1, '5': 11, '6': '.fk_data.JobSchedule', '10': 'gps'},
  ],
};

/// Descriptor for `Schedule`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List scheduleDescriptor = $convert.base64Decode(
    'CghTY2hlZHVsZRIwCghyZWFkaW5ncxgBIAEoCzIULmZrX2RhdGEuSm9iU2NoZWR1bGVSCHJlYW'
    'RpbmdzEi4KB25ldHdvcmsYAiABKAsyFC5ma19kYXRhLkpvYlNjaGVkdWxlUgduZXR3b3JrEigK'
    'BGxvcmEYAyABKAsyFC5ma19kYXRhLkpvYlNjaGVkdWxlUgRsb3JhEiYKA2dwcxgEIAEoCzIULm'
    'ZrX2RhdGEuSm9iU2NoZWR1bGVSA2dwcw==');

@$core.Deprecated('Use identityDescriptor instead')
const Identity$json = {
  '1': 'Identity',
  '2': [
    {'1': 'name', '3': 1, '4': 1, '5': 9, '10': 'name'},
  ],
};

/// Descriptor for `Identity`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List identityDescriptor = $convert.base64Decode(
    'CghJZGVudGl0eRISCgRuYW1lGAEgASgJUgRuYW1l');

@$core.Deprecated('Use conditionDescriptor instead')
const Condition$json = {
  '1': 'Condition',
  '2': [
    {'1': 'flags', '3': 1, '4': 1, '5': 13, '10': 'flags'},
    {'1': 'recording', '3': 2, '4': 1, '5': 13, '10': 'recording'},
  ],
};

/// Descriptor for `Condition`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List conditionDescriptor = $convert.base64Decode(
    'CglDb25kaXRpb24SFAoFZmxhZ3MYASABKA1SBWZsYWdzEhwKCXJlY29yZGluZxgCIAEoDVIJcm'
    'Vjb3JkaW5n');

@$core.Deprecated('Use networkInfoDescriptor instead')
const NetworkInfo$json = {
  '1': 'NetworkInfo',
  '2': [
    {'1': 'ssid', '3': 1, '4': 1, '5': 9, '10': 'ssid'},
    {'1': 'password', '3': 2, '4': 1, '5': 9, '10': 'password'},
    {'1': 'create', '3': 3, '4': 1, '5': 8, '10': 'create'},
    {'1': 'preferred', '3': 4, '4': 1, '5': 8, '10': 'preferred'},
  ],
};

/// Descriptor for `NetworkInfo`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List networkInfoDescriptor = $convert.base64Decode(
    'CgtOZXR3b3JrSW5mbxISCgRzc2lkGAEgASgJUgRzc2lkEhoKCHBhc3N3b3JkGAIgASgJUghwYX'
    'Nzd29yZBIWCgZjcmVhdGUYAyABKAhSBmNyZWF0ZRIcCglwcmVmZXJyZWQYBCABKAhSCXByZWZl'
    'cnJlZA==');

@$core.Deprecated('Use wifiTransmissionDescriptor instead')
const WifiTransmission$json = {
  '1': 'WifiTransmission',
  '2': [
    {'1': 'url', '3': 1, '4': 1, '5': 9, '10': 'url'},
    {'1': 'token', '3': 2, '4': 1, '5': 9, '10': 'token'},
    {'1': 'enabled', '3': 3, '4': 1, '5': 8, '10': 'enabled'},
  ],
};

/// Descriptor for `WifiTransmission`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List wifiTransmissionDescriptor = $convert.base64Decode(
    'ChBXaWZpVHJhbnNtaXNzaW9uEhAKA3VybBgBIAEoCVIDdXJsEhQKBXRva2VuGAIgASgJUgV0b2'
    'tlbhIYCgdlbmFibGVkGAMgASgIUgdlbmFibGVk');

@$core.Deprecated('Use transmissionSettingsDescriptor instead')
const TransmissionSettings$json = {
  '1': 'TransmissionSettings',
  '2': [
    {'1': 'wifi', '3': 1, '4': 1, '5': 11, '6': '.fk_data.WifiTransmission', '10': 'wifi'},
  ],
};

/// Descriptor for `TransmissionSettings`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List transmissionSettingsDescriptor = $convert.base64Decode(
    'ChRUcmFuc21pc3Npb25TZXR0aW5ncxItCgR3aWZpGAEgASgLMhkuZmtfZGF0YS5XaWZpVHJhbn'
    'NtaXNzaW9uUgR3aWZp');

@$core.Deprecated('Use networkSettingsDescriptor instead')
const NetworkSettings$json = {
  '1': 'NetworkSettings',
  '2': [
    {'1': 'networks', '3': 1, '4': 3, '5': 11, '6': '.fk_data.NetworkInfo', '10': 'networks'},
  ],
};

/// Descriptor for `NetworkSettings`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List networkSettingsDescriptor = $convert.base64Decode(
    'Cg9OZXR3b3JrU2V0dGluZ3MSMAoIbmV0d29ya3MYASADKAsyFC5ma19kYXRhLk5ldHdvcmtJbm'
    'ZvUghuZXR3b3Jrcw==');

@$core.Deprecated('Use loraSettingsDescriptor instead')
const LoraSettings$json = {
  '1': 'LoraSettings',
  '2': [
    {'1': 'deviceEui', '3': 1, '4': 1, '5': 12, '10': 'deviceEui'},
    {'1': 'appKey', '3': 2, '4': 1, '5': 12, '10': 'appKey'},
    {'1': 'joinEui', '3': 3, '4': 1, '5': 12, '10': 'joinEui'},
    {'1': 'frequencyBand', '3': 4, '4': 1, '5': 13, '10': 'frequencyBand'},
    {'1': 'deviceAddress', '3': 5, '4': 1, '5': 12, '10': 'deviceAddress'},
    {'1': 'networkSessionKey', '3': 6, '4': 1, '5': 12, '10': 'networkSessionKey'},
    {'1': 'appSessionKey', '3': 7, '4': 1, '5': 12, '10': 'appSessionKey'},
    {'1': 'uplinkCounter', '3': 8, '4': 1, '5': 13, '10': 'uplinkCounter'},
    {'1': 'downlinkCounter', '3': 9, '4': 1, '5': 13, '10': 'downlinkCounter'},
    {'1': 'rxDelay1', '3': 10, '4': 1, '5': 13, '10': 'rxDelay1'},
    {'1': 'rxDelay2', '3': 11, '4': 1, '5': 13, '10': 'rxDelay2'},
  ],
};

/// Descriptor for `LoraSettings`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List loraSettingsDescriptor = $convert.base64Decode(
    'CgxMb3JhU2V0dGluZ3MSHAoJZGV2aWNlRXVpGAEgASgMUglkZXZpY2VFdWkSFgoGYXBwS2V5GA'
    'IgASgMUgZhcHBLZXkSGAoHam9pbkV1aRgDIAEoDFIHam9pbkV1aRIkCg1mcmVxdWVuY3lCYW5k'
    'GAQgASgNUg1mcmVxdWVuY3lCYW5kEiQKDWRldmljZUFkZHJlc3MYBSABKAxSDWRldmljZUFkZH'
    'Jlc3MSLAoRbmV0d29ya1Nlc3Npb25LZXkYBiABKAxSEW5ldHdvcmtTZXNzaW9uS2V5EiQKDWFw'
    'cFNlc3Npb25LZXkYByABKAxSDWFwcFNlc3Npb25LZXkSJAoNdXBsaW5rQ291bnRlchgIIAEoDV'
    'INdXBsaW5rQ291bnRlchIoCg9kb3dubGlua0NvdW50ZXIYCSABKA1SD2Rvd25saW5rQ291bnRl'
    'chIaCghyeERlbGF5MRgKIAEoDVIIcnhEZWxheTESGgoIcnhEZWxheTIYCyABKA1SCHJ4RGVsYX'
    'ky');

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

@$core.Deprecated('Use dataRecordDescriptor instead')
const DataRecord$json = {
  '1': 'DataRecord',
  '2': [
    {'1': 'loggedReading', '3': 1, '4': 1, '5': 11, '6': '.fk_data.LoggedReading', '10': 'loggedReading'},
    {'1': 'metadata', '3': 2, '4': 1, '5': 11, '6': '.fk_data.Metadata', '10': 'metadata'},
    {'1': 'log', '3': 3, '4': 1, '5': 11, '6': '.fk_data.LogMessage', '10': 'log'},
    {'1': 'logs', '3': 13, '4': 3, '5': 11, '6': '.fk_data.LogMessage', '10': 'logs'},
    {'1': 'status', '3': 4, '4': 1, '5': 11, '6': '.fk_data.Status', '10': 'status'},
    {'1': 'readings', '3': 5, '4': 1, '5': 11, '6': '.fk_data.Readings', '10': 'readings'},
    {'1': 'modules', '3': 6, '4': 3, '5': 11, '6': '.fk_data.ModuleInfo', '10': 'modules'},
    {'1': 'schedule', '3': 7, '4': 1, '5': 11, '6': '.fk_data.Schedule', '10': 'schedule'},
    {'1': 'meta', '3': 8, '4': 1, '5': 4, '10': 'meta'},
    {'1': 'identity', '3': 9, '4': 1, '5': 11, '6': '.fk_data.Identity', '10': 'identity'},
    {'1': 'condition', '3': 10, '4': 1, '5': 11, '6': '.fk_data.Condition', '10': 'condition'},
    {'1': 'lora', '3': 11, '4': 1, '5': 11, '6': '.fk_data.LoraSettings', '10': 'lora'},
    {'1': 'network', '3': 12, '4': 1, '5': 11, '6': '.fk_data.NetworkSettings', '10': 'network'},
    {'1': 'transmission', '3': 14, '4': 1, '5': 11, '6': '.fk_data.TransmissionSettings', '10': 'transmission'},
    {'1': 'faults', '3': 15, '4': 3, '5': 11, '6': '.fk_data.Fault', '10': 'faults'},
  ],
};

/// Descriptor for `DataRecord`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List dataRecordDescriptor = $convert.base64Decode(
    'CgpEYXRhUmVjb3JkEjwKDWxvZ2dlZFJlYWRpbmcYASABKAsyFi5ma19kYXRhLkxvZ2dlZFJlYW'
    'RpbmdSDWxvZ2dlZFJlYWRpbmcSLQoIbWV0YWRhdGEYAiABKAsyES5ma19kYXRhLk1ldGFkYXRh'
    'UghtZXRhZGF0YRIlCgNsb2cYAyABKAsyEy5ma19kYXRhLkxvZ01lc3NhZ2VSA2xvZxInCgRsb2'
    'dzGA0gAygLMhMuZmtfZGF0YS5Mb2dNZXNzYWdlUgRsb2dzEicKBnN0YXR1cxgEIAEoCzIPLmZr'
    'X2RhdGEuU3RhdHVzUgZzdGF0dXMSLQoIcmVhZGluZ3MYBSABKAsyES5ma19kYXRhLlJlYWRpbm'
    'dzUghyZWFkaW5ncxItCgdtb2R1bGVzGAYgAygLMhMuZmtfZGF0YS5Nb2R1bGVJbmZvUgdtb2R1'
    'bGVzEi0KCHNjaGVkdWxlGAcgASgLMhEuZmtfZGF0YS5TY2hlZHVsZVIIc2NoZWR1bGUSEgoEbW'
    'V0YRgIIAEoBFIEbWV0YRItCghpZGVudGl0eRgJIAEoCzIRLmZrX2RhdGEuSWRlbnRpdHlSCGlk'
    'ZW50aXR5EjAKCWNvbmRpdGlvbhgKIAEoCzISLmZrX2RhdGEuQ29uZGl0aW9uUgljb25kaXRpb2'
    '4SKQoEbG9yYRgLIAEoCzIVLmZrX2RhdGEuTG9yYVNldHRpbmdzUgRsb3JhEjIKB25ldHdvcmsY'
    'DCABKAsyGC5ma19kYXRhLk5ldHdvcmtTZXR0aW5nc1IHbmV0d29yaxJBCgx0cmFuc21pc3Npb2'
    '4YDiABKAsyHS5ma19kYXRhLlRyYW5zbWlzc2lvblNldHRpbmdzUgx0cmFuc21pc3Npb24SJgoG'
    'ZmF1bHRzGA8gAygLMg4uZmtfZGF0YS5GYXVsdFIGZmF1bHRz');

@$core.Deprecated('Use signedRecordDescriptor instead')
const SignedRecord$json = {
  '1': 'SignedRecord',
  '2': [
    {'1': 'kind', '3': 1, '4': 1, '5': 14, '6': '.fk_data.SignedRecordKind', '10': 'kind'},
    {'1': 'time', '3': 2, '4': 1, '5': 3, '10': 'time'},
    {'1': 'data', '3': 3, '4': 1, '5': 12, '10': 'data'},
    {'1': 'hash', '3': 4, '4': 1, '5': 12, '10': 'hash'},
    {'1': 'record', '3': 5, '4': 1, '5': 4, '10': 'record'},
  ],
};

/// Descriptor for `SignedRecord`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List signedRecordDescriptor = $convert.base64Decode(
    'CgxTaWduZWRSZWNvcmQSLQoEa2luZBgBIAEoDjIZLmZrX2RhdGEuU2lnbmVkUmVjb3JkS2luZF'
    'IEa2luZBISCgR0aW1lGAIgASgDUgR0aW1lEhIKBGRhdGEYAyABKAxSBGRhdGESEgoEaGFzaBgE'
    'IAEoDFIEaGFzaBIWCgZyZWNvcmQYBSABKARSBnJlY29yZA==');

@$core.Deprecated('Use loraRecordDescriptor instead')
const LoraRecord$json = {
  '1': 'LoraRecord',
  '2': [
    {'1': 'deviceId', '3': 1, '4': 1, '5': 12, '10': 'deviceId'},
    {'1': 'time', '3': 2, '4': 1, '5': 3, '10': 'time'},
    {'1': 'number', '3': 3, '4': 1, '5': 4, '10': 'number'},
    {'1': 'module', '3': 4, '4': 1, '5': 13, '10': 'module'},
    {'1': 'sensor', '3': 5, '4': 1, '5': 4, '10': 'sensor'},
    {'1': 'values', '3': 6, '4': 3, '5': 2, '10': 'values'},
    {'1': 'data', '3': 7, '4': 1, '5': 12, '10': 'data'},
  ],
};

/// Descriptor for `LoraRecord`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List loraRecordDescriptor = $convert.base64Decode(
    'CgpMb3JhUmVjb3JkEhoKCGRldmljZUlkGAEgASgMUghkZXZpY2VJZBISCgR0aW1lGAIgASgDUg'
    'R0aW1lEhYKBm51bWJlchgDIAEoBFIGbnVtYmVyEhYKBm1vZHVsZRgEIAEoDVIGbW9kdWxlEhYK'
    'BnNlbnNvchgFIAEoBFIGc2Vuc29yEhYKBnZhbHVlcxgGIAMoAlIGdmFsdWVzEhIKBGRhdGEYBy'
    'ABKAxSBGRhdGE=');

@$core.Deprecated('Use calibrationPointDescriptor instead')
const CalibrationPoint$json = {
  '1': 'CalibrationPoint',
  '2': [
    {'1': 'references', '3': 1, '4': 3, '5': 2, '10': 'references'},
    {'1': 'uncalibrated', '3': 2, '4': 3, '5': 2, '10': 'uncalibrated'},
    {'1': 'factory', '3': 3, '4': 3, '5': 2, '10': 'factory'},
    {'1': 'adc', '3': 4, '4': 3, '5': 12, '10': 'adc'},
  ],
};

/// Descriptor for `CalibrationPoint`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List calibrationPointDescriptor = $convert.base64Decode(
    'ChBDYWxpYnJhdGlvblBvaW50Eh4KCnJlZmVyZW5jZXMYASADKAJSCnJlZmVyZW5jZXMSIgoMdW'
    '5jYWxpYnJhdGVkGAIgAygCUgx1bmNhbGlicmF0ZWQSGAoHZmFjdG9yeRgDIAMoAlIHZmFjdG9y'
    'eRIQCgNhZGMYBCADKAxSA2FkYw==');

@$core.Deprecated('Use calibrationCoefficientsDescriptor instead')
const CalibrationCoefficients$json = {
  '1': 'CalibrationCoefficients',
  '2': [
    {'1': 'values', '3': 1, '4': 3, '5': 2, '10': 'values'},
  ],
};

/// Descriptor for `CalibrationCoefficients`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List calibrationCoefficientsDescriptor = $convert.base64Decode(
    'ChdDYWxpYnJhdGlvbkNvZWZmaWNpZW50cxIWCgZ2YWx1ZXMYASADKAJSBnZhbHVlcw==');

@$core.Deprecated('Use calibrationDescriptor instead')
const Calibration$json = {
  '1': 'Calibration',
  '2': [
    {'1': 'type', '3': 1, '4': 1, '5': 14, '6': '.fk_data.CurveType', '10': 'type'},
    {'1': 'time', '3': 2, '4': 1, '5': 13, '10': 'time'},
    {'1': 'kind', '3': 6, '4': 1, '5': 13, '10': 'kind'},
    {'1': 'points', '3': 3, '4': 3, '5': 11, '6': '.fk_data.CalibrationPoint', '10': 'points'},
    {'1': 'coefficients', '3': 4, '4': 1, '5': 11, '6': '.fk_data.CalibrationCoefficients', '10': 'coefficients'},
    {'1': 'firmware', '3': 5, '4': 1, '5': 11, '6': '.fk_data.Firmware', '10': 'firmware'},
  ],
};

/// Descriptor for `Calibration`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List calibrationDescriptor = $convert.base64Decode(
    'CgtDYWxpYnJhdGlvbhImCgR0eXBlGAEgASgOMhIuZmtfZGF0YS5DdXJ2ZVR5cGVSBHR5cGUSEg'
    'oEdGltZRgCIAEoDVIEdGltZRISCgRraW5kGAYgASgNUgRraW5kEjEKBnBvaW50cxgDIAMoCzIZ'
    'LmZrX2RhdGEuQ2FsaWJyYXRpb25Qb2ludFIGcG9pbnRzEkQKDGNvZWZmaWNpZW50cxgEIAEoCz'
    'IgLmZrX2RhdGEuQ2FsaWJyYXRpb25Db2VmZmljaWVudHNSDGNvZWZmaWNpZW50cxItCghmaXJt'
    'd2FyZRgFIAEoCzIRLmZrX2RhdGEuRmlybXdhcmVSCGZpcm13YXJl');

@$core.Deprecated('Use moduleConfigurationDescriptor instead')
const ModuleConfiguration$json = {
  '1': 'ModuleConfiguration',
  '2': [
    {'1': 'calibration', '3': 1, '4': 1, '5': 11, '6': '.fk_data.Calibration', '10': 'calibration'},
    {'1': 'calibrations', '3': 2, '4': 3, '5': 11, '6': '.fk_data.Calibration', '10': 'calibrations'},
  ],
};

/// Descriptor for `ModuleConfiguration`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List moduleConfigurationDescriptor = $convert.base64Decode(
    'ChNNb2R1bGVDb25maWd1cmF0aW9uEjYKC2NhbGlicmF0aW9uGAEgASgLMhQuZmtfZGF0YS5DYW'
    'xpYnJhdGlvblILY2FsaWJyYXRpb24SOAoMY2FsaWJyYXRpb25zGAIgAygLMhQuZmtfZGF0YS5D'
    'YWxpYnJhdGlvblIMY2FsaWJyYXRpb25z');

