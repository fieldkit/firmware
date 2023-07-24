//
//  Generated code. Do not modify.
//  source: fk-data.proto
//
// @dart = 2.12

// ignore_for_file: annotate_overrides, camel_case_types
// ignore_for_file: constant_identifier_names, library_prefixes
// ignore_for_file: non_constant_identifier_names, prefer_final_fields
// ignore_for_file: unnecessary_import, unnecessary_this, unused_import

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'fk-data.pbenum.dart';

export 'fk-data.pbenum.dart';

class DeviceLocation extends $pb.GeneratedMessage {
  factory DeviceLocation() => create();
  DeviceLocation._() : super();
  factory DeviceLocation.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DeviceLocation.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'DeviceLocation', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..a<$core.int>(1, _omitFieldNames ? '' : 'fix', $pb.PbFieldType.OU3)
    ..aInt64(2, _omitFieldNames ? '' : 'time')
    ..a<$core.double>(3, _omitFieldNames ? '' : 'longitude', $pb.PbFieldType.OF)
    ..a<$core.double>(4, _omitFieldNames ? '' : 'latitude', $pb.PbFieldType.OF)
    ..a<$core.double>(5, _omitFieldNames ? '' : 'altitude', $pb.PbFieldType.OF)
    ..p<$core.double>(6, _omitFieldNames ? '' : 'coordinates', $pb.PbFieldType.KF)
    ..a<$core.int>(7, _omitFieldNames ? '' : 'enabled', $pb.PbFieldType.OU3)
    ..a<$core.int>(8, _omitFieldNames ? '' : 'satellites', $pb.PbFieldType.OU3)
    ..a<$core.int>(9, _omitFieldNames ? '' : 'hdop', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  DeviceLocation clone() => DeviceLocation()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  DeviceLocation copyWith(void Function(DeviceLocation) updates) => super.copyWith((message) => updates(message as DeviceLocation)) as DeviceLocation;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static DeviceLocation create() => DeviceLocation._();
  DeviceLocation createEmptyInstance() => create();
  static $pb.PbList<DeviceLocation> createRepeated() => $pb.PbList<DeviceLocation>();
  @$core.pragma('dart2js:noInline')
  static DeviceLocation getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DeviceLocation>(create);
  static DeviceLocation? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get fix => $_getIZ(0);
  @$pb.TagNumber(1)
  set fix($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasFix() => $_has(0);
  @$pb.TagNumber(1)
  void clearFix() => clearField(1);

  @$pb.TagNumber(2)
  $fixnum.Int64 get time => $_getI64(1);
  @$pb.TagNumber(2)
  set time($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasTime() => $_has(1);
  @$pb.TagNumber(2)
  void clearTime() => clearField(2);

  @$pb.TagNumber(3)
  $core.double get longitude => $_getN(2);
  @$pb.TagNumber(3)
  set longitude($core.double v) { $_setFloat(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasLongitude() => $_has(2);
  @$pb.TagNumber(3)
  void clearLongitude() => clearField(3);

  @$pb.TagNumber(4)
  $core.double get latitude => $_getN(3);
  @$pb.TagNumber(4)
  set latitude($core.double v) { $_setFloat(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasLatitude() => $_has(3);
  @$pb.TagNumber(4)
  void clearLatitude() => clearField(4);

  @$pb.TagNumber(5)
  $core.double get altitude => $_getN(4);
  @$pb.TagNumber(5)
  set altitude($core.double v) { $_setFloat(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasAltitude() => $_has(4);
  @$pb.TagNumber(5)
  void clearAltitude() => clearField(5);

  @$pb.TagNumber(6)
  $core.List<$core.double> get coordinates => $_getList(5);

  @$pb.TagNumber(7)
  $core.int get enabled => $_getIZ(6);
  @$pb.TagNumber(7)
  set enabled($core.int v) { $_setUnsignedInt32(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasEnabled() => $_has(6);
  @$pb.TagNumber(7)
  void clearEnabled() => clearField(7);

  @$pb.TagNumber(8)
  $core.int get satellites => $_getIZ(7);
  @$pb.TagNumber(8)
  set satellites($core.int v) { $_setUnsignedInt32(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasSatellites() => $_has(7);
  @$pb.TagNumber(8)
  void clearSatellites() => clearField(8);

  @$pb.TagNumber(9)
  $core.int get hdop => $_getIZ(8);
  @$pb.TagNumber(9)
  set hdop($core.int v) { $_setUnsignedInt32(8, v); }
  @$pb.TagNumber(9)
  $core.bool hasHdop() => $_has(8);
  @$pb.TagNumber(9)
  void clearHdop() => clearField(9);
}

class SensorReading extends $pb.GeneratedMessage {
  factory SensorReading() => create();
  SensorReading._() : super();
  factory SensorReading.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SensorReading.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SensorReading', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..a<$fixnum.Int64>(1, _omitFieldNames ? '' : 'reading', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aInt64(2, _omitFieldNames ? '' : 'time')
    ..a<$core.int>(3, _omitFieldNames ? '' : 'sensor', $pb.PbFieldType.OU3)
    ..a<$core.double>(4, _omitFieldNames ? '' : 'value', $pb.PbFieldType.OF)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  SensorReading clone() => SensorReading()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  SensorReading copyWith(void Function(SensorReading) updates) => super.copyWith((message) => updates(message as SensorReading)) as SensorReading;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static SensorReading create() => SensorReading._();
  SensorReading createEmptyInstance() => create();
  static $pb.PbList<SensorReading> createRepeated() => $pb.PbList<SensorReading>();
  @$core.pragma('dart2js:noInline')
  static SensorReading getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SensorReading>(create);
  static SensorReading? _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get reading => $_getI64(0);
  @$pb.TagNumber(1)
  set reading($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasReading() => $_has(0);
  @$pb.TagNumber(1)
  void clearReading() => clearField(1);

  @$pb.TagNumber(2)
  $fixnum.Int64 get time => $_getI64(1);
  @$pb.TagNumber(2)
  set time($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasTime() => $_has(1);
  @$pb.TagNumber(2)
  void clearTime() => clearField(2);

  @$pb.TagNumber(3)
  $core.int get sensor => $_getIZ(2);
  @$pb.TagNumber(3)
  set sensor($core.int v) { $_setUnsignedInt32(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasSensor() => $_has(2);
  @$pb.TagNumber(3)
  void clearSensor() => clearField(3);

  @$pb.TagNumber(4)
  $core.double get value => $_getN(3);
  @$pb.TagNumber(4)
  set value($core.double v) { $_setFloat(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasValue() => $_has(3);
  @$pb.TagNumber(4)
  void clearValue() => clearField(4);
}

class LoggedReading extends $pb.GeneratedMessage {
  factory LoggedReading() => create();
  LoggedReading._() : super();
  factory LoggedReading.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory LoggedReading.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'LoggedReading', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..a<$core.int>(1, _omitFieldNames ? '' : 'version', $pb.PbFieldType.OU3)
    ..aOM<DeviceLocation>(2, _omitFieldNames ? '' : 'location', subBuilder: DeviceLocation.create)
    ..aOM<SensorReading>(3, _omitFieldNames ? '' : 'reading', subBuilder: SensorReading.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  LoggedReading clone() => LoggedReading()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  LoggedReading copyWith(void Function(LoggedReading) updates) => super.copyWith((message) => updates(message as LoggedReading)) as LoggedReading;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static LoggedReading create() => LoggedReading._();
  LoggedReading createEmptyInstance() => create();
  static $pb.PbList<LoggedReading> createRepeated() => $pb.PbList<LoggedReading>();
  @$core.pragma('dart2js:noInline')
  static LoggedReading getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<LoggedReading>(create);
  static LoggedReading? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get version => $_getIZ(0);
  @$pb.TagNumber(1)
  set version($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasVersion() => $_has(0);
  @$pb.TagNumber(1)
  void clearVersion() => clearField(1);

  @$pb.TagNumber(2)
  DeviceLocation get location => $_getN(1);
  @$pb.TagNumber(2)
  set location(DeviceLocation v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasLocation() => $_has(1);
  @$pb.TagNumber(2)
  void clearLocation() => clearField(2);
  @$pb.TagNumber(2)
  DeviceLocation ensureLocation() => $_ensure(1);

  @$pb.TagNumber(3)
  SensorReading get reading => $_getN(2);
  @$pb.TagNumber(3)
  set reading(SensorReading v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasReading() => $_has(2);
  @$pb.TagNumber(3)
  void clearReading() => clearField(3);
  @$pb.TagNumber(3)
  SensorReading ensureReading() => $_ensure(2);
}

enum SensorAndValue_Calibrated {
  calibratedValue, 
  calibratedNull, 
  notSet
}

enum SensorAndValue_Uncalibrated {
  uncalibratedValue, 
  uncalibratedNull, 
  notSet
}

class SensorAndValue extends $pb.GeneratedMessage {
  factory SensorAndValue() => create();
  SensorAndValue._() : super();
  factory SensorAndValue.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SensorAndValue.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static const $core.Map<$core.int, SensorAndValue_Calibrated> _SensorAndValue_CalibratedByTag = {
    2 : SensorAndValue_Calibrated.calibratedValue,
    4 : SensorAndValue_Calibrated.calibratedNull,
    0 : SensorAndValue_Calibrated.notSet
  };
  static const $core.Map<$core.int, SensorAndValue_Uncalibrated> _SensorAndValue_UncalibratedByTag = {
    3 : SensorAndValue_Uncalibrated.uncalibratedValue,
    5 : SensorAndValue_Uncalibrated.uncalibratedNull,
    0 : SensorAndValue_Uncalibrated.notSet
  };
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SensorAndValue', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..oo(0, [2, 4])
    ..oo(1, [3, 5])
    ..a<$core.int>(1, _omitFieldNames ? '' : 'sensor', $pb.PbFieldType.OU3)
    ..a<$core.double>(2, _omitFieldNames ? '' : 'calibratedValue', $pb.PbFieldType.OF, protoName: 'calibratedValue')
    ..a<$core.double>(3, _omitFieldNames ? '' : 'uncalibratedValue', $pb.PbFieldType.OF, protoName: 'uncalibratedValue')
    ..aOB(4, _omitFieldNames ? '' : 'calibratedNull', protoName: 'calibratedNull')
    ..aOB(5, _omitFieldNames ? '' : 'uncalibratedNull', protoName: 'uncalibratedNull')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  SensorAndValue clone() => SensorAndValue()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  SensorAndValue copyWith(void Function(SensorAndValue) updates) => super.copyWith((message) => updates(message as SensorAndValue)) as SensorAndValue;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static SensorAndValue create() => SensorAndValue._();
  SensorAndValue createEmptyInstance() => create();
  static $pb.PbList<SensorAndValue> createRepeated() => $pb.PbList<SensorAndValue>();
  @$core.pragma('dart2js:noInline')
  static SensorAndValue getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SensorAndValue>(create);
  static SensorAndValue? _defaultInstance;

  SensorAndValue_Calibrated whichCalibrated() => _SensorAndValue_CalibratedByTag[$_whichOneof(0)]!;
  void clearCalibrated() => clearField($_whichOneof(0));

  SensorAndValue_Uncalibrated whichUncalibrated() => _SensorAndValue_UncalibratedByTag[$_whichOneof(1)]!;
  void clearUncalibrated() => clearField($_whichOneof(1));

  @$pb.TagNumber(1)
  $core.int get sensor => $_getIZ(0);
  @$pb.TagNumber(1)
  set sensor($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasSensor() => $_has(0);
  @$pb.TagNumber(1)
  void clearSensor() => clearField(1);

  @$pb.TagNumber(2)
  $core.double get calibratedValue => $_getN(1);
  @$pb.TagNumber(2)
  set calibratedValue($core.double v) { $_setFloat(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasCalibratedValue() => $_has(1);
  @$pb.TagNumber(2)
  void clearCalibratedValue() => clearField(2);

  @$pb.TagNumber(3)
  $core.double get uncalibratedValue => $_getN(2);
  @$pb.TagNumber(3)
  set uncalibratedValue($core.double v) { $_setFloat(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasUncalibratedValue() => $_has(2);
  @$pb.TagNumber(3)
  void clearUncalibratedValue() => clearField(3);

  @$pb.TagNumber(4)
  $core.bool get calibratedNull => $_getBF(3);
  @$pb.TagNumber(4)
  set calibratedNull($core.bool v) { $_setBool(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasCalibratedNull() => $_has(3);
  @$pb.TagNumber(4)
  void clearCalibratedNull() => clearField(4);

  @$pb.TagNumber(5)
  $core.bool get uncalibratedNull => $_getBF(4);
  @$pb.TagNumber(5)
  set uncalibratedNull($core.bool v) { $_setBool(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasUncalibratedNull() => $_has(4);
  @$pb.TagNumber(5)
  void clearUncalibratedNull() => clearField(5);
}

class ModuleHeader extends $pb.GeneratedMessage {
  factory ModuleHeader() => create();
  ModuleHeader._() : super();
  factory ModuleHeader.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory ModuleHeader.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'ModuleHeader', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..a<$core.int>(1, _omitFieldNames ? '' : 'manufacturer', $pb.PbFieldType.OU3)
    ..a<$core.int>(2, _omitFieldNames ? '' : 'kind', $pb.PbFieldType.OU3)
    ..a<$core.int>(3, _omitFieldNames ? '' : 'version', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  ModuleHeader clone() => ModuleHeader()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  ModuleHeader copyWith(void Function(ModuleHeader) updates) => super.copyWith((message) => updates(message as ModuleHeader)) as ModuleHeader;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static ModuleHeader create() => ModuleHeader._();
  ModuleHeader createEmptyInstance() => create();
  static $pb.PbList<ModuleHeader> createRepeated() => $pb.PbList<ModuleHeader>();
  @$core.pragma('dart2js:noInline')
  static ModuleHeader getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<ModuleHeader>(create);
  static ModuleHeader? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get manufacturer => $_getIZ(0);
  @$pb.TagNumber(1)
  set manufacturer($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasManufacturer() => $_has(0);
  @$pb.TagNumber(1)
  void clearManufacturer() => clearField(1);

  @$pb.TagNumber(2)
  $core.int get kind => $_getIZ(1);
  @$pb.TagNumber(2)
  set kind($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasKind() => $_has(1);
  @$pb.TagNumber(2)
  void clearKind() => clearField(2);

  @$pb.TagNumber(3)
  $core.int get version => $_getIZ(2);
  @$pb.TagNumber(3)
  set version($core.int v) { $_setUnsignedInt32(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasVersion() => $_has(2);
  @$pb.TagNumber(3)
  void clearVersion() => clearField(3);
}

class ModuleInfo extends $pb.GeneratedMessage {
  factory ModuleInfo() => create();
  ModuleInfo._() : super();
  factory ModuleInfo.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory ModuleInfo.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'ModuleInfo', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..a<$core.int>(1, _omitFieldNames ? '' : 'position', $pb.PbFieldType.OU3)
    ..a<$core.int>(2, _omitFieldNames ? '' : 'address', $pb.PbFieldType.OU3)
    ..aOS(3, _omitFieldNames ? '' : 'name')
    ..aOM<ModuleHeader>(4, _omitFieldNames ? '' : 'header', subBuilder: ModuleHeader.create)
    ..aOM<Firmware>(5, _omitFieldNames ? '' : 'firmware', subBuilder: Firmware.create)
    ..pc<SensorInfo>(6, _omitFieldNames ? '' : 'sensors', $pb.PbFieldType.PM, subBuilder: SensorInfo.create)
    ..a<$core.List<$core.int>>(7, _omitFieldNames ? '' : 'id', $pb.PbFieldType.OY)
    ..a<$core.int>(8, _omitFieldNames ? '' : 'flags', $pb.PbFieldType.OU3)
    ..a<$core.List<$core.int>>(9, _omitFieldNames ? '' : 'configuration', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  ModuleInfo clone() => ModuleInfo()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  ModuleInfo copyWith(void Function(ModuleInfo) updates) => super.copyWith((message) => updates(message as ModuleInfo)) as ModuleInfo;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static ModuleInfo create() => ModuleInfo._();
  ModuleInfo createEmptyInstance() => create();
  static $pb.PbList<ModuleInfo> createRepeated() => $pb.PbList<ModuleInfo>();
  @$core.pragma('dart2js:noInline')
  static ModuleInfo getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<ModuleInfo>(create);
  static ModuleInfo? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get position => $_getIZ(0);
  @$pb.TagNumber(1)
  set position($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasPosition() => $_has(0);
  @$pb.TagNumber(1)
  void clearPosition() => clearField(1);

  @$pb.TagNumber(2)
  $core.int get address => $_getIZ(1);
  @$pb.TagNumber(2)
  set address($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearAddress() => clearField(2);

  @$pb.TagNumber(3)
  $core.String get name => $_getSZ(2);
  @$pb.TagNumber(3)
  set name($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasName() => $_has(2);
  @$pb.TagNumber(3)
  void clearName() => clearField(3);

  @$pb.TagNumber(4)
  ModuleHeader get header => $_getN(3);
  @$pb.TagNumber(4)
  set header(ModuleHeader v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasHeader() => $_has(3);
  @$pb.TagNumber(4)
  void clearHeader() => clearField(4);
  @$pb.TagNumber(4)
  ModuleHeader ensureHeader() => $_ensure(3);

  @$pb.TagNumber(5)
  Firmware get firmware => $_getN(4);
  @$pb.TagNumber(5)
  set firmware(Firmware v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasFirmware() => $_has(4);
  @$pb.TagNumber(5)
  void clearFirmware() => clearField(5);
  @$pb.TagNumber(5)
  Firmware ensureFirmware() => $_ensure(4);

  @$pb.TagNumber(6)
  $core.List<SensorInfo> get sensors => $_getList(5);

  @$pb.TagNumber(7)
  $core.List<$core.int> get id => $_getN(6);
  @$pb.TagNumber(7)
  set id($core.List<$core.int> v) { $_setBytes(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasId() => $_has(6);
  @$pb.TagNumber(7)
  void clearId() => clearField(7);

  @$pb.TagNumber(8)
  $core.int get flags => $_getIZ(7);
  @$pb.TagNumber(8)
  set flags($core.int v) { $_setUnsignedInt32(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasFlags() => $_has(7);
  @$pb.TagNumber(8)
  void clearFlags() => clearField(8);

  @$pb.TagNumber(9)
  $core.List<$core.int> get configuration => $_getN(8);
  @$pb.TagNumber(9)
  set configuration($core.List<$core.int> v) { $_setBytes(8, v); }
  @$pb.TagNumber(9)
  $core.bool hasConfiguration() => $_has(8);
  @$pb.TagNumber(9)
  void clearConfiguration() => clearField(9);
}

class SensorInfo extends $pb.GeneratedMessage {
  factory SensorInfo() => create();
  SensorInfo._() : super();
  factory SensorInfo.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SensorInfo.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SensorInfo', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..a<$core.int>(1, _omitFieldNames ? '' : 'number', $pb.PbFieldType.OU3)
    ..aOS(2, _omitFieldNames ? '' : 'name')
    ..aOS(3, _omitFieldNames ? '' : 'unitOfMeasure', protoName: 'unitOfMeasure')
    ..a<$core.int>(4, _omitFieldNames ? '' : 'flags', $pb.PbFieldType.OU3)
    ..aOS(5, _omitFieldNames ? '' : 'uncalibratedUnitOfMeasure', protoName: 'uncalibratedUnitOfMeasure')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  SensorInfo clone() => SensorInfo()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  SensorInfo copyWith(void Function(SensorInfo) updates) => super.copyWith((message) => updates(message as SensorInfo)) as SensorInfo;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static SensorInfo create() => SensorInfo._();
  SensorInfo createEmptyInstance() => create();
  static $pb.PbList<SensorInfo> createRepeated() => $pb.PbList<SensorInfo>();
  @$core.pragma('dart2js:noInline')
  static SensorInfo getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SensorInfo>(create);
  static SensorInfo? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get number => $_getIZ(0);
  @$pb.TagNumber(1)
  set number($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasNumber() => $_has(0);
  @$pb.TagNumber(1)
  void clearNumber() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get name => $_getSZ(1);
  @$pb.TagNumber(2)
  set name($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasName() => $_has(1);
  @$pb.TagNumber(2)
  void clearName() => clearField(2);

  @$pb.TagNumber(3)
  $core.String get unitOfMeasure => $_getSZ(2);
  @$pb.TagNumber(3)
  set unitOfMeasure($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasUnitOfMeasure() => $_has(2);
  @$pb.TagNumber(3)
  void clearUnitOfMeasure() => clearField(3);

  @$pb.TagNumber(4)
  $core.int get flags => $_getIZ(3);
  @$pb.TagNumber(4)
  set flags($core.int v) { $_setUnsignedInt32(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasFlags() => $_has(3);
  @$pb.TagNumber(4)
  void clearFlags() => clearField(4);

  @$pb.TagNumber(5)
  $core.String get uncalibratedUnitOfMeasure => $_getSZ(4);
  @$pb.TagNumber(5)
  set uncalibratedUnitOfMeasure($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasUncalibratedUnitOfMeasure() => $_has(4);
  @$pb.TagNumber(5)
  void clearUncalibratedUnitOfMeasure() => clearField(5);
}

class Firmware extends $pb.GeneratedMessage {
  factory Firmware() => create();
  Firmware._() : super();
  factory Firmware.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Firmware.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Firmware', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'version')
    ..aOS(2, _omitFieldNames ? '' : 'build')
    ..aOS(3, _omitFieldNames ? '' : 'number')
    ..a<$fixnum.Int64>(4, _omitFieldNames ? '' : 'timestamp', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOS(5, _omitFieldNames ? '' : 'hash')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Firmware clone() => Firmware()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Firmware copyWith(void Function(Firmware) updates) => super.copyWith((message) => updates(message as Firmware)) as Firmware;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Firmware create() => Firmware._();
  Firmware createEmptyInstance() => create();
  static $pb.PbList<Firmware> createRepeated() => $pb.PbList<Firmware>();
  @$core.pragma('dart2js:noInline')
  static Firmware getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Firmware>(create);
  static Firmware? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get version => $_getSZ(0);
  @$pb.TagNumber(1)
  set version($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasVersion() => $_has(0);
  @$pb.TagNumber(1)
  void clearVersion() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get build => $_getSZ(1);
  @$pb.TagNumber(2)
  set build($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasBuild() => $_has(1);
  @$pb.TagNumber(2)
  void clearBuild() => clearField(2);

  @$pb.TagNumber(3)
  $core.String get number => $_getSZ(2);
  @$pb.TagNumber(3)
  set number($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasNumber() => $_has(2);
  @$pb.TagNumber(3)
  void clearNumber() => clearField(3);

  @$pb.TagNumber(4)
  $fixnum.Int64 get timestamp => $_getI64(3);
  @$pb.TagNumber(4)
  set timestamp($fixnum.Int64 v) { $_setInt64(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasTimestamp() => $_has(3);
  @$pb.TagNumber(4)
  void clearTimestamp() => clearField(4);

  @$pb.TagNumber(5)
  $core.String get hash => $_getSZ(4);
  @$pb.TagNumber(5)
  set hash($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasHash() => $_has(4);
  @$pb.TagNumber(5)
  void clearHash() => clearField(5);
}

class Metadata extends $pb.GeneratedMessage {
  factory Metadata() => create();
  Metadata._() : super();
  factory Metadata.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Metadata.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Metadata', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'deviceId', $pb.PbFieldType.OY, protoName: 'deviceId')
    ..aInt64(2, _omitFieldNames ? '' : 'time')
    ..aOS(3, _omitFieldNames ? '' : 'git')
    ..a<$core.int>(4, _omitFieldNames ? '' : 'resetCause', $pb.PbFieldType.OU3, protoName: 'resetCause')
    ..pc<SensorInfo>(5, _omitFieldNames ? '' : 'sensors', $pb.PbFieldType.PM, subBuilder: SensorInfo.create)
    ..pc<ModuleInfo>(6, _omitFieldNames ? '' : 'modules', $pb.PbFieldType.PM, subBuilder: ModuleInfo.create)
    ..aOS(7, _omitFieldNames ? '' : 'build')
    ..aOM<Firmware>(8, _omitFieldNames ? '' : 'firmware', subBuilder: Firmware.create)
    ..a<$core.List<$core.int>>(9, _omitFieldNames ? '' : 'generation', $pb.PbFieldType.OY)
    ..a<$fixnum.Int64>(10, _omitFieldNames ? '' : 'record', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Metadata clone() => Metadata()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Metadata copyWith(void Function(Metadata) updates) => super.copyWith((message) => updates(message as Metadata)) as Metadata;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Metadata create() => Metadata._();
  Metadata createEmptyInstance() => create();
  static $pb.PbList<Metadata> createRepeated() => $pb.PbList<Metadata>();
  @$core.pragma('dart2js:noInline')
  static Metadata getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Metadata>(create);
  static Metadata? _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$core.int> get deviceId => $_getN(0);
  @$pb.TagNumber(1)
  set deviceId($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasDeviceId() => $_has(0);
  @$pb.TagNumber(1)
  void clearDeviceId() => clearField(1);

  @$pb.TagNumber(2)
  $fixnum.Int64 get time => $_getI64(1);
  @$pb.TagNumber(2)
  set time($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasTime() => $_has(1);
  @$pb.TagNumber(2)
  void clearTime() => clearField(2);

  @$pb.TagNumber(3)
  $core.String get git => $_getSZ(2);
  @$pb.TagNumber(3)
  set git($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasGit() => $_has(2);
  @$pb.TagNumber(3)
  void clearGit() => clearField(3);

  @$pb.TagNumber(4)
  $core.int get resetCause => $_getIZ(3);
  @$pb.TagNumber(4)
  set resetCause($core.int v) { $_setUnsignedInt32(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasResetCause() => $_has(3);
  @$pb.TagNumber(4)
  void clearResetCause() => clearField(4);

  @$pb.TagNumber(5)
  $core.List<SensorInfo> get sensors => $_getList(4);

  @$pb.TagNumber(6)
  $core.List<ModuleInfo> get modules => $_getList(5);

  @$pb.TagNumber(7)
  $core.String get build => $_getSZ(6);
  @$pb.TagNumber(7)
  set build($core.String v) { $_setString(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasBuild() => $_has(6);
  @$pb.TagNumber(7)
  void clearBuild() => clearField(7);

  @$pb.TagNumber(8)
  Firmware get firmware => $_getN(7);
  @$pb.TagNumber(8)
  set firmware(Firmware v) { setField(8, v); }
  @$pb.TagNumber(8)
  $core.bool hasFirmware() => $_has(7);
  @$pb.TagNumber(8)
  void clearFirmware() => clearField(8);
  @$pb.TagNumber(8)
  Firmware ensureFirmware() => $_ensure(7);

  @$pb.TagNumber(9)
  $core.List<$core.int> get generation => $_getN(8);
  @$pb.TagNumber(9)
  set generation($core.List<$core.int> v) { $_setBytes(8, v); }
  @$pb.TagNumber(9)
  $core.bool hasGeneration() => $_has(8);
  @$pb.TagNumber(9)
  void clearGeneration() => clearField(9);

  @$pb.TagNumber(10)
  $fixnum.Int64 get record => $_getI64(9);
  @$pb.TagNumber(10)
  set record($fixnum.Int64 v) { $_setInt64(9, v); }
  @$pb.TagNumber(10)
  $core.bool hasRecord() => $_has(9);
  @$pb.TagNumber(10)
  void clearRecord() => clearField(10);
}

class Status extends $pb.GeneratedMessage {
  factory Status() => create();
  Status._() : super();
  factory Status.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Status.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Status', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..aInt64(1, _omitFieldNames ? '' : 'time')
    ..a<$core.int>(2, _omitFieldNames ? '' : 'uptime', $pb.PbFieldType.OU3)
    ..a<$core.double>(3, _omitFieldNames ? '' : 'battery', $pb.PbFieldType.OF)
    ..a<$core.int>(4, _omitFieldNames ? '' : 'memory', $pb.PbFieldType.OU3)
    ..a<$fixnum.Int64>(5, _omitFieldNames ? '' : 'busy', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Status clone() => Status()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Status copyWith(void Function(Status) updates) => super.copyWith((message) => updates(message as Status)) as Status;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Status create() => Status._();
  Status createEmptyInstance() => create();
  static $pb.PbList<Status> createRepeated() => $pb.PbList<Status>();
  @$core.pragma('dart2js:noInline')
  static Status getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Status>(create);
  static Status? _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get time => $_getI64(0);
  @$pb.TagNumber(1)
  set time($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasTime() => $_has(0);
  @$pb.TagNumber(1)
  void clearTime() => clearField(1);

  @$pb.TagNumber(2)
  $core.int get uptime => $_getIZ(1);
  @$pb.TagNumber(2)
  set uptime($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasUptime() => $_has(1);
  @$pb.TagNumber(2)
  void clearUptime() => clearField(2);

  @$pb.TagNumber(3)
  $core.double get battery => $_getN(2);
  @$pb.TagNumber(3)
  set battery($core.double v) { $_setFloat(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasBattery() => $_has(2);
  @$pb.TagNumber(3)
  void clearBattery() => clearField(3);

  @$pb.TagNumber(4)
  $core.int get memory => $_getIZ(3);
  @$pb.TagNumber(4)
  set memory($core.int v) { $_setUnsignedInt32(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasMemory() => $_has(3);
  @$pb.TagNumber(4)
  void clearMemory() => clearField(4);

  @$pb.TagNumber(5)
  $fixnum.Int64 get busy => $_getI64(4);
  @$pb.TagNumber(5)
  set busy($fixnum.Int64 v) { $_setInt64(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasBusy() => $_has(4);
  @$pb.TagNumber(5)
  void clearBusy() => clearField(5);
}

class LogMessage extends $pb.GeneratedMessage {
  factory LogMessage() => create();
  LogMessage._() : super();
  factory LogMessage.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory LogMessage.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'LogMessage', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..aInt64(1, _omitFieldNames ? '' : 'time')
    ..a<$core.int>(2, _omitFieldNames ? '' : 'uptime', $pb.PbFieldType.OU3)
    ..a<$core.int>(3, _omitFieldNames ? '' : 'level', $pb.PbFieldType.OU3)
    ..aOS(4, _omitFieldNames ? '' : 'facility')
    ..aOS(5, _omitFieldNames ? '' : 'message')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  LogMessage clone() => LogMessage()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  LogMessage copyWith(void Function(LogMessage) updates) => super.copyWith((message) => updates(message as LogMessage)) as LogMessage;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static LogMessage create() => LogMessage._();
  LogMessage createEmptyInstance() => create();
  static $pb.PbList<LogMessage> createRepeated() => $pb.PbList<LogMessage>();
  @$core.pragma('dart2js:noInline')
  static LogMessage getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<LogMessage>(create);
  static LogMessage? _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get time => $_getI64(0);
  @$pb.TagNumber(1)
  set time($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasTime() => $_has(0);
  @$pb.TagNumber(1)
  void clearTime() => clearField(1);

  @$pb.TagNumber(2)
  $core.int get uptime => $_getIZ(1);
  @$pb.TagNumber(2)
  set uptime($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasUptime() => $_has(1);
  @$pb.TagNumber(2)
  void clearUptime() => clearField(2);

  @$pb.TagNumber(3)
  $core.int get level => $_getIZ(2);
  @$pb.TagNumber(3)
  set level($core.int v) { $_setUnsignedInt32(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasLevel() => $_has(2);
  @$pb.TagNumber(3)
  void clearLevel() => clearField(3);

  @$pb.TagNumber(4)
  $core.String get facility => $_getSZ(3);
  @$pb.TagNumber(4)
  set facility($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasFacility() => $_has(3);
  @$pb.TagNumber(4)
  void clearFacility() => clearField(4);

  @$pb.TagNumber(5)
  $core.String get message => $_getSZ(4);
  @$pb.TagNumber(5)
  set message($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasMessage() => $_has(4);
  @$pb.TagNumber(5)
  void clearMessage() => clearField(5);
}

class SensorGroup extends $pb.GeneratedMessage {
  factory SensorGroup() => create();
  SensorGroup._() : super();
  factory SensorGroup.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SensorGroup.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SensorGroup', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..a<$core.int>(1, _omitFieldNames ? '' : 'module', $pb.PbFieldType.OU3)
    ..pc<SensorAndValue>(2, _omitFieldNames ? '' : 'readings', $pb.PbFieldType.PM, subBuilder: SensorAndValue.create)
    ..aInt64(3, _omitFieldNames ? '' : 'time')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  SensorGroup clone() => SensorGroup()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  SensorGroup copyWith(void Function(SensorGroup) updates) => super.copyWith((message) => updates(message as SensorGroup)) as SensorGroup;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static SensorGroup create() => SensorGroup._();
  SensorGroup createEmptyInstance() => create();
  static $pb.PbList<SensorGroup> createRepeated() => $pb.PbList<SensorGroup>();
  @$core.pragma('dart2js:noInline')
  static SensorGroup getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SensorGroup>(create);
  static SensorGroup? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get module => $_getIZ(0);
  @$pb.TagNumber(1)
  set module($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasModule() => $_has(0);
  @$pb.TagNumber(1)
  void clearModule() => clearField(1);

  @$pb.TagNumber(2)
  $core.List<SensorAndValue> get readings => $_getList(1);

  @$pb.TagNumber(3)
  $fixnum.Int64 get time => $_getI64(2);
  @$pb.TagNumber(3)
  set time($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasTime() => $_has(2);
  @$pb.TagNumber(3)
  void clearTime() => clearField(3);
}

class Readings extends $pb.GeneratedMessage {
  factory Readings() => create();
  Readings._() : super();
  factory Readings.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Readings.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Readings', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..aInt64(1, _omitFieldNames ? '' : 'time')
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'reading', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.int>(3, _omitFieldNames ? '' : 'flags', $pb.PbFieldType.OU3)
    ..aOM<DeviceLocation>(4, _omitFieldNames ? '' : 'location', subBuilder: DeviceLocation.create)
    ..pc<SensorGroup>(5, _omitFieldNames ? '' : 'sensorGroups', $pb.PbFieldType.PM, protoName: 'sensorGroups', subBuilder: SensorGroup.create)
    ..a<$fixnum.Int64>(6, _omitFieldNames ? '' : 'meta', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.int>(7, _omitFieldNames ? '' : 'uptime', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Readings clone() => Readings()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Readings copyWith(void Function(Readings) updates) => super.copyWith((message) => updates(message as Readings)) as Readings;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Readings create() => Readings._();
  Readings createEmptyInstance() => create();
  static $pb.PbList<Readings> createRepeated() => $pb.PbList<Readings>();
  @$core.pragma('dart2js:noInline')
  static Readings getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Readings>(create);
  static Readings? _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get time => $_getI64(0);
  @$pb.TagNumber(1)
  set time($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasTime() => $_has(0);
  @$pb.TagNumber(1)
  void clearTime() => clearField(1);

  @$pb.TagNumber(2)
  $fixnum.Int64 get reading => $_getI64(1);
  @$pb.TagNumber(2)
  set reading($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasReading() => $_has(1);
  @$pb.TagNumber(2)
  void clearReading() => clearField(2);

  @$pb.TagNumber(3)
  $core.int get flags => $_getIZ(2);
  @$pb.TagNumber(3)
  set flags($core.int v) { $_setUnsignedInt32(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasFlags() => $_has(2);
  @$pb.TagNumber(3)
  void clearFlags() => clearField(3);

  @$pb.TagNumber(4)
  DeviceLocation get location => $_getN(3);
  @$pb.TagNumber(4)
  set location(DeviceLocation v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasLocation() => $_has(3);
  @$pb.TagNumber(4)
  void clearLocation() => clearField(4);
  @$pb.TagNumber(4)
  DeviceLocation ensureLocation() => $_ensure(3);

  @$pb.TagNumber(5)
  $core.List<SensorGroup> get sensorGroups => $_getList(4);

  @$pb.TagNumber(6)
  $fixnum.Int64 get meta => $_getI64(5);
  @$pb.TagNumber(6)
  set meta($fixnum.Int64 v) { $_setInt64(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasMeta() => $_has(5);
  @$pb.TagNumber(6)
  void clearMeta() => clearField(6);

  @$pb.TagNumber(7)
  $core.int get uptime => $_getIZ(6);
  @$pb.TagNumber(7)
  set uptime($core.int v) { $_setUnsignedInt32(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasUptime() => $_has(6);
  @$pb.TagNumber(7)
  void clearUptime() => clearField(7);
}

class Interval extends $pb.GeneratedMessage {
  factory Interval() => create();
  Interval._() : super();
  factory Interval.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Interval.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Interval', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..a<$fixnum.Int64>(1, _omitFieldNames ? '' : 'start', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$fixnum.Int64>(2, _omitFieldNames ? '' : 'end', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.int>(3, _omitFieldNames ? '' : 'interval', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Interval clone() => Interval()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Interval copyWith(void Function(Interval) updates) => super.copyWith((message) => updates(message as Interval)) as Interval;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Interval create() => Interval._();
  Interval createEmptyInstance() => create();
  static $pb.PbList<Interval> createRepeated() => $pb.PbList<Interval>();
  @$core.pragma('dart2js:noInline')
  static Interval getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Interval>(create);
  static Interval? _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get start => $_getI64(0);
  @$pb.TagNumber(1)
  set start($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasStart() => $_has(0);
  @$pb.TagNumber(1)
  void clearStart() => clearField(1);

  @$pb.TagNumber(2)
  $fixnum.Int64 get end => $_getI64(1);
  @$pb.TagNumber(2)
  set end($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasEnd() => $_has(1);
  @$pb.TagNumber(2)
  void clearEnd() => clearField(2);

  @$pb.TagNumber(3)
  $core.int get interval => $_getIZ(2);
  @$pb.TagNumber(3)
  set interval($core.int v) { $_setUnsignedInt32(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasInterval() => $_has(2);
  @$pb.TagNumber(3)
  void clearInterval() => clearField(3);
}

class JobSchedule extends $pb.GeneratedMessage {
  factory JobSchedule() => create();
  JobSchedule._() : super();
  factory JobSchedule.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory JobSchedule.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'JobSchedule', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'cron', $pb.PbFieldType.OY)
    ..a<$core.int>(2, _omitFieldNames ? '' : 'interval', $pb.PbFieldType.OU3)
    ..a<$core.int>(3, _omitFieldNames ? '' : 'repeated', $pb.PbFieldType.OU3)
    ..a<$core.int>(4, _omitFieldNames ? '' : 'duration', $pb.PbFieldType.OU3)
    ..a<$core.int>(5, _omitFieldNames ? '' : 'jitter', $pb.PbFieldType.OU3)
    ..pc<Interval>(6, _omitFieldNames ? '' : 'intervals', $pb.PbFieldType.PM, subBuilder: Interval.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  JobSchedule clone() => JobSchedule()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  JobSchedule copyWith(void Function(JobSchedule) updates) => super.copyWith((message) => updates(message as JobSchedule)) as JobSchedule;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static JobSchedule create() => JobSchedule._();
  JobSchedule createEmptyInstance() => create();
  static $pb.PbList<JobSchedule> createRepeated() => $pb.PbList<JobSchedule>();
  @$core.pragma('dart2js:noInline')
  static JobSchedule getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<JobSchedule>(create);
  static JobSchedule? _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$core.int> get cron => $_getN(0);
  @$pb.TagNumber(1)
  set cron($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasCron() => $_has(0);
  @$pb.TagNumber(1)
  void clearCron() => clearField(1);

  @$pb.TagNumber(2)
  $core.int get interval => $_getIZ(1);
  @$pb.TagNumber(2)
  set interval($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasInterval() => $_has(1);
  @$pb.TagNumber(2)
  void clearInterval() => clearField(2);

  @$pb.TagNumber(3)
  $core.int get repeated => $_getIZ(2);
  @$pb.TagNumber(3)
  set repeated($core.int v) { $_setUnsignedInt32(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasRepeated() => $_has(2);
  @$pb.TagNumber(3)
  void clearRepeated() => clearField(3);

  @$pb.TagNumber(4)
  $core.int get duration => $_getIZ(3);
  @$pb.TagNumber(4)
  set duration($core.int v) { $_setUnsignedInt32(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasDuration() => $_has(3);
  @$pb.TagNumber(4)
  void clearDuration() => clearField(4);

  @$pb.TagNumber(5)
  $core.int get jitter => $_getIZ(4);
  @$pb.TagNumber(5)
  set jitter($core.int v) { $_setUnsignedInt32(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasJitter() => $_has(4);
  @$pb.TagNumber(5)
  void clearJitter() => clearField(5);

  @$pb.TagNumber(6)
  $core.List<Interval> get intervals => $_getList(5);
}

class Schedule extends $pb.GeneratedMessage {
  factory Schedule() => create();
  Schedule._() : super();
  factory Schedule.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Schedule.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Schedule', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..aOM<JobSchedule>(1, _omitFieldNames ? '' : 'readings', subBuilder: JobSchedule.create)
    ..aOM<JobSchedule>(2, _omitFieldNames ? '' : 'network', subBuilder: JobSchedule.create)
    ..aOM<JobSchedule>(3, _omitFieldNames ? '' : 'lora', subBuilder: JobSchedule.create)
    ..aOM<JobSchedule>(4, _omitFieldNames ? '' : 'gps', subBuilder: JobSchedule.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Schedule clone() => Schedule()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Schedule copyWith(void Function(Schedule) updates) => super.copyWith((message) => updates(message as Schedule)) as Schedule;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Schedule create() => Schedule._();
  Schedule createEmptyInstance() => create();
  static $pb.PbList<Schedule> createRepeated() => $pb.PbList<Schedule>();
  @$core.pragma('dart2js:noInline')
  static Schedule getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Schedule>(create);
  static Schedule? _defaultInstance;

  @$pb.TagNumber(1)
  JobSchedule get readings => $_getN(0);
  @$pb.TagNumber(1)
  set readings(JobSchedule v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasReadings() => $_has(0);
  @$pb.TagNumber(1)
  void clearReadings() => clearField(1);
  @$pb.TagNumber(1)
  JobSchedule ensureReadings() => $_ensure(0);

  @$pb.TagNumber(2)
  JobSchedule get network => $_getN(1);
  @$pb.TagNumber(2)
  set network(JobSchedule v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasNetwork() => $_has(1);
  @$pb.TagNumber(2)
  void clearNetwork() => clearField(2);
  @$pb.TagNumber(2)
  JobSchedule ensureNetwork() => $_ensure(1);

  @$pb.TagNumber(3)
  JobSchedule get lora => $_getN(2);
  @$pb.TagNumber(3)
  set lora(JobSchedule v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasLora() => $_has(2);
  @$pb.TagNumber(3)
  void clearLora() => clearField(3);
  @$pb.TagNumber(3)
  JobSchedule ensureLora() => $_ensure(2);

  @$pb.TagNumber(4)
  JobSchedule get gps => $_getN(3);
  @$pb.TagNumber(4)
  set gps(JobSchedule v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasGps() => $_has(3);
  @$pb.TagNumber(4)
  void clearGps() => clearField(4);
  @$pb.TagNumber(4)
  JobSchedule ensureGps() => $_ensure(3);
}

class Identity extends $pb.GeneratedMessage {
  factory Identity() => create();
  Identity._() : super();
  factory Identity.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Identity.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Identity', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'name')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Identity clone() => Identity()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Identity copyWith(void Function(Identity) updates) => super.copyWith((message) => updates(message as Identity)) as Identity;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Identity create() => Identity._();
  Identity createEmptyInstance() => create();
  static $pb.PbList<Identity> createRepeated() => $pb.PbList<Identity>();
  @$core.pragma('dart2js:noInline')
  static Identity getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Identity>(create);
  static Identity? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get name => $_getSZ(0);
  @$pb.TagNumber(1)
  set name($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasName() => $_has(0);
  @$pb.TagNumber(1)
  void clearName() => clearField(1);
}

class Condition extends $pb.GeneratedMessage {
  factory Condition() => create();
  Condition._() : super();
  factory Condition.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Condition.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Condition', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..a<$core.int>(1, _omitFieldNames ? '' : 'flags', $pb.PbFieldType.OU3)
    ..a<$core.int>(2, _omitFieldNames ? '' : 'recording', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Condition clone() => Condition()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Condition copyWith(void Function(Condition) updates) => super.copyWith((message) => updates(message as Condition)) as Condition;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Condition create() => Condition._();
  Condition createEmptyInstance() => create();
  static $pb.PbList<Condition> createRepeated() => $pb.PbList<Condition>();
  @$core.pragma('dart2js:noInline')
  static Condition getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Condition>(create);
  static Condition? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get flags => $_getIZ(0);
  @$pb.TagNumber(1)
  set flags($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasFlags() => $_has(0);
  @$pb.TagNumber(1)
  void clearFlags() => clearField(1);

  @$pb.TagNumber(2)
  $core.int get recording => $_getIZ(1);
  @$pb.TagNumber(2)
  set recording($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasRecording() => $_has(1);
  @$pb.TagNumber(2)
  void clearRecording() => clearField(2);
}

class NetworkInfo extends $pb.GeneratedMessage {
  factory NetworkInfo() => create();
  NetworkInfo._() : super();
  factory NetworkInfo.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory NetworkInfo.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'NetworkInfo', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'ssid')
    ..aOS(2, _omitFieldNames ? '' : 'password')
    ..aOB(3, _omitFieldNames ? '' : 'create')
    ..aOB(4, _omitFieldNames ? '' : 'preferred')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  NetworkInfo clone() => NetworkInfo()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  NetworkInfo copyWith(void Function(NetworkInfo) updates) => super.copyWith((message) => updates(message as NetworkInfo)) as NetworkInfo;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static NetworkInfo create() => NetworkInfo._();
  NetworkInfo createEmptyInstance() => create();
  static $pb.PbList<NetworkInfo> createRepeated() => $pb.PbList<NetworkInfo>();
  @$core.pragma('dart2js:noInline')
  static NetworkInfo getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<NetworkInfo>(create);
  static NetworkInfo? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get ssid => $_getSZ(0);
  @$pb.TagNumber(1)
  set ssid($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasSsid() => $_has(0);
  @$pb.TagNumber(1)
  void clearSsid() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get password => $_getSZ(1);
  @$pb.TagNumber(2)
  set password($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasPassword() => $_has(1);
  @$pb.TagNumber(2)
  void clearPassword() => clearField(2);

  @$pb.TagNumber(3)
  $core.bool get create_3 => $_getBF(2);
  @$pb.TagNumber(3)
  set create_3($core.bool v) { $_setBool(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasCreate_3() => $_has(2);
  @$pb.TagNumber(3)
  void clearCreate_3() => clearField(3);

  @$pb.TagNumber(4)
  $core.bool get preferred => $_getBF(3);
  @$pb.TagNumber(4)
  set preferred($core.bool v) { $_setBool(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasPreferred() => $_has(3);
  @$pb.TagNumber(4)
  void clearPreferred() => clearField(4);
}

class WifiTransmission extends $pb.GeneratedMessage {
  factory WifiTransmission() => create();
  WifiTransmission._() : super();
  factory WifiTransmission.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory WifiTransmission.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'WifiTransmission', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..aOS(1, _omitFieldNames ? '' : 'url')
    ..aOS(2, _omitFieldNames ? '' : 'token')
    ..aOB(3, _omitFieldNames ? '' : 'enabled')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  WifiTransmission clone() => WifiTransmission()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  WifiTransmission copyWith(void Function(WifiTransmission) updates) => super.copyWith((message) => updates(message as WifiTransmission)) as WifiTransmission;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static WifiTransmission create() => WifiTransmission._();
  WifiTransmission createEmptyInstance() => create();
  static $pb.PbList<WifiTransmission> createRepeated() => $pb.PbList<WifiTransmission>();
  @$core.pragma('dart2js:noInline')
  static WifiTransmission getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<WifiTransmission>(create);
  static WifiTransmission? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get url => $_getSZ(0);
  @$pb.TagNumber(1)
  set url($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasUrl() => $_has(0);
  @$pb.TagNumber(1)
  void clearUrl() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get token => $_getSZ(1);
  @$pb.TagNumber(2)
  set token($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasToken() => $_has(1);
  @$pb.TagNumber(2)
  void clearToken() => clearField(2);

  @$pb.TagNumber(3)
  $core.bool get enabled => $_getBF(2);
  @$pb.TagNumber(3)
  set enabled($core.bool v) { $_setBool(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasEnabled() => $_has(2);
  @$pb.TagNumber(3)
  void clearEnabled() => clearField(3);
}

class TransmissionSettings extends $pb.GeneratedMessage {
  factory TransmissionSettings() => create();
  TransmissionSettings._() : super();
  factory TransmissionSettings.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory TransmissionSettings.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'TransmissionSettings', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..aOM<WifiTransmission>(1, _omitFieldNames ? '' : 'wifi', subBuilder: WifiTransmission.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  TransmissionSettings clone() => TransmissionSettings()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  TransmissionSettings copyWith(void Function(TransmissionSettings) updates) => super.copyWith((message) => updates(message as TransmissionSettings)) as TransmissionSettings;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static TransmissionSettings create() => TransmissionSettings._();
  TransmissionSettings createEmptyInstance() => create();
  static $pb.PbList<TransmissionSettings> createRepeated() => $pb.PbList<TransmissionSettings>();
  @$core.pragma('dart2js:noInline')
  static TransmissionSettings getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<TransmissionSettings>(create);
  static TransmissionSettings? _defaultInstance;

  @$pb.TagNumber(1)
  WifiTransmission get wifi => $_getN(0);
  @$pb.TagNumber(1)
  set wifi(WifiTransmission v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasWifi() => $_has(0);
  @$pb.TagNumber(1)
  void clearWifi() => clearField(1);
  @$pb.TagNumber(1)
  WifiTransmission ensureWifi() => $_ensure(0);
}

class NetworkSettings extends $pb.GeneratedMessage {
  factory NetworkSettings() => create();
  NetworkSettings._() : super();
  factory NetworkSettings.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory NetworkSettings.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'NetworkSettings', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..pc<NetworkInfo>(1, _omitFieldNames ? '' : 'networks', $pb.PbFieldType.PM, subBuilder: NetworkInfo.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  NetworkSettings clone() => NetworkSettings()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  NetworkSettings copyWith(void Function(NetworkSettings) updates) => super.copyWith((message) => updates(message as NetworkSettings)) as NetworkSettings;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static NetworkSettings create() => NetworkSettings._();
  NetworkSettings createEmptyInstance() => create();
  static $pb.PbList<NetworkSettings> createRepeated() => $pb.PbList<NetworkSettings>();
  @$core.pragma('dart2js:noInline')
  static NetworkSettings getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<NetworkSettings>(create);
  static NetworkSettings? _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<NetworkInfo> get networks => $_getList(0);
}

class LoraSettings extends $pb.GeneratedMessage {
  factory LoraSettings() => create();
  LoraSettings._() : super();
  factory LoraSettings.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory LoraSettings.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'LoraSettings', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'deviceEui', $pb.PbFieldType.OY, protoName: 'deviceEui')
    ..a<$core.List<$core.int>>(2, _omitFieldNames ? '' : 'appKey', $pb.PbFieldType.OY, protoName: 'appKey')
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'joinEui', $pb.PbFieldType.OY, protoName: 'joinEui')
    ..a<$core.int>(4, _omitFieldNames ? '' : 'frequencyBand', $pb.PbFieldType.OU3, protoName: 'frequencyBand')
    ..a<$core.List<$core.int>>(5, _omitFieldNames ? '' : 'deviceAddress', $pb.PbFieldType.OY, protoName: 'deviceAddress')
    ..a<$core.List<$core.int>>(6, _omitFieldNames ? '' : 'networkSessionKey', $pb.PbFieldType.OY, protoName: 'networkSessionKey')
    ..a<$core.List<$core.int>>(7, _omitFieldNames ? '' : 'appSessionKey', $pb.PbFieldType.OY, protoName: 'appSessionKey')
    ..a<$core.int>(8, _omitFieldNames ? '' : 'uplinkCounter', $pb.PbFieldType.OU3, protoName: 'uplinkCounter')
    ..a<$core.int>(9, _omitFieldNames ? '' : 'downlinkCounter', $pb.PbFieldType.OU3, protoName: 'downlinkCounter')
    ..a<$core.int>(10, _omitFieldNames ? '' : 'rxDelay1', $pb.PbFieldType.OU3, protoName: 'rxDelay1')
    ..a<$core.int>(11, _omitFieldNames ? '' : 'rxDelay2', $pb.PbFieldType.OU3, protoName: 'rxDelay2')
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  LoraSettings clone() => LoraSettings()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  LoraSettings copyWith(void Function(LoraSettings) updates) => super.copyWith((message) => updates(message as LoraSettings)) as LoraSettings;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static LoraSettings create() => LoraSettings._();
  LoraSettings createEmptyInstance() => create();
  static $pb.PbList<LoraSettings> createRepeated() => $pb.PbList<LoraSettings>();
  @$core.pragma('dart2js:noInline')
  static LoraSettings getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<LoraSettings>(create);
  static LoraSettings? _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$core.int> get deviceEui => $_getN(0);
  @$pb.TagNumber(1)
  set deviceEui($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasDeviceEui() => $_has(0);
  @$pb.TagNumber(1)
  void clearDeviceEui() => clearField(1);

  @$pb.TagNumber(2)
  $core.List<$core.int> get appKey => $_getN(1);
  @$pb.TagNumber(2)
  set appKey($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAppKey() => $_has(1);
  @$pb.TagNumber(2)
  void clearAppKey() => clearField(2);

  @$pb.TagNumber(3)
  $core.List<$core.int> get joinEui => $_getN(2);
  @$pb.TagNumber(3)
  set joinEui($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasJoinEui() => $_has(2);
  @$pb.TagNumber(3)
  void clearJoinEui() => clearField(3);

  @$pb.TagNumber(4)
  $core.int get frequencyBand => $_getIZ(3);
  @$pb.TagNumber(4)
  set frequencyBand($core.int v) { $_setUnsignedInt32(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasFrequencyBand() => $_has(3);
  @$pb.TagNumber(4)
  void clearFrequencyBand() => clearField(4);

  @$pb.TagNumber(5)
  $core.List<$core.int> get deviceAddress => $_getN(4);
  @$pb.TagNumber(5)
  set deviceAddress($core.List<$core.int> v) { $_setBytes(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasDeviceAddress() => $_has(4);
  @$pb.TagNumber(5)
  void clearDeviceAddress() => clearField(5);

  @$pb.TagNumber(6)
  $core.List<$core.int> get networkSessionKey => $_getN(5);
  @$pb.TagNumber(6)
  set networkSessionKey($core.List<$core.int> v) { $_setBytes(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasNetworkSessionKey() => $_has(5);
  @$pb.TagNumber(6)
  void clearNetworkSessionKey() => clearField(6);

  @$pb.TagNumber(7)
  $core.List<$core.int> get appSessionKey => $_getN(6);
  @$pb.TagNumber(7)
  set appSessionKey($core.List<$core.int> v) { $_setBytes(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasAppSessionKey() => $_has(6);
  @$pb.TagNumber(7)
  void clearAppSessionKey() => clearField(7);

  @$pb.TagNumber(8)
  $core.int get uplinkCounter => $_getIZ(7);
  @$pb.TagNumber(8)
  set uplinkCounter($core.int v) { $_setUnsignedInt32(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasUplinkCounter() => $_has(7);
  @$pb.TagNumber(8)
  void clearUplinkCounter() => clearField(8);

  @$pb.TagNumber(9)
  $core.int get downlinkCounter => $_getIZ(8);
  @$pb.TagNumber(9)
  set downlinkCounter($core.int v) { $_setUnsignedInt32(8, v); }
  @$pb.TagNumber(9)
  $core.bool hasDownlinkCounter() => $_has(8);
  @$pb.TagNumber(9)
  void clearDownlinkCounter() => clearField(9);

  @$pb.TagNumber(10)
  $core.int get rxDelay1 => $_getIZ(9);
  @$pb.TagNumber(10)
  set rxDelay1($core.int v) { $_setUnsignedInt32(9, v); }
  @$pb.TagNumber(10)
  $core.bool hasRxDelay1() => $_has(9);
  @$pb.TagNumber(10)
  void clearRxDelay1() => clearField(10);

  @$pb.TagNumber(11)
  $core.int get rxDelay2 => $_getIZ(10);
  @$pb.TagNumber(11)
  set rxDelay2($core.int v) { $_setUnsignedInt32(10, v); }
  @$pb.TagNumber(11)
  $core.bool hasRxDelay2() => $_has(10);
  @$pb.TagNumber(11)
  void clearRxDelay2() => clearField(11);
}

class Fault extends $pb.GeneratedMessage {
  factory Fault() => create();
  Fault._() : super();
  factory Fault.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Fault.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Fault', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..a<$core.int>(1, _omitFieldNames ? '' : 'time', $pb.PbFieldType.OU3)
    ..a<$core.int>(2, _omitFieldNames ? '' : 'code', $pb.PbFieldType.OU3)
    ..aOS(3, _omitFieldNames ? '' : 'description')
    ..a<$core.List<$core.int>>(4, _omitFieldNames ? '' : 'debug', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Fault clone() => Fault()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Fault copyWith(void Function(Fault) updates) => super.copyWith((message) => updates(message as Fault)) as Fault;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Fault create() => Fault._();
  Fault createEmptyInstance() => create();
  static $pb.PbList<Fault> createRepeated() => $pb.PbList<Fault>();
  @$core.pragma('dart2js:noInline')
  static Fault getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Fault>(create);
  static Fault? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get time => $_getIZ(0);
  @$pb.TagNumber(1)
  set time($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasTime() => $_has(0);
  @$pb.TagNumber(1)
  void clearTime() => clearField(1);

  @$pb.TagNumber(2)
  $core.int get code => $_getIZ(1);
  @$pb.TagNumber(2)
  set code($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasCode() => $_has(1);
  @$pb.TagNumber(2)
  void clearCode() => clearField(2);

  @$pb.TagNumber(3)
  $core.String get description => $_getSZ(2);
  @$pb.TagNumber(3)
  set description($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasDescription() => $_has(2);
  @$pb.TagNumber(3)
  void clearDescription() => clearField(3);

  @$pb.TagNumber(4)
  $core.List<$core.int> get debug => $_getN(3);
  @$pb.TagNumber(4)
  set debug($core.List<$core.int> v) { $_setBytes(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasDebug() => $_has(3);
  @$pb.TagNumber(4)
  void clearDebug() => clearField(4);
}

class DataRecord extends $pb.GeneratedMessage {
  factory DataRecord() => create();
  DataRecord._() : super();
  factory DataRecord.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DataRecord.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'DataRecord', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..aOM<LoggedReading>(1, _omitFieldNames ? '' : 'loggedReading', protoName: 'loggedReading', subBuilder: LoggedReading.create)
    ..aOM<Metadata>(2, _omitFieldNames ? '' : 'metadata', subBuilder: Metadata.create)
    ..aOM<LogMessage>(3, _omitFieldNames ? '' : 'log', subBuilder: LogMessage.create)
    ..aOM<Status>(4, _omitFieldNames ? '' : 'status', subBuilder: Status.create)
    ..aOM<Readings>(5, _omitFieldNames ? '' : 'readings', subBuilder: Readings.create)
    ..pc<ModuleInfo>(6, _omitFieldNames ? '' : 'modules', $pb.PbFieldType.PM, subBuilder: ModuleInfo.create)
    ..aOM<Schedule>(7, _omitFieldNames ? '' : 'schedule', subBuilder: Schedule.create)
    ..a<$fixnum.Int64>(8, _omitFieldNames ? '' : 'meta', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOM<Identity>(9, _omitFieldNames ? '' : 'identity', subBuilder: Identity.create)
    ..aOM<Condition>(10, _omitFieldNames ? '' : 'condition', subBuilder: Condition.create)
    ..aOM<LoraSettings>(11, _omitFieldNames ? '' : 'lora', subBuilder: LoraSettings.create)
    ..aOM<NetworkSettings>(12, _omitFieldNames ? '' : 'network', subBuilder: NetworkSettings.create)
    ..pc<LogMessage>(13, _omitFieldNames ? '' : 'logs', $pb.PbFieldType.PM, subBuilder: LogMessage.create)
    ..aOM<TransmissionSettings>(14, _omitFieldNames ? '' : 'transmission', subBuilder: TransmissionSettings.create)
    ..pc<Fault>(15, _omitFieldNames ? '' : 'faults', $pb.PbFieldType.PM, subBuilder: Fault.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  DataRecord clone() => DataRecord()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  DataRecord copyWith(void Function(DataRecord) updates) => super.copyWith((message) => updates(message as DataRecord)) as DataRecord;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static DataRecord create() => DataRecord._();
  DataRecord createEmptyInstance() => create();
  static $pb.PbList<DataRecord> createRepeated() => $pb.PbList<DataRecord>();
  @$core.pragma('dart2js:noInline')
  static DataRecord getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DataRecord>(create);
  static DataRecord? _defaultInstance;

  @$pb.TagNumber(1)
  LoggedReading get loggedReading => $_getN(0);
  @$pb.TagNumber(1)
  set loggedReading(LoggedReading v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasLoggedReading() => $_has(0);
  @$pb.TagNumber(1)
  void clearLoggedReading() => clearField(1);
  @$pb.TagNumber(1)
  LoggedReading ensureLoggedReading() => $_ensure(0);

  @$pb.TagNumber(2)
  Metadata get metadata => $_getN(1);
  @$pb.TagNumber(2)
  set metadata(Metadata v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasMetadata() => $_has(1);
  @$pb.TagNumber(2)
  void clearMetadata() => clearField(2);
  @$pb.TagNumber(2)
  Metadata ensureMetadata() => $_ensure(1);

  @$pb.TagNumber(3)
  LogMessage get log => $_getN(2);
  @$pb.TagNumber(3)
  set log(LogMessage v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasLog() => $_has(2);
  @$pb.TagNumber(3)
  void clearLog() => clearField(3);
  @$pb.TagNumber(3)
  LogMessage ensureLog() => $_ensure(2);

  @$pb.TagNumber(4)
  Status get status => $_getN(3);
  @$pb.TagNumber(4)
  set status(Status v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasStatus() => $_has(3);
  @$pb.TagNumber(4)
  void clearStatus() => clearField(4);
  @$pb.TagNumber(4)
  Status ensureStatus() => $_ensure(3);

  @$pb.TagNumber(5)
  Readings get readings => $_getN(4);
  @$pb.TagNumber(5)
  set readings(Readings v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasReadings() => $_has(4);
  @$pb.TagNumber(5)
  void clearReadings() => clearField(5);
  @$pb.TagNumber(5)
  Readings ensureReadings() => $_ensure(4);

  @$pb.TagNumber(6)
  $core.List<ModuleInfo> get modules => $_getList(5);

  @$pb.TagNumber(7)
  Schedule get schedule => $_getN(6);
  @$pb.TagNumber(7)
  set schedule(Schedule v) { setField(7, v); }
  @$pb.TagNumber(7)
  $core.bool hasSchedule() => $_has(6);
  @$pb.TagNumber(7)
  void clearSchedule() => clearField(7);
  @$pb.TagNumber(7)
  Schedule ensureSchedule() => $_ensure(6);

  @$pb.TagNumber(8)
  $fixnum.Int64 get meta => $_getI64(7);
  @$pb.TagNumber(8)
  set meta($fixnum.Int64 v) { $_setInt64(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasMeta() => $_has(7);
  @$pb.TagNumber(8)
  void clearMeta() => clearField(8);

  @$pb.TagNumber(9)
  Identity get identity => $_getN(8);
  @$pb.TagNumber(9)
  set identity(Identity v) { setField(9, v); }
  @$pb.TagNumber(9)
  $core.bool hasIdentity() => $_has(8);
  @$pb.TagNumber(9)
  void clearIdentity() => clearField(9);
  @$pb.TagNumber(9)
  Identity ensureIdentity() => $_ensure(8);

  @$pb.TagNumber(10)
  Condition get condition => $_getN(9);
  @$pb.TagNumber(10)
  set condition(Condition v) { setField(10, v); }
  @$pb.TagNumber(10)
  $core.bool hasCondition() => $_has(9);
  @$pb.TagNumber(10)
  void clearCondition() => clearField(10);
  @$pb.TagNumber(10)
  Condition ensureCondition() => $_ensure(9);

  @$pb.TagNumber(11)
  LoraSettings get lora => $_getN(10);
  @$pb.TagNumber(11)
  set lora(LoraSettings v) { setField(11, v); }
  @$pb.TagNumber(11)
  $core.bool hasLora() => $_has(10);
  @$pb.TagNumber(11)
  void clearLora() => clearField(11);
  @$pb.TagNumber(11)
  LoraSettings ensureLora() => $_ensure(10);

  @$pb.TagNumber(12)
  NetworkSettings get network => $_getN(11);
  @$pb.TagNumber(12)
  set network(NetworkSettings v) { setField(12, v); }
  @$pb.TagNumber(12)
  $core.bool hasNetwork() => $_has(11);
  @$pb.TagNumber(12)
  void clearNetwork() => clearField(12);
  @$pb.TagNumber(12)
  NetworkSettings ensureNetwork() => $_ensure(11);

  @$pb.TagNumber(13)
  $core.List<LogMessage> get logs => $_getList(12);

  @$pb.TagNumber(14)
  TransmissionSettings get transmission => $_getN(13);
  @$pb.TagNumber(14)
  set transmission(TransmissionSettings v) { setField(14, v); }
  @$pb.TagNumber(14)
  $core.bool hasTransmission() => $_has(13);
  @$pb.TagNumber(14)
  void clearTransmission() => clearField(14);
  @$pb.TagNumber(14)
  TransmissionSettings ensureTransmission() => $_ensure(13);

  @$pb.TagNumber(15)
  $core.List<Fault> get faults => $_getList(14);
}

class SignedRecord extends $pb.GeneratedMessage {
  factory SignedRecord() => create();
  SignedRecord._() : super();
  factory SignedRecord.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SignedRecord.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'SignedRecord', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..e<SignedRecordKind>(1, _omitFieldNames ? '' : 'kind', $pb.PbFieldType.OE, defaultOrMaker: SignedRecordKind.SIGNED_RECORD_KIND_NONE, valueOf: SignedRecordKind.valueOf, enumValues: SignedRecordKind.values)
    ..aInt64(2, _omitFieldNames ? '' : 'time')
    ..a<$core.List<$core.int>>(3, _omitFieldNames ? '' : 'data', $pb.PbFieldType.OY)
    ..a<$core.List<$core.int>>(4, _omitFieldNames ? '' : 'hash', $pb.PbFieldType.OY)
    ..a<$fixnum.Int64>(5, _omitFieldNames ? '' : 'record', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  SignedRecord clone() => SignedRecord()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  SignedRecord copyWith(void Function(SignedRecord) updates) => super.copyWith((message) => updates(message as SignedRecord)) as SignedRecord;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static SignedRecord create() => SignedRecord._();
  SignedRecord createEmptyInstance() => create();
  static $pb.PbList<SignedRecord> createRepeated() => $pb.PbList<SignedRecord>();
  @$core.pragma('dart2js:noInline')
  static SignedRecord getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SignedRecord>(create);
  static SignedRecord? _defaultInstance;

  @$pb.TagNumber(1)
  SignedRecordKind get kind => $_getN(0);
  @$pb.TagNumber(1)
  set kind(SignedRecordKind v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasKind() => $_has(0);
  @$pb.TagNumber(1)
  void clearKind() => clearField(1);

  @$pb.TagNumber(2)
  $fixnum.Int64 get time => $_getI64(1);
  @$pb.TagNumber(2)
  set time($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasTime() => $_has(1);
  @$pb.TagNumber(2)
  void clearTime() => clearField(2);

  @$pb.TagNumber(3)
  $core.List<$core.int> get data => $_getN(2);
  @$pb.TagNumber(3)
  set data($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasData() => $_has(2);
  @$pb.TagNumber(3)
  void clearData() => clearField(3);

  @$pb.TagNumber(4)
  $core.List<$core.int> get hash => $_getN(3);
  @$pb.TagNumber(4)
  set hash($core.List<$core.int> v) { $_setBytes(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasHash() => $_has(3);
  @$pb.TagNumber(4)
  void clearHash() => clearField(4);

  @$pb.TagNumber(5)
  $fixnum.Int64 get record => $_getI64(4);
  @$pb.TagNumber(5)
  set record($fixnum.Int64 v) { $_setInt64(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasRecord() => $_has(4);
  @$pb.TagNumber(5)
  void clearRecord() => clearField(5);
}

class LoraRecord extends $pb.GeneratedMessage {
  factory LoraRecord() => create();
  LoraRecord._() : super();
  factory LoraRecord.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory LoraRecord.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'LoraRecord', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, _omitFieldNames ? '' : 'deviceId', $pb.PbFieldType.OY, protoName: 'deviceId')
    ..aInt64(2, _omitFieldNames ? '' : 'time')
    ..a<$fixnum.Int64>(3, _omitFieldNames ? '' : 'number', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.int>(4, _omitFieldNames ? '' : 'module', $pb.PbFieldType.OU3)
    ..a<$fixnum.Int64>(5, _omitFieldNames ? '' : 'sensor', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..p<$core.double>(6, _omitFieldNames ? '' : 'values', $pb.PbFieldType.KF)
    ..a<$core.List<$core.int>>(7, _omitFieldNames ? '' : 'data', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  LoraRecord clone() => LoraRecord()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  LoraRecord copyWith(void Function(LoraRecord) updates) => super.copyWith((message) => updates(message as LoraRecord)) as LoraRecord;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static LoraRecord create() => LoraRecord._();
  LoraRecord createEmptyInstance() => create();
  static $pb.PbList<LoraRecord> createRepeated() => $pb.PbList<LoraRecord>();
  @$core.pragma('dart2js:noInline')
  static LoraRecord getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<LoraRecord>(create);
  static LoraRecord? _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$core.int> get deviceId => $_getN(0);
  @$pb.TagNumber(1)
  set deviceId($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasDeviceId() => $_has(0);
  @$pb.TagNumber(1)
  void clearDeviceId() => clearField(1);

  @$pb.TagNumber(2)
  $fixnum.Int64 get time => $_getI64(1);
  @$pb.TagNumber(2)
  set time($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasTime() => $_has(1);
  @$pb.TagNumber(2)
  void clearTime() => clearField(2);

  @$pb.TagNumber(3)
  $fixnum.Int64 get number => $_getI64(2);
  @$pb.TagNumber(3)
  set number($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasNumber() => $_has(2);
  @$pb.TagNumber(3)
  void clearNumber() => clearField(3);

  @$pb.TagNumber(4)
  $core.int get module => $_getIZ(3);
  @$pb.TagNumber(4)
  set module($core.int v) { $_setUnsignedInt32(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasModule() => $_has(3);
  @$pb.TagNumber(4)
  void clearModule() => clearField(4);

  @$pb.TagNumber(5)
  $fixnum.Int64 get sensor => $_getI64(4);
  @$pb.TagNumber(5)
  set sensor($fixnum.Int64 v) { $_setInt64(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasSensor() => $_has(4);
  @$pb.TagNumber(5)
  void clearSensor() => clearField(5);

  @$pb.TagNumber(6)
  $core.List<$core.double> get values => $_getList(5);

  @$pb.TagNumber(7)
  $core.List<$core.int> get data => $_getN(6);
  @$pb.TagNumber(7)
  set data($core.List<$core.int> v) { $_setBytes(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasData() => $_has(6);
  @$pb.TagNumber(7)
  void clearData() => clearField(7);
}

class CalibrationPoint extends $pb.GeneratedMessage {
  factory CalibrationPoint() => create();
  CalibrationPoint._() : super();
  factory CalibrationPoint.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory CalibrationPoint.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'CalibrationPoint', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..p<$core.double>(1, _omitFieldNames ? '' : 'references', $pb.PbFieldType.KF)
    ..p<$core.double>(2, _omitFieldNames ? '' : 'uncalibrated', $pb.PbFieldType.KF)
    ..p<$core.double>(3, _omitFieldNames ? '' : 'factory', $pb.PbFieldType.KF)
    ..p<$core.List<$core.int>>(4, _omitFieldNames ? '' : 'adc', $pb.PbFieldType.PY)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  CalibrationPoint clone() => CalibrationPoint()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  CalibrationPoint copyWith(void Function(CalibrationPoint) updates) => super.copyWith((message) => updates(message as CalibrationPoint)) as CalibrationPoint;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static CalibrationPoint create() => CalibrationPoint._();
  CalibrationPoint createEmptyInstance() => create();
  static $pb.PbList<CalibrationPoint> createRepeated() => $pb.PbList<CalibrationPoint>();
  @$core.pragma('dart2js:noInline')
  static CalibrationPoint getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<CalibrationPoint>(create);
  static CalibrationPoint? _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$core.double> get references => $_getList(0);

  @$pb.TagNumber(2)
  $core.List<$core.double> get uncalibrated => $_getList(1);

  @$pb.TagNumber(3)
  $core.List<$core.double> get factory => $_getList(2);

  @$pb.TagNumber(4)
  $core.List<$core.List<$core.int>> get adc => $_getList(3);
}

class CalibrationCoefficients extends $pb.GeneratedMessage {
  factory CalibrationCoefficients() => create();
  CalibrationCoefficients._() : super();
  factory CalibrationCoefficients.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory CalibrationCoefficients.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'CalibrationCoefficients', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..p<$core.double>(1, _omitFieldNames ? '' : 'values', $pb.PbFieldType.KF)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  CalibrationCoefficients clone() => CalibrationCoefficients()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  CalibrationCoefficients copyWith(void Function(CalibrationCoefficients) updates) => super.copyWith((message) => updates(message as CalibrationCoefficients)) as CalibrationCoefficients;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static CalibrationCoefficients create() => CalibrationCoefficients._();
  CalibrationCoefficients createEmptyInstance() => create();
  static $pb.PbList<CalibrationCoefficients> createRepeated() => $pb.PbList<CalibrationCoefficients>();
  @$core.pragma('dart2js:noInline')
  static CalibrationCoefficients getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<CalibrationCoefficients>(create);
  static CalibrationCoefficients? _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$core.double> get values => $_getList(0);
}

class Calibration extends $pb.GeneratedMessage {
  factory Calibration() => create();
  Calibration._() : super();
  factory Calibration.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Calibration.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'Calibration', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..e<CurveType>(1, _omitFieldNames ? '' : 'type', $pb.PbFieldType.OE, defaultOrMaker: CurveType.CURVE_NONE, valueOf: CurveType.valueOf, enumValues: CurveType.values)
    ..a<$core.int>(2, _omitFieldNames ? '' : 'time', $pb.PbFieldType.OU3)
    ..pc<CalibrationPoint>(3, _omitFieldNames ? '' : 'points', $pb.PbFieldType.PM, subBuilder: CalibrationPoint.create)
    ..aOM<CalibrationCoefficients>(4, _omitFieldNames ? '' : 'coefficients', subBuilder: CalibrationCoefficients.create)
    ..aOM<Firmware>(5, _omitFieldNames ? '' : 'firmware', subBuilder: Firmware.create)
    ..a<$core.int>(6, _omitFieldNames ? '' : 'kind', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Calibration clone() => Calibration()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Calibration copyWith(void Function(Calibration) updates) => super.copyWith((message) => updates(message as Calibration)) as Calibration;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static Calibration create() => Calibration._();
  Calibration createEmptyInstance() => create();
  static $pb.PbList<Calibration> createRepeated() => $pb.PbList<Calibration>();
  @$core.pragma('dart2js:noInline')
  static Calibration getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Calibration>(create);
  static Calibration? _defaultInstance;

  @$pb.TagNumber(1)
  CurveType get type => $_getN(0);
  @$pb.TagNumber(1)
  set type(CurveType v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasType() => $_has(0);
  @$pb.TagNumber(1)
  void clearType() => clearField(1);

  @$pb.TagNumber(2)
  $core.int get time => $_getIZ(1);
  @$pb.TagNumber(2)
  set time($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasTime() => $_has(1);
  @$pb.TagNumber(2)
  void clearTime() => clearField(2);

  @$pb.TagNumber(3)
  $core.List<CalibrationPoint> get points => $_getList(2);

  @$pb.TagNumber(4)
  CalibrationCoefficients get coefficients => $_getN(3);
  @$pb.TagNumber(4)
  set coefficients(CalibrationCoefficients v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasCoefficients() => $_has(3);
  @$pb.TagNumber(4)
  void clearCoefficients() => clearField(4);
  @$pb.TagNumber(4)
  CalibrationCoefficients ensureCoefficients() => $_ensure(3);

  @$pb.TagNumber(5)
  Firmware get firmware => $_getN(4);
  @$pb.TagNumber(5)
  set firmware(Firmware v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasFirmware() => $_has(4);
  @$pb.TagNumber(5)
  void clearFirmware() => clearField(5);
  @$pb.TagNumber(5)
  Firmware ensureFirmware() => $_ensure(4);

  @$pb.TagNumber(6)
  $core.int get kind => $_getIZ(5);
  @$pb.TagNumber(6)
  set kind($core.int v) { $_setUnsignedInt32(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasKind() => $_has(5);
  @$pb.TagNumber(6)
  void clearKind() => clearField(6);
}

class ModuleConfiguration extends $pb.GeneratedMessage {
  factory ModuleConfiguration() => create();
  ModuleConfiguration._() : super();
  factory ModuleConfiguration.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory ModuleConfiguration.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(_omitMessageNames ? '' : 'ModuleConfiguration', package: const $pb.PackageName(_omitMessageNames ? '' : 'fk_data'), createEmptyInstance: create)
    ..aOM<Calibration>(1, _omitFieldNames ? '' : 'calibration', subBuilder: Calibration.create)
    ..pc<Calibration>(2, _omitFieldNames ? '' : 'calibrations', $pb.PbFieldType.PM, subBuilder: Calibration.create)
    ..hasRequiredFields = false
  ;

  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  ModuleConfiguration clone() => ModuleConfiguration()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  ModuleConfiguration copyWith(void Function(ModuleConfiguration) updates) => super.copyWith((message) => updates(message as ModuleConfiguration)) as ModuleConfiguration;

  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static ModuleConfiguration create() => ModuleConfiguration._();
  ModuleConfiguration createEmptyInstance() => create();
  static $pb.PbList<ModuleConfiguration> createRepeated() => $pb.PbList<ModuleConfiguration>();
  @$core.pragma('dart2js:noInline')
  static ModuleConfiguration getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<ModuleConfiguration>(create);
  static ModuleConfiguration? _defaultInstance;

  @$pb.TagNumber(1)
  Calibration get calibration => $_getN(0);
  @$pb.TagNumber(1)
  set calibration(Calibration v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasCalibration() => $_has(0);
  @$pb.TagNumber(1)
  void clearCalibration() => clearField(1);
  @$pb.TagNumber(1)
  Calibration ensureCalibration() => $_ensure(0);

  @$pb.TagNumber(2)
  $core.List<Calibration> get calibrations => $_getList(1);
}


const _omitFieldNames = $core.bool.fromEnvironment('protobuf.omit_field_names');
const _omitMessageNames = $core.bool.fromEnvironment('protobuf.omit_message_names');
