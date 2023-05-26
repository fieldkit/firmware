///
//  Generated code. Do not modify.
//  source: fk-app.proto
//
// @dart = 2.12
// ignore_for_file: annotate_overrides,camel_case_types,constant_identifier_names,directives_ordering,library_prefixes,non_constant_identifier_names,prefer_final_fields,return_of_invalid_type,unnecessary_const,unnecessary_import,unnecessary_this,unused_import,unused_shown_name

import 'dart:core' as $core;

import 'package:fixnum/fixnum.dart' as $fixnum;
import 'package:protobuf/protobuf.dart' as $pb;

import 'fk-app.pbenum.dart';

export 'fk-app.pbenum.dart';

class QueryCapabilities extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'QueryCapabilities', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'version', $pb.PbFieldType.OU3)
    ..a<$core.int>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'callerTime', $pb.PbFieldType.OU3, protoName: 'callerTime')
    ..hasRequiredFields = false
  ;

  QueryCapabilities._() : super();
  factory QueryCapabilities({
    $core.int? version,
    $core.int? callerTime,
  }) {
    final _result = create();
    if (version != null) {
      _result.version = version;
    }
    if (callerTime != null) {
      _result.callerTime = callerTime;
    }
    return _result;
  }
  factory QueryCapabilities.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory QueryCapabilities.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  QueryCapabilities clone() => QueryCapabilities()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  QueryCapabilities copyWith(void Function(QueryCapabilities) updates) => super.copyWith((message) => updates(message as QueryCapabilities)) as QueryCapabilities; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static QueryCapabilities create() => QueryCapabilities._();
  QueryCapabilities createEmptyInstance() => create();
  static $pb.PbList<QueryCapabilities> createRepeated() => $pb.PbList<QueryCapabilities>();
  @$core.pragma('dart2js:noInline')
  static QueryCapabilities getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<QueryCapabilities>(create);
  static QueryCapabilities? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get version => $_getIZ(0);
  @$pb.TagNumber(1)
  set version($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasVersion() => $_has(0);
  @$pb.TagNumber(1)
  void clearVersion() => clearField(1);

  @$pb.TagNumber(2)
  $core.int get callerTime => $_getIZ(1);
  @$pb.TagNumber(2)
  set callerTime($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasCallerTime() => $_has(1);
  @$pb.TagNumber(2)
  void clearCallerTime() => clearField(2);
}

class LiveValue extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'LiveValue', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..aOB(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'valid')
    ..a<$core.double>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'value', $pb.PbFieldType.OF)
    ..a<$core.double>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'uncalibrated', $pb.PbFieldType.OF)
    ..hasRequiredFields = false
  ;

  LiveValue._() : super();
  factory LiveValue({
    $core.bool? valid,
    $core.double? value,
    $core.double? uncalibrated,
  }) {
    final _result = create();
    if (valid != null) {
      _result.valid = valid;
    }
    if (value != null) {
      _result.value = value;
    }
    if (uncalibrated != null) {
      _result.uncalibrated = uncalibrated;
    }
    return _result;
  }
  factory LiveValue.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory LiveValue.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  LiveValue clone() => LiveValue()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  LiveValue copyWith(void Function(LiveValue) updates) => super.copyWith((message) => updates(message as LiveValue)) as LiveValue; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static LiveValue create() => LiveValue._();
  LiveValue createEmptyInstance() => create();
  static $pb.PbList<LiveValue> createRepeated() => $pb.PbList<LiveValue>();
  @$core.pragma('dart2js:noInline')
  static LiveValue getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<LiveValue>(create);
  static LiveValue? _defaultInstance;

  @$pb.TagNumber(1)
  $core.bool get valid => $_getBF(0);
  @$pb.TagNumber(1)
  set valid($core.bool v) { $_setBool(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasValid() => $_has(0);
  @$pb.TagNumber(1)
  void clearValid() => clearField(1);

  @$pb.TagNumber(2)
  $core.double get value => $_getN(1);
  @$pb.TagNumber(2)
  set value($core.double v) { $_setFloat(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasValue() => $_has(1);
  @$pb.TagNumber(2)
  void clearValue() => clearField(2);

  @$pb.TagNumber(3)
  $core.double get uncalibrated => $_getN(2);
  @$pb.TagNumber(3)
  set uncalibrated($core.double v) { $_setFloat(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasUncalibrated() => $_has(2);
  @$pb.TagNumber(3)
  void clearUncalibrated() => clearField(3);
}

class SensorCapabilities extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'SensorCapabilities', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'number', $pb.PbFieldType.OU3)
    ..a<$core.int>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'module', $pb.PbFieldType.OU3)
    ..aOS(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'name')
    ..a<$core.int>(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'frequency', $pb.PbFieldType.OU3)
    ..aOS(5, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'unitOfMeasure', protoName: 'unitOfMeasure')
    ..aOS(6, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'path')
    ..a<$core.int>(7, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'flags', $pb.PbFieldType.OU3)
    ..aOM<LiveValue>(8, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'value', subBuilder: LiveValue.create)
    ..aOS(9, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'uncalibratedUnitOfMeasure', protoName: 'uncalibratedUnitOfMeasure')
    ..hasRequiredFields = false
  ;

  SensorCapabilities._() : super();
  factory SensorCapabilities({
    $core.int? number,
    $core.int? module,
    $core.String? name,
    $core.int? frequency,
    $core.String? unitOfMeasure,
    $core.String? path,
    $core.int? flags,
    LiveValue? value,
    $core.String? uncalibratedUnitOfMeasure,
  }) {
    final _result = create();
    if (number != null) {
      _result.number = number;
    }
    if (module != null) {
      _result.module = module;
    }
    if (name != null) {
      _result.name = name;
    }
    if (frequency != null) {
      _result.frequency = frequency;
    }
    if (unitOfMeasure != null) {
      _result.unitOfMeasure = unitOfMeasure;
    }
    if (path != null) {
      _result.path = path;
    }
    if (flags != null) {
      _result.flags = flags;
    }
    if (value != null) {
      _result.value = value;
    }
    if (uncalibratedUnitOfMeasure != null) {
      _result.uncalibratedUnitOfMeasure = uncalibratedUnitOfMeasure;
    }
    return _result;
  }
  factory SensorCapabilities.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SensorCapabilities.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  SensorCapabilities clone() => SensorCapabilities()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  SensorCapabilities copyWith(void Function(SensorCapabilities) updates) => super.copyWith((message) => updates(message as SensorCapabilities)) as SensorCapabilities; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static SensorCapabilities create() => SensorCapabilities._();
  SensorCapabilities createEmptyInstance() => create();
  static $pb.PbList<SensorCapabilities> createRepeated() => $pb.PbList<SensorCapabilities>();
  @$core.pragma('dart2js:noInline')
  static SensorCapabilities getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SensorCapabilities>(create);
  static SensorCapabilities? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get number => $_getIZ(0);
  @$pb.TagNumber(1)
  set number($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasNumber() => $_has(0);
  @$pb.TagNumber(1)
  void clearNumber() => clearField(1);

  @$pb.TagNumber(2)
  $core.int get module => $_getIZ(1);
  @$pb.TagNumber(2)
  set module($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasModule() => $_has(1);
  @$pb.TagNumber(2)
  void clearModule() => clearField(2);

  @$pb.TagNumber(3)
  $core.String get name => $_getSZ(2);
  @$pb.TagNumber(3)
  set name($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasName() => $_has(2);
  @$pb.TagNumber(3)
  void clearName() => clearField(3);

  @$pb.TagNumber(4)
  $core.int get frequency => $_getIZ(3);
  @$pb.TagNumber(4)
  set frequency($core.int v) { $_setUnsignedInt32(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasFrequency() => $_has(3);
  @$pb.TagNumber(4)
  void clearFrequency() => clearField(4);

  @$pb.TagNumber(5)
  $core.String get unitOfMeasure => $_getSZ(4);
  @$pb.TagNumber(5)
  set unitOfMeasure($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasUnitOfMeasure() => $_has(4);
  @$pb.TagNumber(5)
  void clearUnitOfMeasure() => clearField(5);

  @$pb.TagNumber(6)
  $core.String get path => $_getSZ(5);
  @$pb.TagNumber(6)
  set path($core.String v) { $_setString(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasPath() => $_has(5);
  @$pb.TagNumber(6)
  void clearPath() => clearField(6);

  @$pb.TagNumber(7)
  $core.int get flags => $_getIZ(6);
  @$pb.TagNumber(7)
  set flags($core.int v) { $_setUnsignedInt32(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasFlags() => $_has(6);
  @$pb.TagNumber(7)
  void clearFlags() => clearField(7);

  @$pb.TagNumber(8)
  LiveValue get value => $_getN(7);
  @$pb.TagNumber(8)
  set value(LiveValue v) { setField(8, v); }
  @$pb.TagNumber(8)
  $core.bool hasValue() => $_has(7);
  @$pb.TagNumber(8)
  void clearValue() => clearField(8);
  @$pb.TagNumber(8)
  LiveValue ensureValue() => $_ensure(7);

  @$pb.TagNumber(9)
  $core.String get uncalibratedUnitOfMeasure => $_getSZ(8);
  @$pb.TagNumber(9)
  set uncalibratedUnitOfMeasure($core.String v) { $_setString(8, v); }
  @$pb.TagNumber(9)
  $core.bool hasUncalibratedUnitOfMeasure() => $_has(8);
  @$pb.TagNumber(9)
  void clearUncalibratedUnitOfMeasure() => clearField(9);
}

class ModuleHeader extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'ModuleHeader', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'manufacturer', $pb.PbFieldType.OU3)
    ..a<$core.int>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'kind', $pb.PbFieldType.OU3)
    ..a<$core.int>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'version', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  ModuleHeader._() : super();
  factory ModuleHeader({
    $core.int? manufacturer,
    $core.int? kind,
    $core.int? version,
  }) {
    final _result = create();
    if (manufacturer != null) {
      _result.manufacturer = manufacturer;
    }
    if (kind != null) {
      _result.kind = kind;
    }
    if (version != null) {
      _result.version = version;
    }
    return _result;
  }
  factory ModuleHeader.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory ModuleHeader.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  ModuleHeader clone() => ModuleHeader()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  ModuleHeader copyWith(void Function(ModuleHeader) updates) => super.copyWith((message) => updates(message as ModuleHeader)) as ModuleHeader; // ignore: deprecated_member_use
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

class ModuleCapabilities extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'ModuleCapabilities', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'position', $pb.PbFieldType.OU3)
    ..aOS(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'name')
    ..pc<SensorCapabilities>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'sensors', $pb.PbFieldType.PM, subBuilder: SensorCapabilities.create)
    ..aOS(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'path')
    ..a<$core.int>(5, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'flags', $pb.PbFieldType.OU3)
    ..a<$core.List<$core.int>>(6, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'id', $pb.PbFieldType.OY)
    ..aOM<ModuleHeader>(7, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'header', subBuilder: ModuleHeader.create)
    ..a<$core.List<$core.int>>(8, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'configuration', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  ModuleCapabilities._() : super();
  factory ModuleCapabilities({
    $core.int? position,
    $core.String? name,
    $core.Iterable<SensorCapabilities>? sensors,
    $core.String? path,
    $core.int? flags,
    $core.List<$core.int>? id,
    ModuleHeader? header,
    $core.List<$core.int>? configuration,
  }) {
    final _result = create();
    if (position != null) {
      _result.position = position;
    }
    if (name != null) {
      _result.name = name;
    }
    if (sensors != null) {
      _result.sensors.addAll(sensors);
    }
    if (path != null) {
      _result.path = path;
    }
    if (flags != null) {
      _result.flags = flags;
    }
    if (id != null) {
      _result.id = id;
    }
    if (header != null) {
      _result.header = header;
    }
    if (configuration != null) {
      _result.configuration = configuration;
    }
    return _result;
  }
  factory ModuleCapabilities.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory ModuleCapabilities.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  ModuleCapabilities clone() => ModuleCapabilities()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  ModuleCapabilities copyWith(void Function(ModuleCapabilities) updates) => super.copyWith((message) => updates(message as ModuleCapabilities)) as ModuleCapabilities; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static ModuleCapabilities create() => ModuleCapabilities._();
  ModuleCapabilities createEmptyInstance() => create();
  static $pb.PbList<ModuleCapabilities> createRepeated() => $pb.PbList<ModuleCapabilities>();
  @$core.pragma('dart2js:noInline')
  static ModuleCapabilities getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<ModuleCapabilities>(create);
  static ModuleCapabilities? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get position => $_getIZ(0);
  @$pb.TagNumber(1)
  set position($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasPosition() => $_has(0);
  @$pb.TagNumber(1)
  void clearPosition() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get name => $_getSZ(1);
  @$pb.TagNumber(2)
  set name($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasName() => $_has(1);
  @$pb.TagNumber(2)
  void clearName() => clearField(2);

  @$pb.TagNumber(3)
  $core.List<SensorCapabilities> get sensors => $_getList(2);

  @$pb.TagNumber(4)
  $core.String get path => $_getSZ(3);
  @$pb.TagNumber(4)
  set path($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasPath() => $_has(3);
  @$pb.TagNumber(4)
  void clearPath() => clearField(4);

  @$pb.TagNumber(5)
  $core.int get flags => $_getIZ(4);
  @$pb.TagNumber(5)
  set flags($core.int v) { $_setUnsignedInt32(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasFlags() => $_has(4);
  @$pb.TagNumber(5)
  void clearFlags() => clearField(5);

  @$pb.TagNumber(6)
  $core.List<$core.int> get id => $_getN(5);
  @$pb.TagNumber(6)
  set id($core.List<$core.int> v) { $_setBytes(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasId() => $_has(5);
  @$pb.TagNumber(6)
  void clearId() => clearField(6);

  @$pb.TagNumber(7)
  ModuleHeader get header => $_getN(6);
  @$pb.TagNumber(7)
  set header(ModuleHeader v) { setField(7, v); }
  @$pb.TagNumber(7)
  $core.bool hasHeader() => $_has(6);
  @$pb.TagNumber(7)
  void clearHeader() => clearField(7);
  @$pb.TagNumber(7)
  ModuleHeader ensureHeader() => $_ensure(6);

  @$pb.TagNumber(8)
  $core.List<$core.int> get configuration => $_getN(7);
  @$pb.TagNumber(8)
  set configuration($core.List<$core.int> v) { $_setBytes(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasConfiguration() => $_has(7);
  @$pb.TagNumber(8)
  void clearConfiguration() => clearField(8);
}

class Capabilities extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'Capabilities', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'version', $pb.PbFieldType.OU3)
    ..a<$core.List<$core.int>>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'deviceId', $pb.PbFieldType.OY, protoName: 'deviceId')
    ..aOS(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'name')
    ..pc<ModuleCapabilities>(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'modules', $pb.PbFieldType.PM, subBuilder: ModuleCapabilities.create)
    ..pc<SensorCapabilities>(5, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'sensors', $pb.PbFieldType.PM, subBuilder: SensorCapabilities.create)
    ..hasRequiredFields = false
  ;

  Capabilities._() : super();
  factory Capabilities({
    $core.int? version,
    $core.List<$core.int>? deviceId,
    $core.String? name,
    $core.Iterable<ModuleCapabilities>? modules,
    $core.Iterable<SensorCapabilities>? sensors,
  }) {
    final _result = create();
    if (version != null) {
      _result.version = version;
    }
    if (deviceId != null) {
      _result.deviceId = deviceId;
    }
    if (name != null) {
      _result.name = name;
    }
    if (modules != null) {
      _result.modules.addAll(modules);
    }
    if (sensors != null) {
      _result.sensors.addAll(sensors);
    }
    return _result;
  }
  factory Capabilities.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Capabilities.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Capabilities clone() => Capabilities()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Capabilities copyWith(void Function(Capabilities) updates) => super.copyWith((message) => updates(message as Capabilities)) as Capabilities; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static Capabilities create() => Capabilities._();
  Capabilities createEmptyInstance() => create();
  static $pb.PbList<Capabilities> createRepeated() => $pb.PbList<Capabilities>();
  @$core.pragma('dart2js:noInline')
  static Capabilities getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Capabilities>(create);
  static Capabilities? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get version => $_getIZ(0);
  @$pb.TagNumber(1)
  set version($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasVersion() => $_has(0);
  @$pb.TagNumber(1)
  void clearVersion() => clearField(1);

  @$pb.TagNumber(2)
  $core.List<$core.int> get deviceId => $_getN(1);
  @$pb.TagNumber(2)
  set deviceId($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasDeviceId() => $_has(1);
  @$pb.TagNumber(2)
  void clearDeviceId() => clearField(2);

  @$pb.TagNumber(3)
  $core.String get name => $_getSZ(2);
  @$pb.TagNumber(3)
  set name($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasName() => $_has(2);
  @$pb.TagNumber(3)
  void clearName() => clearField(3);

  @$pb.TagNumber(4)
  $core.List<ModuleCapabilities> get modules => $_getList(3);

  @$pb.TagNumber(5)
  $core.List<SensorCapabilities> get sensors => $_getList(4);
}

class NetworkInfo extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'NetworkInfo', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..aOS(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'ssid')
    ..aOS(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'password')
    ..aOB(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'create')
    ..aOB(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'preferred')
    ..aOB(5, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'keeping')
    ..hasRequiredFields = false
  ;

  NetworkInfo._() : super();
  factory NetworkInfo({
    $core.String? ssid,
    $core.String? password,
    $core.bool? create_3,
    $core.bool? preferred,
    $core.bool? keeping,
  }) {
    final _result = create();
    if (ssid != null) {
      _result.ssid = ssid;
    }
    if (password != null) {
      _result.password = password;
    }
    if (create_3 != null) {
      _result.create_3 = create_3;
    }
    if (preferred != null) {
      _result.preferred = preferred;
    }
    if (keeping != null) {
      _result.keeping = keeping;
    }
    return _result;
  }
  factory NetworkInfo.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory NetworkInfo.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  NetworkInfo clone() => NetworkInfo()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  NetworkInfo copyWith(void Function(NetworkInfo) updates) => super.copyWith((message) => updates(message as NetworkInfo)) as NetworkInfo; // ignore: deprecated_member_use
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

  @$pb.TagNumber(5)
  $core.bool get keeping => $_getBF(4);
  @$pb.TagNumber(5)
  set keeping($core.bool v) { $_setBool(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasKeeping() => $_has(4);
  @$pb.TagNumber(5)
  void clearKeeping() => clearField(5);
}

class NetworkSettings extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'NetworkSettings', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'createAccessPoint', $pb.PbFieldType.O3, protoName: 'createAccessPoint')
    ..pc<NetworkInfo>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'networks', $pb.PbFieldType.PM, subBuilder: NetworkInfo.create)
    ..aOM<NetworkInfo>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'connected', subBuilder: NetworkInfo.create)
    ..aOS(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'macAddress', protoName: 'macAddress')
    ..aOB(5, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'modifying')
    ..hasRequiredFields = false
  ;

  NetworkSettings._() : super();
  factory NetworkSettings({
    $core.int? createAccessPoint,
    $core.Iterable<NetworkInfo>? networks,
    NetworkInfo? connected,
    $core.String? macAddress,
    $core.bool? modifying,
  }) {
    final _result = create();
    if (createAccessPoint != null) {
      _result.createAccessPoint = createAccessPoint;
    }
    if (networks != null) {
      _result.networks.addAll(networks);
    }
    if (connected != null) {
      _result.connected = connected;
    }
    if (macAddress != null) {
      _result.macAddress = macAddress;
    }
    if (modifying != null) {
      _result.modifying = modifying;
    }
    return _result;
  }
  factory NetworkSettings.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory NetworkSettings.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  NetworkSettings clone() => NetworkSettings()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  NetworkSettings copyWith(void Function(NetworkSettings) updates) => super.copyWith((message) => updates(message as NetworkSettings)) as NetworkSettings; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static NetworkSettings create() => NetworkSettings._();
  NetworkSettings createEmptyInstance() => create();
  static $pb.PbList<NetworkSettings> createRepeated() => $pb.PbList<NetworkSettings>();
  @$core.pragma('dart2js:noInline')
  static NetworkSettings getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<NetworkSettings>(create);
  static NetworkSettings? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get createAccessPoint => $_getIZ(0);
  @$pb.TagNumber(1)
  set createAccessPoint($core.int v) { $_setSignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasCreateAccessPoint() => $_has(0);
  @$pb.TagNumber(1)
  void clearCreateAccessPoint() => clearField(1);

  @$pb.TagNumber(2)
  $core.List<NetworkInfo> get networks => $_getList(1);

  @$pb.TagNumber(3)
  NetworkInfo get connected => $_getN(2);
  @$pb.TagNumber(3)
  set connected(NetworkInfo v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasConnected() => $_has(2);
  @$pb.TagNumber(3)
  void clearConnected() => clearField(3);
  @$pb.TagNumber(3)
  NetworkInfo ensureConnected() => $_ensure(2);

  @$pb.TagNumber(4)
  $core.String get macAddress => $_getSZ(3);
  @$pb.TagNumber(4)
  set macAddress($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasMacAddress() => $_has(3);
  @$pb.TagNumber(4)
  void clearMacAddress() => clearField(4);

  @$pb.TagNumber(5)
  $core.bool get modifying => $_getBF(4);
  @$pb.TagNumber(5)
  set modifying($core.bool v) { $_setBool(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasModifying() => $_has(4);
  @$pb.TagNumber(5)
  void clearModifying() => clearField(5);
}

class Firmware extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'Firmware', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..aOS(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'version')
    ..aOS(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'build')
    ..aOS(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'number')
    ..a<$fixnum.Int64>(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'timestamp', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOS(5, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'hash')
    ..a<$fixnum.Int64>(6, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'logicalAddress', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOS(7, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'name')
    ..hasRequiredFields = false
  ;

  Firmware._() : super();
  factory Firmware({
    $core.String? version,
    $core.String? build,
    $core.String? number,
    $fixnum.Int64? timestamp,
    $core.String? hash,
    $fixnum.Int64? logicalAddress,
    $core.String? name,
  }) {
    final _result = create();
    if (version != null) {
      _result.version = version;
    }
    if (build != null) {
      _result.build = build;
    }
    if (number != null) {
      _result.number = number;
    }
    if (timestamp != null) {
      _result.timestamp = timestamp;
    }
    if (hash != null) {
      _result.hash = hash;
    }
    if (logicalAddress != null) {
      _result.logicalAddress = logicalAddress;
    }
    if (name != null) {
      _result.name = name;
    }
    return _result;
  }
  factory Firmware.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Firmware.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Firmware clone() => Firmware()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Firmware copyWith(void Function(Firmware) updates) => super.copyWith((message) => updates(message as Firmware)) as Firmware; // ignore: deprecated_member_use
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

  @$pb.TagNumber(6)
  $fixnum.Int64 get logicalAddress => $_getI64(5);
  @$pb.TagNumber(6)
  set logicalAddress($fixnum.Int64 v) { $_setInt64(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasLogicalAddress() => $_has(5);
  @$pb.TagNumber(6)
  void clearLogicalAddress() => clearField(6);

  @$pb.TagNumber(7)
  $core.String get name => $_getSZ(6);
  @$pb.TagNumber(7)
  set name($core.String v) { $_setString(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasName() => $_has(6);
  @$pb.TagNumber(7)
  void clearName() => clearField(7);
}

class Identity extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'Identity', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..aOS(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'device')
    ..aOS(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'stream')
    ..a<$core.List<$core.int>>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'deviceId', $pb.PbFieldType.OY, protoName: 'deviceId')
    ..aOS(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'firmware')
    ..aOS(5, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'build')
    ..aOS(6, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'name')
    ..a<$core.List<$core.int>>(7, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'generationId', $pb.PbFieldType.OY, protoName: 'generationId')
    ..aOS(8, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'number')
    ..hasRequiredFields = false
  ;

  Identity._() : super();
  factory Identity({
    $core.String? device,
    $core.String? stream,
    $core.List<$core.int>? deviceId,
    $core.String? firmware,
    $core.String? build,
    $core.String? name,
    $core.List<$core.int>? generationId,
    $core.String? number,
  }) {
    final _result = create();
    if (device != null) {
      _result.device = device;
    }
    if (stream != null) {
      _result.stream = stream;
    }
    if (deviceId != null) {
      _result.deviceId = deviceId;
    }
    if (firmware != null) {
      _result.firmware = firmware;
    }
    if (build != null) {
      _result.build = build;
    }
    if (name != null) {
      _result.name = name;
    }
    if (generationId != null) {
      _result.generationId = generationId;
    }
    if (number != null) {
      _result.number = number;
    }
    return _result;
  }
  factory Identity.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Identity.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Identity clone() => Identity()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Identity copyWith(void Function(Identity) updates) => super.copyWith((message) => updates(message as Identity)) as Identity; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static Identity create() => Identity._();
  Identity createEmptyInstance() => create();
  static $pb.PbList<Identity> createRepeated() => $pb.PbList<Identity>();
  @$core.pragma('dart2js:noInline')
  static Identity getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Identity>(create);
  static Identity? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get device => $_getSZ(0);
  @$pb.TagNumber(1)
  set device($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasDevice() => $_has(0);
  @$pb.TagNumber(1)
  void clearDevice() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get stream => $_getSZ(1);
  @$pb.TagNumber(2)
  set stream($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasStream() => $_has(1);
  @$pb.TagNumber(2)
  void clearStream() => clearField(2);

  @$pb.TagNumber(3)
  $core.List<$core.int> get deviceId => $_getN(2);
  @$pb.TagNumber(3)
  set deviceId($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasDeviceId() => $_has(2);
  @$pb.TagNumber(3)
  void clearDeviceId() => clearField(3);

  @$pb.TagNumber(4)
  $core.String get firmware => $_getSZ(3);
  @$pb.TagNumber(4)
  set firmware($core.String v) { $_setString(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasFirmware() => $_has(3);
  @$pb.TagNumber(4)
  void clearFirmware() => clearField(4);

  @$pb.TagNumber(5)
  $core.String get build => $_getSZ(4);
  @$pb.TagNumber(5)
  set build($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasBuild() => $_has(4);
  @$pb.TagNumber(5)
  void clearBuild() => clearField(5);

  @$pb.TagNumber(6)
  $core.String get name => $_getSZ(5);
  @$pb.TagNumber(6)
  set name($core.String v) { $_setString(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasName() => $_has(5);
  @$pb.TagNumber(6)
  void clearName() => clearField(6);

  @$pb.TagNumber(7)
  $core.List<$core.int> get generationId => $_getN(6);
  @$pb.TagNumber(7)
  set generationId($core.List<$core.int> v) { $_setBytes(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasGenerationId() => $_has(6);
  @$pb.TagNumber(7)
  void clearGenerationId() => clearField(7);

  @$pb.TagNumber(8)
  $core.String get number => $_getSZ(7);
  @$pb.TagNumber(8)
  set number($core.String v) { $_setString(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasNumber() => $_has(7);
  @$pb.TagNumber(8)
  void clearNumber() => clearField(8);
}

class ConfigureSensorQuery extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'ConfigureSensorQuery', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'id', $pb.PbFieldType.OU3)
    ..a<$core.int>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'frequency', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  ConfigureSensorQuery._() : super();
  factory ConfigureSensorQuery({
    $core.int? id,
    $core.int? frequency,
  }) {
    final _result = create();
    if (id != null) {
      _result.id = id;
    }
    if (frequency != null) {
      _result.frequency = frequency;
    }
    return _result;
  }
  factory ConfigureSensorQuery.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory ConfigureSensorQuery.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  ConfigureSensorQuery clone() => ConfigureSensorQuery()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  ConfigureSensorQuery copyWith(void Function(ConfigureSensorQuery) updates) => super.copyWith((message) => updates(message as ConfigureSensorQuery)) as ConfigureSensorQuery; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static ConfigureSensorQuery create() => ConfigureSensorQuery._();
  ConfigureSensorQuery createEmptyInstance() => create();
  static $pb.PbList<ConfigureSensorQuery> createRepeated() => $pb.PbList<ConfigureSensorQuery>();
  @$core.pragma('dart2js:noInline')
  static ConfigureSensorQuery getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<ConfigureSensorQuery>(create);
  static ConfigureSensorQuery? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get id => $_getIZ(0);
  @$pb.TagNumber(1)
  set id($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasId() => $_has(0);
  @$pb.TagNumber(1)
  void clearId() => clearField(1);

  @$pb.TagNumber(2)
  $core.int get frequency => $_getIZ(1);
  @$pb.TagNumber(2)
  set frequency($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasFrequency() => $_has(1);
  @$pb.TagNumber(2)
  void clearFrequency() => clearField(2);
}

class LiveDataPoll extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'LiveDataPoll', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'interval', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  LiveDataPoll._() : super();
  factory LiveDataPoll({
    $core.int? interval,
  }) {
    final _result = create();
    if (interval != null) {
      _result.interval = interval;
    }
    return _result;
  }
  factory LiveDataPoll.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory LiveDataPoll.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  LiveDataPoll clone() => LiveDataPoll()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  LiveDataPoll copyWith(void Function(LiveDataPoll) updates) => super.copyWith((message) => updates(message as LiveDataPoll)) as LiveDataPoll; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static LiveDataPoll create() => LiveDataPoll._();
  LiveDataPoll createEmptyInstance() => create();
  static $pb.PbList<LiveDataPoll> createRepeated() => $pb.PbList<LiveDataPoll>();
  @$core.pragma('dart2js:noInline')
  static LiveDataPoll getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<LiveDataPoll>(create);
  static LiveDataPoll? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get interval => $_getIZ(0);
  @$pb.TagNumber(1)
  set interval($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasInterval() => $_has(0);
  @$pb.TagNumber(1)
  void clearInterval() => clearField(1);
}

class LiveDataSample extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'LiveDataSample', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'sensor', $pb.PbFieldType.OU3)
    ..a<$fixnum.Int64>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'time', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.double>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'value', $pb.PbFieldType.OF)
    ..hasRequiredFields = false
  ;

  LiveDataSample._() : super();
  factory LiveDataSample({
    $core.int? sensor,
    $fixnum.Int64? time,
    $core.double? value,
  }) {
    final _result = create();
    if (sensor != null) {
      _result.sensor = sensor;
    }
    if (time != null) {
      _result.time = time;
    }
    if (value != null) {
      _result.value = value;
    }
    return _result;
  }
  factory LiveDataSample.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory LiveDataSample.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  LiveDataSample clone() => LiveDataSample()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  LiveDataSample copyWith(void Function(LiveDataSample) updates) => super.copyWith((message) => updates(message as LiveDataSample)) as LiveDataSample; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static LiveDataSample create() => LiveDataSample._();
  LiveDataSample createEmptyInstance() => create();
  static $pb.PbList<LiveDataSample> createRepeated() => $pb.PbList<LiveDataSample>();
  @$core.pragma('dart2js:noInline')
  static LiveDataSample getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<LiveDataSample>(create);
  static LiveDataSample? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get sensor => $_getIZ(0);
  @$pb.TagNumber(1)
  set sensor($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasSensor() => $_has(0);
  @$pb.TagNumber(1)
  void clearSensor() => clearField(1);

  @$pb.TagNumber(2)
  $fixnum.Int64 get time => $_getI64(1);
  @$pb.TagNumber(2)
  set time($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasTime() => $_has(1);
  @$pb.TagNumber(2)
  void clearTime() => clearField(2);

  @$pb.TagNumber(3)
  $core.double get value => $_getN(2);
  @$pb.TagNumber(3)
  set value($core.double v) { $_setFloat(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasValue() => $_has(2);
  @$pb.TagNumber(3)
  void clearValue() => clearField(3);
}

class LiveData extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'LiveData', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..pc<LiveDataSample>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'samples', $pb.PbFieldType.PM, subBuilder: LiveDataSample.create)
    ..hasRequiredFields = false
  ;

  LiveData._() : super();
  factory LiveData({
    $core.Iterable<LiveDataSample>? samples,
  }) {
    final _result = create();
    if (samples != null) {
      _result.samples.addAll(samples);
    }
    return _result;
  }
  factory LiveData.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory LiveData.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  LiveData clone() => LiveData()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  LiveData copyWith(void Function(LiveData) updates) => super.copyWith((message) => updates(message as LiveData)) as LiveData; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static LiveData create() => LiveData._();
  LiveData createEmptyInstance() => create();
  static $pb.PbList<LiveData> createRepeated() => $pb.PbList<LiveData>();
  @$core.pragma('dart2js:noInline')
  static LiveData getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<LiveData>(create);
  static LiveData? _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<LiveDataSample> get samples => $_getList(0);
}

class File extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'File', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'id', $pb.PbFieldType.OU3)
    ..a<$fixnum.Int64>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'time', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$fixnum.Int64>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'size', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.int>(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'version', $pb.PbFieldType.OU3)
    ..aOS(5, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'name')
    ..a<$fixnum.Int64>(6, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'maximum', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  File._() : super();
  factory File({
    $core.int? id,
    $fixnum.Int64? time,
    $fixnum.Int64? size,
    $core.int? version,
    $core.String? name,
    $fixnum.Int64? maximum,
  }) {
    final _result = create();
    if (id != null) {
      _result.id = id;
    }
    if (time != null) {
      _result.time = time;
    }
    if (size != null) {
      _result.size = size;
    }
    if (version != null) {
      _result.version = version;
    }
    if (name != null) {
      _result.name = name;
    }
    if (maximum != null) {
      _result.maximum = maximum;
    }
    return _result;
  }
  factory File.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory File.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  File clone() => File()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  File copyWith(void Function(File) updates) => super.copyWith((message) => updates(message as File)) as File; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static File create() => File._();
  File createEmptyInstance() => create();
  static $pb.PbList<File> createRepeated() => $pb.PbList<File>();
  @$core.pragma('dart2js:noInline')
  static File getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<File>(create);
  static File? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get id => $_getIZ(0);
  @$pb.TagNumber(1)
  set id($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasId() => $_has(0);
  @$pb.TagNumber(1)
  void clearId() => clearField(1);

  @$pb.TagNumber(2)
  $fixnum.Int64 get time => $_getI64(1);
  @$pb.TagNumber(2)
  set time($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasTime() => $_has(1);
  @$pb.TagNumber(2)
  void clearTime() => clearField(2);

  @$pb.TagNumber(3)
  $fixnum.Int64 get size => $_getI64(2);
  @$pb.TagNumber(3)
  set size($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasSize() => $_has(2);
  @$pb.TagNumber(3)
  void clearSize() => clearField(3);

  @$pb.TagNumber(4)
  $core.int get version => $_getIZ(3);
  @$pb.TagNumber(4)
  set version($core.int v) { $_setUnsignedInt32(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasVersion() => $_has(3);
  @$pb.TagNumber(4)
  void clearVersion() => clearField(4);

  @$pb.TagNumber(5)
  $core.String get name => $_getSZ(4);
  @$pb.TagNumber(5)
  set name($core.String v) { $_setString(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasName() => $_has(4);
  @$pb.TagNumber(5)
  void clearName() => clearField(5);

  @$pb.TagNumber(6)
  $fixnum.Int64 get maximum => $_getI64(5);
  @$pb.TagNumber(6)
  set maximum($fixnum.Int64 v) { $_setInt64(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasMaximum() => $_has(5);
  @$pb.TagNumber(6)
  void clearMaximum() => clearField(6);
}

class Files extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'Files', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..pc<File>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'files', $pb.PbFieldType.PM, subBuilder: File.create)
    ..hasRequiredFields = false
  ;

  Files._() : super();
  factory Files({
    $core.Iterable<File>? files,
  }) {
    final _result = create();
    if (files != null) {
      _result.files.addAll(files);
    }
    return _result;
  }
  factory Files.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Files.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Files clone() => Files()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Files copyWith(void Function(Files) updates) => super.copyWith((message) => updates(message as Files)) as Files; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static Files create() => Files._();
  Files createEmptyInstance() => create();
  static $pb.PbList<Files> createRepeated() => $pb.PbList<Files>();
  @$core.pragma('dart2js:noInline')
  static Files getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Files>(create);
  static Files? _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<File> get files => $_getList(0);
}

class DownloadFile extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'DownloadFile', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'id', $pb.PbFieldType.OU3)
    ..a<$core.int>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'offset', $pb.PbFieldType.OU3)
    ..a<$core.int>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'length', $pb.PbFieldType.OU3)
    ..a<$core.int>(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'flags', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  DownloadFile._() : super();
  factory DownloadFile({
    $core.int? id,
    $core.int? offset,
    $core.int? length,
    $core.int? flags,
  }) {
    final _result = create();
    if (id != null) {
      _result.id = id;
    }
    if (offset != null) {
      _result.offset = offset;
    }
    if (length != null) {
      _result.length = length;
    }
    if (flags != null) {
      _result.flags = flags;
    }
    return _result;
  }
  factory DownloadFile.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DownloadFile.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  DownloadFile clone() => DownloadFile()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  DownloadFile copyWith(void Function(DownloadFile) updates) => super.copyWith((message) => updates(message as DownloadFile)) as DownloadFile; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static DownloadFile create() => DownloadFile._();
  DownloadFile createEmptyInstance() => create();
  static $pb.PbList<DownloadFile> createRepeated() => $pb.PbList<DownloadFile>();
  @$core.pragma('dart2js:noInline')
  static DownloadFile getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DownloadFile>(create);
  static DownloadFile? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get id => $_getIZ(0);
  @$pb.TagNumber(1)
  set id($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasId() => $_has(0);
  @$pb.TagNumber(1)
  void clearId() => clearField(1);

  @$pb.TagNumber(2)
  $core.int get offset => $_getIZ(1);
  @$pb.TagNumber(2)
  set offset($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasOffset() => $_has(1);
  @$pb.TagNumber(2)
  void clearOffset() => clearField(2);

  @$pb.TagNumber(3)
  $core.int get length => $_getIZ(2);
  @$pb.TagNumber(3)
  set length($core.int v) { $_setUnsignedInt32(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasLength() => $_has(2);
  @$pb.TagNumber(3)
  void clearLength() => clearField(3);

  @$pb.TagNumber(4)
  $core.int get flags => $_getIZ(3);
  @$pb.TagNumber(4)
  set flags($core.int v) { $_setUnsignedInt32(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasFlags() => $_has(3);
  @$pb.TagNumber(4)
  void clearFlags() => clearField(4);
}

class EraseFile extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'EraseFile', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'id', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  EraseFile._() : super();
  factory EraseFile({
    $core.int? id,
  }) {
    final _result = create();
    if (id != null) {
      _result.id = id;
    }
    return _result;
  }
  factory EraseFile.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory EraseFile.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  EraseFile clone() => EraseFile()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  EraseFile copyWith(void Function(EraseFile) updates) => super.copyWith((message) => updates(message as EraseFile)) as EraseFile; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static EraseFile create() => EraseFile._();
  EraseFile createEmptyInstance() => create();
  static $pb.PbList<EraseFile> createRepeated() => $pb.PbList<EraseFile>();
  @$core.pragma('dart2js:noInline')
  static EraseFile getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<EraseFile>(create);
  static EraseFile? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get id => $_getIZ(0);
  @$pb.TagNumber(1)
  set id($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasId() => $_has(0);
  @$pb.TagNumber(1)
  void clearId() => clearField(1);
}

class FileData extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'FileData', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'offset', $pb.PbFieldType.OU3)
    ..a<$core.List<$core.int>>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'data', $pb.PbFieldType.OY)
    ..a<$core.int>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'size', $pb.PbFieldType.OU3)
    ..a<$core.int>(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'hash', $pb.PbFieldType.OU3)
    ..a<$core.int>(5, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'version', $pb.PbFieldType.OU3)
    ..a<$core.int>(6, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'id', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  FileData._() : super();
  factory FileData({
    $core.int? offset,
    $core.List<$core.int>? data,
    $core.int? size,
    $core.int? hash,
    $core.int? version,
    $core.int? id,
  }) {
    final _result = create();
    if (offset != null) {
      _result.offset = offset;
    }
    if (data != null) {
      _result.data = data;
    }
    if (size != null) {
      _result.size = size;
    }
    if (hash != null) {
      _result.hash = hash;
    }
    if (version != null) {
      _result.version = version;
    }
    if (id != null) {
      _result.id = id;
    }
    return _result;
  }
  factory FileData.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory FileData.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  FileData clone() => FileData()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  FileData copyWith(void Function(FileData) updates) => super.copyWith((message) => updates(message as FileData)) as FileData; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static FileData create() => FileData._();
  FileData createEmptyInstance() => create();
  static $pb.PbList<FileData> createRepeated() => $pb.PbList<FileData>();
  @$core.pragma('dart2js:noInline')
  static FileData getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<FileData>(create);
  static FileData? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get offset => $_getIZ(0);
  @$pb.TagNumber(1)
  set offset($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasOffset() => $_has(0);
  @$pb.TagNumber(1)
  void clearOffset() => clearField(1);

  @$pb.TagNumber(2)
  $core.List<$core.int> get data => $_getN(1);
  @$pb.TagNumber(2)
  set data($core.List<$core.int> v) { $_setBytes(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasData() => $_has(1);
  @$pb.TagNumber(2)
  void clearData() => clearField(2);

  @$pb.TagNumber(3)
  $core.int get size => $_getIZ(2);
  @$pb.TagNumber(3)
  set size($core.int v) { $_setUnsignedInt32(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasSize() => $_has(2);
  @$pb.TagNumber(3)
  void clearSize() => clearField(3);

  @$pb.TagNumber(4)
  $core.int get hash => $_getIZ(3);
  @$pb.TagNumber(4)
  set hash($core.int v) { $_setUnsignedInt32(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasHash() => $_has(3);
  @$pb.TagNumber(4)
  void clearHash() => clearField(4);

  @$pb.TagNumber(5)
  $core.int get version => $_getIZ(4);
  @$pb.TagNumber(5)
  set version($core.int v) { $_setUnsignedInt32(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasVersion() => $_has(4);
  @$pb.TagNumber(5)
  void clearVersion() => clearField(5);

  @$pb.TagNumber(6)
  $core.int get id => $_getIZ(5);
  @$pb.TagNumber(6)
  set id($core.int v) { $_setUnsignedInt32(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasId() => $_has(5);
  @$pb.TagNumber(6)
  void clearId() => clearField(6);
}

class DeviceStatus extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'DeviceStatus', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'uptime', $pb.PbFieldType.OU3)
    ..a<$core.double>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'batteryPercentage', $pb.PbFieldType.OF, protoName: 'batteryPercentage')
    ..a<$core.double>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'batteryVoltage', $pb.PbFieldType.OF, protoName: 'batteryVoltage')
    ..a<$core.int>(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'gpsHasFix', $pb.PbFieldType.OU3, protoName: 'gpsHasFix')
    ..a<$core.int>(5, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'gpsSatellites', $pb.PbFieldType.OU3, protoName: 'gpsSatellites')
    ..hasRequiredFields = false
  ;

  DeviceStatus._() : super();
  factory DeviceStatus({
    $core.int? uptime,
    $core.double? batteryPercentage,
    $core.double? batteryVoltage,
    $core.int? gpsHasFix,
    $core.int? gpsSatellites,
  }) {
    final _result = create();
    if (uptime != null) {
      _result.uptime = uptime;
    }
    if (batteryPercentage != null) {
      _result.batteryPercentage = batteryPercentage;
    }
    if (batteryVoltage != null) {
      _result.batteryVoltage = batteryVoltage;
    }
    if (gpsHasFix != null) {
      _result.gpsHasFix = gpsHasFix;
    }
    if (gpsSatellites != null) {
      _result.gpsSatellites = gpsSatellites;
    }
    return _result;
  }
  factory DeviceStatus.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DeviceStatus.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  DeviceStatus clone() => DeviceStatus()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  DeviceStatus copyWith(void Function(DeviceStatus) updates) => super.copyWith((message) => updates(message as DeviceStatus)) as DeviceStatus; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static DeviceStatus create() => DeviceStatus._();
  DeviceStatus createEmptyInstance() => create();
  static $pb.PbList<DeviceStatus> createRepeated() => $pb.PbList<DeviceStatus>();
  @$core.pragma('dart2js:noInline')
  static DeviceStatus getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DeviceStatus>(create);
  static DeviceStatus? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get uptime => $_getIZ(0);
  @$pb.TagNumber(1)
  set uptime($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasUptime() => $_has(0);
  @$pb.TagNumber(1)
  void clearUptime() => clearField(1);

  @$pb.TagNumber(2)
  $core.double get batteryPercentage => $_getN(1);
  @$pb.TagNumber(2)
  set batteryPercentage($core.double v) { $_setFloat(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasBatteryPercentage() => $_has(1);
  @$pb.TagNumber(2)
  void clearBatteryPercentage() => clearField(2);

  @$pb.TagNumber(3)
  $core.double get batteryVoltage => $_getN(2);
  @$pb.TagNumber(3)
  set batteryVoltage($core.double v) { $_setFloat(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasBatteryVoltage() => $_has(2);
  @$pb.TagNumber(3)
  void clearBatteryVoltage() => clearField(3);

  @$pb.TagNumber(4)
  $core.int get gpsHasFix => $_getIZ(3);
  @$pb.TagNumber(4)
  set gpsHasFix($core.int v) { $_setUnsignedInt32(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasGpsHasFix() => $_has(3);
  @$pb.TagNumber(4)
  void clearGpsHasFix() => clearField(4);

  @$pb.TagNumber(5)
  $core.int get gpsSatellites => $_getIZ(4);
  @$pb.TagNumber(5)
  set gpsSatellites($core.int v) { $_setUnsignedInt32(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasGpsSatellites() => $_has(4);
  @$pb.TagNumber(5)
  void clearGpsSatellites() => clearField(5);
}

class QueryModule extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'QueryModule', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'id', $pb.PbFieldType.OU3)
    ..a<$core.int>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'address', $pb.PbFieldType.OU3)
    ..a<$core.List<$core.int>>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'message', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  QueryModule._() : super();
  factory QueryModule({
    $core.int? id,
    $core.int? address,
    $core.List<$core.int>? message,
  }) {
    final _result = create();
    if (id != null) {
      _result.id = id;
    }
    if (address != null) {
      _result.address = address;
    }
    if (message != null) {
      _result.message = message;
    }
    return _result;
  }
  factory QueryModule.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory QueryModule.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  QueryModule clone() => QueryModule()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  QueryModule copyWith(void Function(QueryModule) updates) => super.copyWith((message) => updates(message as QueryModule)) as QueryModule; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static QueryModule create() => QueryModule._();
  QueryModule createEmptyInstance() => create();
  static $pb.PbList<QueryModule> createRepeated() => $pb.PbList<QueryModule>();
  @$core.pragma('dart2js:noInline')
  static QueryModule getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<QueryModule>(create);
  static QueryModule? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get id => $_getIZ(0);
  @$pb.TagNumber(1)
  set id($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasId() => $_has(0);
  @$pb.TagNumber(1)
  void clearId() => clearField(1);

  @$pb.TagNumber(2)
  $core.int get address => $_getIZ(1);
  @$pb.TagNumber(2)
  set address($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearAddress() => clearField(2);

  @$pb.TagNumber(3)
  $core.List<$core.int> get message => $_getN(2);
  @$pb.TagNumber(3)
  set message($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasMessage() => $_has(2);
  @$pb.TagNumber(3)
  void clearMessage() => clearField(3);
}

class ModuleReply extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'ModuleReply', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'id', $pb.PbFieldType.OU3)
    ..a<$core.int>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'address', $pb.PbFieldType.OU3)
    ..a<$core.List<$core.int>>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'message', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  ModuleReply._() : super();
  factory ModuleReply({
    $core.int? id,
    $core.int? address,
    $core.List<$core.int>? message,
  }) {
    final _result = create();
    if (id != null) {
      _result.id = id;
    }
    if (address != null) {
      _result.address = address;
    }
    if (message != null) {
      _result.message = message;
    }
    return _result;
  }
  factory ModuleReply.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory ModuleReply.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  ModuleReply clone() => ModuleReply()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  ModuleReply copyWith(void Function(ModuleReply) updates) => super.copyWith((message) => updates(message as ModuleReply)) as ModuleReply; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static ModuleReply create() => ModuleReply._();
  ModuleReply createEmptyInstance() => create();
  static $pb.PbList<ModuleReply> createRepeated() => $pb.PbList<ModuleReply>();
  @$core.pragma('dart2js:noInline')
  static ModuleReply getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<ModuleReply>(create);
  static ModuleReply? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get id => $_getIZ(0);
  @$pb.TagNumber(1)
  set id($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasId() => $_has(0);
  @$pb.TagNumber(1)
  void clearId() => clearField(1);

  @$pb.TagNumber(2)
  $core.int get address => $_getIZ(1);
  @$pb.TagNumber(2)
  set address($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasAddress() => $_has(1);
  @$pb.TagNumber(2)
  void clearAddress() => clearField(2);

  @$pb.TagNumber(3)
  $core.List<$core.int> get message => $_getN(2);
  @$pb.TagNumber(3)
  set message($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasMessage() => $_has(2);
  @$pb.TagNumber(3)
  void clearMessage() => clearField(3);
}

class WireMessageQuery extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'WireMessageQuery', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..e<QueryType>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'type', $pb.PbFieldType.OE, defaultOrMaker: QueryType.QUERY_NONE, valueOf: QueryType.valueOf, enumValues: QueryType.values)
    ..aOM<QueryCapabilities>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'queryCapabilities', protoName: 'queryCapabilities', subBuilder: QueryCapabilities.create)
    ..aOM<ConfigureSensorQuery>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'configureSensor', protoName: 'configureSensor', subBuilder: ConfigureSensorQuery.create)
    ..aOM<LiveDataPoll>(8, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'liveDataPoll', protoName: 'liveDataPoll', subBuilder: LiveDataPoll.create)
    ..aOM<DownloadFile>(10, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'downloadFile', protoName: 'downloadFile', subBuilder: DownloadFile.create)
    ..aOM<EraseFile>(11, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'eraseFile', protoName: 'eraseFile', subBuilder: EraseFile.create)
    ..aOM<NetworkSettings>(12, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'networkSettings', protoName: 'networkSettings', subBuilder: NetworkSettings.create)
    ..aOM<Identity>(13, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'identity', subBuilder: Identity.create)
    ..aOM<QueryModule>(14, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'module', subBuilder: QueryModule.create)
    ..hasRequiredFields = false
  ;

  WireMessageQuery._() : super();
  factory WireMessageQuery({
    QueryType? type,
    QueryCapabilities? queryCapabilities,
    ConfigureSensorQuery? configureSensor,
    LiveDataPoll? liveDataPoll,
    DownloadFile? downloadFile,
    EraseFile? eraseFile,
    NetworkSettings? networkSettings,
    Identity? identity,
    QueryModule? module,
  }) {
    final _result = create();
    if (type != null) {
      _result.type = type;
    }
    if (queryCapabilities != null) {
      _result.queryCapabilities = queryCapabilities;
    }
    if (configureSensor != null) {
      _result.configureSensor = configureSensor;
    }
    if (liveDataPoll != null) {
      _result.liveDataPoll = liveDataPoll;
    }
    if (downloadFile != null) {
      _result.downloadFile = downloadFile;
    }
    if (eraseFile != null) {
      _result.eraseFile = eraseFile;
    }
    if (networkSettings != null) {
      _result.networkSettings = networkSettings;
    }
    if (identity != null) {
      _result.identity = identity;
    }
    if (module != null) {
      _result.module = module;
    }
    return _result;
  }
  factory WireMessageQuery.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory WireMessageQuery.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  WireMessageQuery clone() => WireMessageQuery()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  WireMessageQuery copyWith(void Function(WireMessageQuery) updates) => super.copyWith((message) => updates(message as WireMessageQuery)) as WireMessageQuery; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static WireMessageQuery create() => WireMessageQuery._();
  WireMessageQuery createEmptyInstance() => create();
  static $pb.PbList<WireMessageQuery> createRepeated() => $pb.PbList<WireMessageQuery>();
  @$core.pragma('dart2js:noInline')
  static WireMessageQuery getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<WireMessageQuery>(create);
  static WireMessageQuery? _defaultInstance;

  @$pb.TagNumber(1)
  QueryType get type => $_getN(0);
  @$pb.TagNumber(1)
  set type(QueryType v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasType() => $_has(0);
  @$pb.TagNumber(1)
  void clearType() => clearField(1);

  @$pb.TagNumber(2)
  QueryCapabilities get queryCapabilities => $_getN(1);
  @$pb.TagNumber(2)
  set queryCapabilities(QueryCapabilities v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasQueryCapabilities() => $_has(1);
  @$pb.TagNumber(2)
  void clearQueryCapabilities() => clearField(2);
  @$pb.TagNumber(2)
  QueryCapabilities ensureQueryCapabilities() => $_ensure(1);

  @$pb.TagNumber(3)
  ConfigureSensorQuery get configureSensor => $_getN(2);
  @$pb.TagNumber(3)
  set configureSensor(ConfigureSensorQuery v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasConfigureSensor() => $_has(2);
  @$pb.TagNumber(3)
  void clearConfigureSensor() => clearField(3);
  @$pb.TagNumber(3)
  ConfigureSensorQuery ensureConfigureSensor() => $_ensure(2);

  @$pb.TagNumber(8)
  LiveDataPoll get liveDataPoll => $_getN(3);
  @$pb.TagNumber(8)
  set liveDataPoll(LiveDataPoll v) { setField(8, v); }
  @$pb.TagNumber(8)
  $core.bool hasLiveDataPoll() => $_has(3);
  @$pb.TagNumber(8)
  void clearLiveDataPoll() => clearField(8);
  @$pb.TagNumber(8)
  LiveDataPoll ensureLiveDataPoll() => $_ensure(3);

  @$pb.TagNumber(10)
  DownloadFile get downloadFile => $_getN(4);
  @$pb.TagNumber(10)
  set downloadFile(DownloadFile v) { setField(10, v); }
  @$pb.TagNumber(10)
  $core.bool hasDownloadFile() => $_has(4);
  @$pb.TagNumber(10)
  void clearDownloadFile() => clearField(10);
  @$pb.TagNumber(10)
  DownloadFile ensureDownloadFile() => $_ensure(4);

  @$pb.TagNumber(11)
  EraseFile get eraseFile => $_getN(5);
  @$pb.TagNumber(11)
  set eraseFile(EraseFile v) { setField(11, v); }
  @$pb.TagNumber(11)
  $core.bool hasEraseFile() => $_has(5);
  @$pb.TagNumber(11)
  void clearEraseFile() => clearField(11);
  @$pb.TagNumber(11)
  EraseFile ensureEraseFile() => $_ensure(5);

  @$pb.TagNumber(12)
  NetworkSettings get networkSettings => $_getN(6);
  @$pb.TagNumber(12)
  set networkSettings(NetworkSettings v) { setField(12, v); }
  @$pb.TagNumber(12)
  $core.bool hasNetworkSettings() => $_has(6);
  @$pb.TagNumber(12)
  void clearNetworkSettings() => clearField(12);
  @$pb.TagNumber(12)
  NetworkSettings ensureNetworkSettings() => $_ensure(6);

  @$pb.TagNumber(13)
  Identity get identity => $_getN(7);
  @$pb.TagNumber(13)
  set identity(Identity v) { setField(13, v); }
  @$pb.TagNumber(13)
  $core.bool hasIdentity() => $_has(7);
  @$pb.TagNumber(13)
  void clearIdentity() => clearField(13);
  @$pb.TagNumber(13)
  Identity ensureIdentity() => $_ensure(7);

  @$pb.TagNumber(14)
  QueryModule get module => $_getN(8);
  @$pb.TagNumber(14)
  set module(QueryModule v) { setField(14, v); }
  @$pb.TagNumber(14)
  $core.bool hasModule() => $_has(8);
  @$pb.TagNumber(14)
  void clearModule() => clearField(14);
  @$pb.TagNumber(14)
  QueryModule ensureModule() => $_ensure(8);
}

class Error extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'Error', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..aOS(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'message')
    ..a<$core.int>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'delay', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  Error._() : super();
  factory Error({
    $core.String? message,
    $core.int? delay,
  }) {
    final _result = create();
    if (message != null) {
      _result.message = message;
    }
    if (delay != null) {
      _result.delay = delay;
    }
    return _result;
  }
  factory Error.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Error.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Error clone() => Error()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Error copyWith(void Function(Error) updates) => super.copyWith((message) => updates(message as Error)) as Error; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static Error create() => Error._();
  Error createEmptyInstance() => create();
  static $pb.PbList<Error> createRepeated() => $pb.PbList<Error>();
  @$core.pragma('dart2js:noInline')
  static Error getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Error>(create);
  static Error? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get message => $_getSZ(0);
  @$pb.TagNumber(1)
  set message($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasMessage() => $_has(0);
  @$pb.TagNumber(1)
  void clearMessage() => clearField(1);

  @$pb.TagNumber(2)
  $core.int get delay => $_getIZ(1);
  @$pb.TagNumber(2)
  set delay($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasDelay() => $_has(1);
  @$pb.TagNumber(2)
  void clearDelay() => clearField(2);
}

class WireMessageReply extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'WireMessageReply', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..e<ReplyType>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'type', $pb.PbFieldType.OE, defaultOrMaker: ReplyType.REPLY_NONE, valueOf: ReplyType.valueOf, enumValues: ReplyType.values)
    ..pc<Error>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'errors', $pb.PbFieldType.PM, subBuilder: Error.create)
    ..aOM<Capabilities>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'capabilities', subBuilder: Capabilities.create)
    ..aOM<LiveData>(6, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'liveData', protoName: 'liveData', subBuilder: LiveData.create)
    ..aOM<Files>(8, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'files', subBuilder: Files.create)
    ..aOM<FileData>(9, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'fileData', protoName: 'fileData', subBuilder: FileData.create)
    ..aOM<NetworkSettings>(10, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'networkSettings', protoName: 'networkSettings', subBuilder: NetworkSettings.create)
    ..aOM<Identity>(11, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'identity', subBuilder: Identity.create)
    ..aOM<DeviceStatus>(12, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'status', subBuilder: DeviceStatus.create)
    ..aOM<ModuleReply>(13, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'module', subBuilder: ModuleReply.create)
    ..hasRequiredFields = false
  ;

  WireMessageReply._() : super();
  factory WireMessageReply({
    ReplyType? type,
    $core.Iterable<Error>? errors,
    Capabilities? capabilities,
    LiveData? liveData,
    Files? files,
    FileData? fileData,
    NetworkSettings? networkSettings,
    Identity? identity,
    DeviceStatus? status,
    ModuleReply? module,
  }) {
    final _result = create();
    if (type != null) {
      _result.type = type;
    }
    if (errors != null) {
      _result.errors.addAll(errors);
    }
    if (capabilities != null) {
      _result.capabilities = capabilities;
    }
    if (liveData != null) {
      _result.liveData = liveData;
    }
    if (files != null) {
      _result.files = files;
    }
    if (fileData != null) {
      _result.fileData = fileData;
    }
    if (networkSettings != null) {
      _result.networkSettings = networkSettings;
    }
    if (identity != null) {
      _result.identity = identity;
    }
    if (status != null) {
      _result.status = status;
    }
    if (module != null) {
      _result.module = module;
    }
    return _result;
  }
  factory WireMessageReply.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory WireMessageReply.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  WireMessageReply clone() => WireMessageReply()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  WireMessageReply copyWith(void Function(WireMessageReply) updates) => super.copyWith((message) => updates(message as WireMessageReply)) as WireMessageReply; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static WireMessageReply create() => WireMessageReply._();
  WireMessageReply createEmptyInstance() => create();
  static $pb.PbList<WireMessageReply> createRepeated() => $pb.PbList<WireMessageReply>();
  @$core.pragma('dart2js:noInline')
  static WireMessageReply getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<WireMessageReply>(create);
  static WireMessageReply? _defaultInstance;

  @$pb.TagNumber(1)
  ReplyType get type => $_getN(0);
  @$pb.TagNumber(1)
  set type(ReplyType v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasType() => $_has(0);
  @$pb.TagNumber(1)
  void clearType() => clearField(1);

  @$pb.TagNumber(2)
  $core.List<Error> get errors => $_getList(1);

  @$pb.TagNumber(3)
  Capabilities get capabilities => $_getN(2);
  @$pb.TagNumber(3)
  set capabilities(Capabilities v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasCapabilities() => $_has(2);
  @$pb.TagNumber(3)
  void clearCapabilities() => clearField(3);
  @$pb.TagNumber(3)
  Capabilities ensureCapabilities() => $_ensure(2);

  @$pb.TagNumber(6)
  LiveData get liveData => $_getN(3);
  @$pb.TagNumber(6)
  set liveData(LiveData v) { setField(6, v); }
  @$pb.TagNumber(6)
  $core.bool hasLiveData() => $_has(3);
  @$pb.TagNumber(6)
  void clearLiveData() => clearField(6);
  @$pb.TagNumber(6)
  LiveData ensureLiveData() => $_ensure(3);

  @$pb.TagNumber(8)
  Files get files => $_getN(4);
  @$pb.TagNumber(8)
  set files(Files v) { setField(8, v); }
  @$pb.TagNumber(8)
  $core.bool hasFiles() => $_has(4);
  @$pb.TagNumber(8)
  void clearFiles() => clearField(8);
  @$pb.TagNumber(8)
  Files ensureFiles() => $_ensure(4);

  @$pb.TagNumber(9)
  FileData get fileData => $_getN(5);
  @$pb.TagNumber(9)
  set fileData(FileData v) { setField(9, v); }
  @$pb.TagNumber(9)
  $core.bool hasFileData() => $_has(5);
  @$pb.TagNumber(9)
  void clearFileData() => clearField(9);
  @$pb.TagNumber(9)
  FileData ensureFileData() => $_ensure(5);

  @$pb.TagNumber(10)
  NetworkSettings get networkSettings => $_getN(6);
  @$pb.TagNumber(10)
  set networkSettings(NetworkSettings v) { setField(10, v); }
  @$pb.TagNumber(10)
  $core.bool hasNetworkSettings() => $_has(6);
  @$pb.TagNumber(10)
  void clearNetworkSettings() => clearField(10);
  @$pb.TagNumber(10)
  NetworkSettings ensureNetworkSettings() => $_ensure(6);

  @$pb.TagNumber(11)
  Identity get identity => $_getN(7);
  @$pb.TagNumber(11)
  set identity(Identity v) { setField(11, v); }
  @$pb.TagNumber(11)
  $core.bool hasIdentity() => $_has(7);
  @$pb.TagNumber(11)
  void clearIdentity() => clearField(11);
  @$pb.TagNumber(11)
  Identity ensureIdentity() => $_ensure(7);

  @$pb.TagNumber(12)
  DeviceStatus get status => $_getN(8);
  @$pb.TagNumber(12)
  set status(DeviceStatus v) { setField(12, v); }
  @$pb.TagNumber(12)
  $core.bool hasStatus() => $_has(8);
  @$pb.TagNumber(12)
  void clearStatus() => clearField(12);
  @$pb.TagNumber(12)
  DeviceStatus ensureStatus() => $_ensure(8);

  @$pb.TagNumber(13)
  ModuleReply get module => $_getN(9);
  @$pb.TagNumber(13)
  set module(ModuleReply v) { setField(13, v); }
  @$pb.TagNumber(13)
  $core.bool hasModule() => $_has(9);
  @$pb.TagNumber(13)
  void clearModule() => clearField(13);
  @$pb.TagNumber(13)
  ModuleReply ensureModule() => $_ensure(9);
}

class Interval extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'Interval', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$fixnum.Int64>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'start', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$fixnum.Int64>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'end', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.int>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'interval', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  Interval._() : super();
  factory Interval({
    $fixnum.Int64? start,
    $fixnum.Int64? end,
    $core.int? interval,
  }) {
    final _result = create();
    if (start != null) {
      _result.start = start;
    }
    if (end != null) {
      _result.end = end;
    }
    if (interval != null) {
      _result.interval = interval;
    }
    return _result;
  }
  factory Interval.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Interval.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Interval clone() => Interval()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Interval copyWith(void Function(Interval) updates) => super.copyWith((message) => updates(message as Interval)) as Interval; // ignore: deprecated_member_use
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

class Schedule extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'Schedule', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'cron', $pb.PbFieldType.OY)
    ..a<$core.int>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'interval', $pb.PbFieldType.OU3)
    ..a<$core.int>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'repeated', $pb.PbFieldType.OU3)
    ..a<$core.int>(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'duration', $pb.PbFieldType.OU3)
    ..a<$core.int>(5, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'jitter', $pb.PbFieldType.OU3)
    ..pc<Interval>(6, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'intervals', $pb.PbFieldType.PM, subBuilder: Interval.create)
    ..hasRequiredFields = false
  ;

  Schedule._() : super();
  factory Schedule({
    $core.List<$core.int>? cron,
    $core.int? interval,
    $core.int? repeated,
    $core.int? duration,
    $core.int? jitter,
    $core.Iterable<Interval>? intervals,
  }) {
    final _result = create();
    if (cron != null) {
      _result.cron = cron;
    }
    if (interval != null) {
      _result.interval = interval;
    }
    if (repeated != null) {
      _result.repeated = repeated;
    }
    if (duration != null) {
      _result.duration = duration;
    }
    if (jitter != null) {
      _result.jitter = jitter;
    }
    if (intervals != null) {
      _result.intervals.addAll(intervals);
    }
    return _result;
  }
  factory Schedule.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Schedule.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Schedule clone() => Schedule()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Schedule copyWith(void Function(Schedule) updates) => super.copyWith((message) => updates(message as Schedule)) as Schedule; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static Schedule create() => Schedule._();
  Schedule createEmptyInstance() => create();
  static $pb.PbList<Schedule> createRepeated() => $pb.PbList<Schedule>();
  @$core.pragma('dart2js:noInline')
  static Schedule getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Schedule>(create);
  static Schedule? _defaultInstance;

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

class Schedules extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'Schedules', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..aOB(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'modifying')
    ..aOM<Schedule>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'readings', subBuilder: Schedule.create)
    ..aOM<Schedule>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'lora', subBuilder: Schedule.create)
    ..aOM<Schedule>(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'network', subBuilder: Schedule.create)
    ..aOM<Schedule>(5, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'gps', subBuilder: Schedule.create)
    ..hasRequiredFields = false
  ;

  Schedules._() : super();
  factory Schedules({
    $core.bool? modifying,
    Schedule? readings,
    Schedule? lora,
    Schedule? network,
    Schedule? gps,
  }) {
    final _result = create();
    if (modifying != null) {
      _result.modifying = modifying;
    }
    if (readings != null) {
      _result.readings = readings;
    }
    if (lora != null) {
      _result.lora = lora;
    }
    if (network != null) {
      _result.network = network;
    }
    if (gps != null) {
      _result.gps = gps;
    }
    return _result;
  }
  factory Schedules.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Schedules.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Schedules clone() => Schedules()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Schedules copyWith(void Function(Schedules) updates) => super.copyWith((message) => updates(message as Schedules)) as Schedules; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static Schedules create() => Schedules._();
  Schedules createEmptyInstance() => create();
  static $pb.PbList<Schedules> createRepeated() => $pb.PbList<Schedules>();
  @$core.pragma('dart2js:noInline')
  static Schedules getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Schedules>(create);
  static Schedules? _defaultInstance;

  @$pb.TagNumber(1)
  $core.bool get modifying => $_getBF(0);
  @$pb.TagNumber(1)
  set modifying($core.bool v) { $_setBool(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasModifying() => $_has(0);
  @$pb.TagNumber(1)
  void clearModifying() => clearField(1);

  @$pb.TagNumber(2)
  Schedule get readings => $_getN(1);
  @$pb.TagNumber(2)
  set readings(Schedule v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasReadings() => $_has(1);
  @$pb.TagNumber(2)
  void clearReadings() => clearField(2);
  @$pb.TagNumber(2)
  Schedule ensureReadings() => $_ensure(1);

  @$pb.TagNumber(3)
  Schedule get lora => $_getN(2);
  @$pb.TagNumber(3)
  set lora(Schedule v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasLora() => $_has(2);
  @$pb.TagNumber(3)
  void clearLora() => clearField(3);
  @$pb.TagNumber(3)
  Schedule ensureLora() => $_ensure(2);

  @$pb.TagNumber(4)
  Schedule get network => $_getN(3);
  @$pb.TagNumber(4)
  set network(Schedule v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasNetwork() => $_has(3);
  @$pb.TagNumber(4)
  void clearNetwork() => clearField(4);
  @$pb.TagNumber(4)
  Schedule ensureNetwork() => $_ensure(3);

  @$pb.TagNumber(5)
  Schedule get gps => $_getN(4);
  @$pb.TagNumber(5)
  set gps(Schedule v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasGps() => $_has(4);
  @$pb.TagNumber(5)
  void clearGps() => clearField(5);
  @$pb.TagNumber(5)
  Schedule ensureGps() => $_ensure(4);
}

class HardwareStatus extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'HardwareStatus', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..hasRequiredFields = false
  ;

  HardwareStatus._() : super();
  factory HardwareStatus() => create();
  factory HardwareStatus.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory HardwareStatus.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  HardwareStatus clone() => HardwareStatus()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  HardwareStatus copyWith(void Function(HardwareStatus) updates) => super.copyWith((message) => updates(message as HardwareStatus)) as HardwareStatus; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static HardwareStatus create() => HardwareStatus._();
  HardwareStatus createEmptyInstance() => create();
  static $pb.PbList<HardwareStatus> createRepeated() => $pb.PbList<HardwareStatus>();
  @$core.pragma('dart2js:noInline')
  static HardwareStatus getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<HardwareStatus>(create);
  static HardwareStatus? _defaultInstance;
}

class GpsStatus extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'GpsStatus', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'fix', $pb.PbFieldType.OU3)
    ..a<$fixnum.Int64>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'time', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.int>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'satellites', $pb.PbFieldType.OU3)
    ..a<$core.double>(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'longitude', $pb.PbFieldType.OF)
    ..a<$core.double>(5, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'latitude', $pb.PbFieldType.OF)
    ..a<$core.double>(6, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'altitude', $pb.PbFieldType.OF)
    ..a<$core.int>(7, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'enabled', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  GpsStatus._() : super();
  factory GpsStatus({
    $core.int? fix,
    $fixnum.Int64? time,
    $core.int? satellites,
    $core.double? longitude,
    $core.double? latitude,
    $core.double? altitude,
    $core.int? enabled,
  }) {
    final _result = create();
    if (fix != null) {
      _result.fix = fix;
    }
    if (time != null) {
      _result.time = time;
    }
    if (satellites != null) {
      _result.satellites = satellites;
    }
    if (longitude != null) {
      _result.longitude = longitude;
    }
    if (latitude != null) {
      _result.latitude = latitude;
    }
    if (altitude != null) {
      _result.altitude = altitude;
    }
    if (enabled != null) {
      _result.enabled = enabled;
    }
    return _result;
  }
  factory GpsStatus.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory GpsStatus.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  GpsStatus clone() => GpsStatus()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  GpsStatus copyWith(void Function(GpsStatus) updates) => super.copyWith((message) => updates(message as GpsStatus)) as GpsStatus; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static GpsStatus create() => GpsStatus._();
  GpsStatus createEmptyInstance() => create();
  static $pb.PbList<GpsStatus> createRepeated() => $pb.PbList<GpsStatus>();
  @$core.pragma('dart2js:noInline')
  static GpsStatus getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<GpsStatus>(create);
  static GpsStatus? _defaultInstance;

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
  $core.int get satellites => $_getIZ(2);
  @$pb.TagNumber(3)
  set satellites($core.int v) { $_setUnsignedInt32(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasSatellites() => $_has(2);
  @$pb.TagNumber(3)
  void clearSatellites() => clearField(3);

  @$pb.TagNumber(4)
  $core.double get longitude => $_getN(3);
  @$pb.TagNumber(4)
  set longitude($core.double v) { $_setFloat(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasLongitude() => $_has(3);
  @$pb.TagNumber(4)
  void clearLongitude() => clearField(4);

  @$pb.TagNumber(5)
  $core.double get latitude => $_getN(4);
  @$pb.TagNumber(5)
  set latitude($core.double v) { $_setFloat(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasLatitude() => $_has(4);
  @$pb.TagNumber(5)
  void clearLatitude() => clearField(5);

  @$pb.TagNumber(6)
  $core.double get altitude => $_getN(5);
  @$pb.TagNumber(6)
  set altitude($core.double v) { $_setFloat(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasAltitude() => $_has(5);
  @$pb.TagNumber(6)
  void clearAltitude() => clearField(6);

  @$pb.TagNumber(7)
  $core.int get enabled => $_getIZ(6);
  @$pb.TagNumber(7)
  set enabled($core.int v) { $_setUnsignedInt32(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasEnabled() => $_has(6);
  @$pb.TagNumber(7)
  void clearEnabled() => clearField(7);
}

class MemoryStatus extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'MemoryStatus', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'sramAvailable', $pb.PbFieldType.OU3, protoName: 'sramAvailable')
    ..a<$core.int>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'programFlashAvailable', $pb.PbFieldType.OU3, protoName: 'programFlashAvailable')
    ..a<$core.int>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'extendedMemoryAvailable', $pb.PbFieldType.OU3, protoName: 'extendedMemoryAvailable')
    ..a<$core.int>(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'dataMemoryInstalled', $pb.PbFieldType.OU3, protoName: 'dataMemoryInstalled')
    ..a<$core.int>(5, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'dataMemoryUsed', $pb.PbFieldType.OU3, protoName: 'dataMemoryUsed')
    ..a<$core.double>(6, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'dataMemoryConsumption', $pb.PbFieldType.OF, protoName: 'dataMemoryConsumption')
    ..pc<Firmware>(7, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'firmware', $pb.PbFieldType.PM, subBuilder: Firmware.create)
    ..hasRequiredFields = false
  ;

  MemoryStatus._() : super();
  factory MemoryStatus({
    $core.int? sramAvailable,
    $core.int? programFlashAvailable,
    $core.int? extendedMemoryAvailable,
    $core.int? dataMemoryInstalled,
    $core.int? dataMemoryUsed,
    $core.double? dataMemoryConsumption,
    $core.Iterable<Firmware>? firmware,
  }) {
    final _result = create();
    if (sramAvailable != null) {
      _result.sramAvailable = sramAvailable;
    }
    if (programFlashAvailable != null) {
      _result.programFlashAvailable = programFlashAvailable;
    }
    if (extendedMemoryAvailable != null) {
      _result.extendedMemoryAvailable = extendedMemoryAvailable;
    }
    if (dataMemoryInstalled != null) {
      _result.dataMemoryInstalled = dataMemoryInstalled;
    }
    if (dataMemoryUsed != null) {
      _result.dataMemoryUsed = dataMemoryUsed;
    }
    if (dataMemoryConsumption != null) {
      _result.dataMemoryConsumption = dataMemoryConsumption;
    }
    if (firmware != null) {
      _result.firmware.addAll(firmware);
    }
    return _result;
  }
  factory MemoryStatus.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory MemoryStatus.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  MemoryStatus clone() => MemoryStatus()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  MemoryStatus copyWith(void Function(MemoryStatus) updates) => super.copyWith((message) => updates(message as MemoryStatus)) as MemoryStatus; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static MemoryStatus create() => MemoryStatus._();
  MemoryStatus createEmptyInstance() => create();
  static $pb.PbList<MemoryStatus> createRepeated() => $pb.PbList<MemoryStatus>();
  @$core.pragma('dart2js:noInline')
  static MemoryStatus getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<MemoryStatus>(create);
  static MemoryStatus? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get sramAvailable => $_getIZ(0);
  @$pb.TagNumber(1)
  set sramAvailable($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasSramAvailable() => $_has(0);
  @$pb.TagNumber(1)
  void clearSramAvailable() => clearField(1);

  @$pb.TagNumber(2)
  $core.int get programFlashAvailable => $_getIZ(1);
  @$pb.TagNumber(2)
  set programFlashAvailable($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasProgramFlashAvailable() => $_has(1);
  @$pb.TagNumber(2)
  void clearProgramFlashAvailable() => clearField(2);

  @$pb.TagNumber(3)
  $core.int get extendedMemoryAvailable => $_getIZ(2);
  @$pb.TagNumber(3)
  set extendedMemoryAvailable($core.int v) { $_setUnsignedInt32(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasExtendedMemoryAvailable() => $_has(2);
  @$pb.TagNumber(3)
  void clearExtendedMemoryAvailable() => clearField(3);

  @$pb.TagNumber(4)
  $core.int get dataMemoryInstalled => $_getIZ(3);
  @$pb.TagNumber(4)
  set dataMemoryInstalled($core.int v) { $_setUnsignedInt32(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasDataMemoryInstalled() => $_has(3);
  @$pb.TagNumber(4)
  void clearDataMemoryInstalled() => clearField(4);

  @$pb.TagNumber(5)
  $core.int get dataMemoryUsed => $_getIZ(4);
  @$pb.TagNumber(5)
  set dataMemoryUsed($core.int v) { $_setUnsignedInt32(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasDataMemoryUsed() => $_has(4);
  @$pb.TagNumber(5)
  void clearDataMemoryUsed() => clearField(5);

  @$pb.TagNumber(6)
  $core.double get dataMemoryConsumption => $_getN(5);
  @$pb.TagNumber(6)
  set dataMemoryConsumption($core.double v) { $_setFloat(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasDataMemoryConsumption() => $_has(5);
  @$pb.TagNumber(6)
  void clearDataMemoryConsumption() => clearField(6);

  @$pb.TagNumber(7)
  $core.List<Firmware> get firmware => $_getList(6);
}

class BatteryStatus extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'BatteryStatus', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'voltage', $pb.PbFieldType.OU3)
    ..a<$core.int>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'percentage', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  BatteryStatus._() : super();
  factory BatteryStatus({
    $core.int? voltage,
    $core.int? percentage,
  }) {
    final _result = create();
    if (voltage != null) {
      _result.voltage = voltage;
    }
    if (percentage != null) {
      _result.percentage = percentage;
    }
    return _result;
  }
  factory BatteryStatus.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory BatteryStatus.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  BatteryStatus clone() => BatteryStatus()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  BatteryStatus copyWith(void Function(BatteryStatus) updates) => super.copyWith((message) => updates(message as BatteryStatus)) as BatteryStatus; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static BatteryStatus create() => BatteryStatus._();
  BatteryStatus createEmptyInstance() => create();
  static $pb.PbList<BatteryStatus> createRepeated() => $pb.PbList<BatteryStatus>();
  @$core.pragma('dart2js:noInline')
  static BatteryStatus getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<BatteryStatus>(create);
  static BatteryStatus? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get voltage => $_getIZ(0);
  @$pb.TagNumber(1)
  set voltage($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasVoltage() => $_has(0);
  @$pb.TagNumber(1)
  void clearVoltage() => clearField(1);

  @$pb.TagNumber(2)
  $core.int get percentage => $_getIZ(1);
  @$pb.TagNumber(2)
  set percentage($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasPercentage() => $_has(1);
  @$pb.TagNumber(2)
  void clearPercentage() => clearField(2);
}

class SolarStatus extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'SolarStatus', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'voltage', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  SolarStatus._() : super();
  factory SolarStatus({
    $core.int? voltage,
  }) {
    final _result = create();
    if (voltage != null) {
      _result.voltage = voltage;
    }
    return _result;
  }
  factory SolarStatus.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory SolarStatus.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  SolarStatus clone() => SolarStatus()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  SolarStatus copyWith(void Function(SolarStatus) updates) => super.copyWith((message) => updates(message as SolarStatus)) as SolarStatus; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static SolarStatus create() => SolarStatus._();
  SolarStatus createEmptyInstance() => create();
  static $pb.PbList<SolarStatus> createRepeated() => $pb.PbList<SolarStatus>();
  @$core.pragma('dart2js:noInline')
  static SolarStatus getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<SolarStatus>(create);
  static SolarStatus? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get voltage => $_getIZ(0);
  @$pb.TagNumber(1)
  set voltage($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasVoltage() => $_has(0);
  @$pb.TagNumber(1)
  void clearVoltage() => clearField(1);
}

class PowerStatus extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'PowerStatus', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..aOM<BatteryStatus>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'battery', subBuilder: BatteryStatus.create)
    ..aOM<SolarStatus>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'solar', subBuilder: SolarStatus.create)
    ..hasRequiredFields = false
  ;

  PowerStatus._() : super();
  factory PowerStatus({
    BatteryStatus? battery,
    SolarStatus? solar,
  }) {
    final _result = create();
    if (battery != null) {
      _result.battery = battery;
    }
    if (solar != null) {
      _result.solar = solar;
    }
    return _result;
  }
  factory PowerStatus.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory PowerStatus.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  PowerStatus clone() => PowerStatus()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  PowerStatus copyWith(void Function(PowerStatus) updates) => super.copyWith((message) => updates(message as PowerStatus)) as PowerStatus; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static PowerStatus create() => PowerStatus._();
  PowerStatus createEmptyInstance() => create();
  static $pb.PbList<PowerStatus> createRepeated() => $pb.PbList<PowerStatus>();
  @$core.pragma('dart2js:noInline')
  static PowerStatus getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<PowerStatus>(create);
  static PowerStatus? _defaultInstance;

  @$pb.TagNumber(1)
  BatteryStatus get battery => $_getN(0);
  @$pb.TagNumber(1)
  set battery(BatteryStatus v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasBattery() => $_has(0);
  @$pb.TagNumber(1)
  void clearBattery() => clearField(1);
  @$pb.TagNumber(1)
  BatteryStatus ensureBattery() => $_ensure(0);

  @$pb.TagNumber(2)
  SolarStatus get solar => $_getN(1);
  @$pb.TagNumber(2)
  set solar(SolarStatus v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasSolar() => $_has(1);
  @$pb.TagNumber(2)
  void clearSolar() => clearField(2);
  @$pb.TagNumber(2)
  SolarStatus ensureSolar() => $_ensure(1);
}

class Status extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'Status', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'version', $pb.PbFieldType.OU3)
    ..a<$core.int>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'uptime', $pb.PbFieldType.OU3)
    ..aOM<Identity>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'identity', subBuilder: Identity.create)
    ..aOM<HardwareStatus>(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'hardware', subBuilder: HardwareStatus.create)
    ..aOM<PowerStatus>(5, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'power', subBuilder: PowerStatus.create)
    ..aOM<MemoryStatus>(6, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'memory', subBuilder: MemoryStatus.create)
    ..aOM<GpsStatus>(7, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'gps', subBuilder: GpsStatus.create)
    ..aOM<Schedules>(8, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'schedules', subBuilder: Schedules.create)
    ..aOM<Recording>(9, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'recording', subBuilder: Recording.create)
    ..aOM<NetworkSettings>(10, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'network', subBuilder: NetworkSettings.create)
    ..a<$fixnum.Int64>(11, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'time', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOM<Firmware>(12, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'firmware', subBuilder: Firmware.create)
    ..aOS(13, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'logs')
    ..hasRequiredFields = false
  ;

  Status._() : super();
  factory Status({
    $core.int? version,
    $core.int? uptime,
    Identity? identity,
    HardwareStatus? hardware,
    PowerStatus? power,
    MemoryStatus? memory,
    GpsStatus? gps,
    Schedules? schedules,
    Recording? recording,
    NetworkSettings? network,
    $fixnum.Int64? time,
    Firmware? firmware,
    $core.String? logs,
  }) {
    final _result = create();
    if (version != null) {
      _result.version = version;
    }
    if (uptime != null) {
      _result.uptime = uptime;
    }
    if (identity != null) {
      _result.identity = identity;
    }
    if (hardware != null) {
      _result.hardware = hardware;
    }
    if (power != null) {
      _result.power = power;
    }
    if (memory != null) {
      _result.memory = memory;
    }
    if (gps != null) {
      _result.gps = gps;
    }
    if (schedules != null) {
      _result.schedules = schedules;
    }
    if (recording != null) {
      _result.recording = recording;
    }
    if (network != null) {
      _result.network = network;
    }
    if (time != null) {
      _result.time = time;
    }
    if (firmware != null) {
      _result.firmware = firmware;
    }
    if (logs != null) {
      _result.logs = logs;
    }
    return _result;
  }
  factory Status.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Status.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Status clone() => Status()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Status copyWith(void Function(Status) updates) => super.copyWith((message) => updates(message as Status)) as Status; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static Status create() => Status._();
  Status createEmptyInstance() => create();
  static $pb.PbList<Status> createRepeated() => $pb.PbList<Status>();
  @$core.pragma('dart2js:noInline')
  static Status getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Status>(create);
  static Status? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get version => $_getIZ(0);
  @$pb.TagNumber(1)
  set version($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasVersion() => $_has(0);
  @$pb.TagNumber(1)
  void clearVersion() => clearField(1);

  @$pb.TagNumber(2)
  $core.int get uptime => $_getIZ(1);
  @$pb.TagNumber(2)
  set uptime($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasUptime() => $_has(1);
  @$pb.TagNumber(2)
  void clearUptime() => clearField(2);

  @$pb.TagNumber(3)
  Identity get identity => $_getN(2);
  @$pb.TagNumber(3)
  set identity(Identity v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasIdentity() => $_has(2);
  @$pb.TagNumber(3)
  void clearIdentity() => clearField(3);
  @$pb.TagNumber(3)
  Identity ensureIdentity() => $_ensure(2);

  @$pb.TagNumber(4)
  HardwareStatus get hardware => $_getN(3);
  @$pb.TagNumber(4)
  set hardware(HardwareStatus v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasHardware() => $_has(3);
  @$pb.TagNumber(4)
  void clearHardware() => clearField(4);
  @$pb.TagNumber(4)
  HardwareStatus ensureHardware() => $_ensure(3);

  @$pb.TagNumber(5)
  PowerStatus get power => $_getN(4);
  @$pb.TagNumber(5)
  set power(PowerStatus v) { setField(5, v); }
  @$pb.TagNumber(5)
  $core.bool hasPower() => $_has(4);
  @$pb.TagNumber(5)
  void clearPower() => clearField(5);
  @$pb.TagNumber(5)
  PowerStatus ensurePower() => $_ensure(4);

  @$pb.TagNumber(6)
  MemoryStatus get memory => $_getN(5);
  @$pb.TagNumber(6)
  set memory(MemoryStatus v) { setField(6, v); }
  @$pb.TagNumber(6)
  $core.bool hasMemory() => $_has(5);
  @$pb.TagNumber(6)
  void clearMemory() => clearField(6);
  @$pb.TagNumber(6)
  MemoryStatus ensureMemory() => $_ensure(5);

  @$pb.TagNumber(7)
  GpsStatus get gps => $_getN(6);
  @$pb.TagNumber(7)
  set gps(GpsStatus v) { setField(7, v); }
  @$pb.TagNumber(7)
  $core.bool hasGps() => $_has(6);
  @$pb.TagNumber(7)
  void clearGps() => clearField(7);
  @$pb.TagNumber(7)
  GpsStatus ensureGps() => $_ensure(6);

  @$pb.TagNumber(8)
  Schedules get schedules => $_getN(7);
  @$pb.TagNumber(8)
  set schedules(Schedules v) { setField(8, v); }
  @$pb.TagNumber(8)
  $core.bool hasSchedules() => $_has(7);
  @$pb.TagNumber(8)
  void clearSchedules() => clearField(8);
  @$pb.TagNumber(8)
  Schedules ensureSchedules() => $_ensure(7);

  @$pb.TagNumber(9)
  Recording get recording => $_getN(8);
  @$pb.TagNumber(9)
  set recording(Recording v) { setField(9, v); }
  @$pb.TagNumber(9)
  $core.bool hasRecording() => $_has(8);
  @$pb.TagNumber(9)
  void clearRecording() => clearField(9);
  @$pb.TagNumber(9)
  Recording ensureRecording() => $_ensure(8);

  @$pb.TagNumber(10)
  NetworkSettings get network => $_getN(9);
  @$pb.TagNumber(10)
  set network(NetworkSettings v) { setField(10, v); }
  @$pb.TagNumber(10)
  $core.bool hasNetwork() => $_has(9);
  @$pb.TagNumber(10)
  void clearNetwork() => clearField(10);
  @$pb.TagNumber(10)
  NetworkSettings ensureNetwork() => $_ensure(9);

  @$pb.TagNumber(11)
  $fixnum.Int64 get time => $_getI64(10);
  @$pb.TagNumber(11)
  set time($fixnum.Int64 v) { $_setInt64(10, v); }
  @$pb.TagNumber(11)
  $core.bool hasTime() => $_has(10);
  @$pb.TagNumber(11)
  void clearTime() => clearField(11);

  @$pb.TagNumber(12)
  Firmware get firmware => $_getN(11);
  @$pb.TagNumber(12)
  set firmware(Firmware v) { setField(12, v); }
  @$pb.TagNumber(12)
  $core.bool hasFirmware() => $_has(11);
  @$pb.TagNumber(12)
  void clearFirmware() => clearField(12);
  @$pb.TagNumber(12)
  Firmware ensureFirmware() => $_ensure(11);

  @$pb.TagNumber(13)
  $core.String get logs => $_getSZ(12);
  @$pb.TagNumber(13)
  set logs($core.String v) { $_setString(12, v); }
  @$pb.TagNumber(13)
  $core.bool hasLogs() => $_has(12);
  @$pb.TagNumber(13)
  void clearLogs() => clearField(13);
}

class Range extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'Range', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'start', $pb.PbFieldType.OU3)
    ..a<$core.int>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'end', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  Range._() : super();
  factory Range({
    $core.int? start,
    $core.int? end,
  }) {
    final _result = create();
    if (start != null) {
      _result.start = start;
    }
    if (end != null) {
      _result.end = end;
    }
    return _result;
  }
  factory Range.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Range.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Range clone() => Range()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Range copyWith(void Function(Range) updates) => super.copyWith((message) => updates(message as Range)) as Range; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static Range create() => Range._();
  Range createEmptyInstance() => create();
  static $pb.PbList<Range> createRepeated() => $pb.PbList<Range>();
  @$core.pragma('dart2js:noInline')
  static Range getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Range>(create);
  static Range? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get start => $_getIZ(0);
  @$pb.TagNumber(1)
  set start($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasStart() => $_has(0);
  @$pb.TagNumber(1)
  void clearStart() => clearField(1);

  @$pb.TagNumber(2)
  $core.int get end => $_getIZ(1);
  @$pb.TagNumber(2)
  set end($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasEnd() => $_has(1);
  @$pb.TagNumber(2)
  void clearEnd() => clearField(2);
}

class DownloadQuery extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'DownloadQuery', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'stream', $pb.PbFieldType.OU3)
    ..pc<Range>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'ranges', $pb.PbFieldType.PM, subBuilder: Range.create)
    ..p<$core.int>(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'blocks', $pb.PbFieldType.KU3)
    ..hasRequiredFields = false
  ;

  DownloadQuery._() : super();
  factory DownloadQuery({
    $core.int? stream,
    $core.Iterable<Range>? ranges,
    $core.Iterable<$core.int>? blocks,
  }) {
    final _result = create();
    if (stream != null) {
      _result.stream = stream;
    }
    if (ranges != null) {
      _result.ranges.addAll(ranges);
    }
    if (blocks != null) {
      _result.blocks.addAll(blocks);
    }
    return _result;
  }
  factory DownloadQuery.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DownloadQuery.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  DownloadQuery clone() => DownloadQuery()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  DownloadQuery copyWith(void Function(DownloadQuery) updates) => super.copyWith((message) => updates(message as DownloadQuery)) as DownloadQuery; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static DownloadQuery create() => DownloadQuery._();
  DownloadQuery createEmptyInstance() => create();
  static $pb.PbList<DownloadQuery> createRepeated() => $pb.PbList<DownloadQuery>();
  @$core.pragma('dart2js:noInline')
  static DownloadQuery getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DownloadQuery>(create);
  static DownloadQuery? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get stream => $_getIZ(0);
  @$pb.TagNumber(1)
  set stream($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasStream() => $_has(0);
  @$pb.TagNumber(1)
  void clearStream() => clearField(1);

  @$pb.TagNumber(3)
  $core.List<Range> get ranges => $_getList(1);

  @$pb.TagNumber(4)
  $core.List<$core.int> get blocks => $_getList(2);
}

class Recording extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'Recording', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..aOB(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'modifying')
    ..aOB(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'enabled')
    ..a<$fixnum.Int64>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'startedTime', $pb.PbFieldType.OU6, protoName: 'startedTime', defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOM<Location>(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'location', subBuilder: Location.create)
    ..hasRequiredFields = false
  ;

  Recording._() : super();
  factory Recording({
    $core.bool? modifying,
    $core.bool? enabled,
    $fixnum.Int64? startedTime,
    Location? location,
  }) {
    final _result = create();
    if (modifying != null) {
      _result.modifying = modifying;
    }
    if (enabled != null) {
      _result.enabled = enabled;
    }
    if (startedTime != null) {
      _result.startedTime = startedTime;
    }
    if (location != null) {
      _result.location = location;
    }
    return _result;
  }
  factory Recording.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Recording.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Recording clone() => Recording()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Recording copyWith(void Function(Recording) updates) => super.copyWith((message) => updates(message as Recording)) as Recording; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static Recording create() => Recording._();
  Recording createEmptyInstance() => create();
  static $pb.PbList<Recording> createRepeated() => $pb.PbList<Recording>();
  @$core.pragma('dart2js:noInline')
  static Recording getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Recording>(create);
  static Recording? _defaultInstance;

  @$pb.TagNumber(1)
  $core.bool get modifying => $_getBF(0);
  @$pb.TagNumber(1)
  set modifying($core.bool v) { $_setBool(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasModifying() => $_has(0);
  @$pb.TagNumber(1)
  void clearModifying() => clearField(1);

  @$pb.TagNumber(2)
  $core.bool get enabled => $_getBF(1);
  @$pb.TagNumber(2)
  set enabled($core.bool v) { $_setBool(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasEnabled() => $_has(1);
  @$pb.TagNumber(2)
  void clearEnabled() => clearField(2);

  @$pb.TagNumber(3)
  $fixnum.Int64 get startedTime => $_getI64(2);
  @$pb.TagNumber(3)
  set startedTime($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasStartedTime() => $_has(2);
  @$pb.TagNumber(3)
  void clearStartedTime() => clearField(3);

  @$pb.TagNumber(4)
  Location get location => $_getN(3);
  @$pb.TagNumber(4)
  set location(Location v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasLocation() => $_has(3);
  @$pb.TagNumber(4)
  void clearLocation() => clearField(4);
  @$pb.TagNumber(4)
  Location ensureLocation() => $_ensure(3);
}

class LoraSettings extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'LoraSettings', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..aOB(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'available')
    ..aOB(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'modifying')
    ..aOB(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'clearing')
    ..a<$core.int>(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'frequencyBand', $pb.PbFieldType.OU3, protoName: 'frequencyBand')
    ..a<$core.List<$core.int>>(5, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'deviceEui', $pb.PbFieldType.OY, protoName: 'deviceEui')
    ..a<$core.List<$core.int>>(6, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'appKey', $pb.PbFieldType.OY, protoName: 'appKey')
    ..a<$core.List<$core.int>>(7, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'joinEui', $pb.PbFieldType.OY, protoName: 'joinEui')
    ..a<$core.List<$core.int>>(8, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'deviceAddress', $pb.PbFieldType.OY, protoName: 'deviceAddress')
    ..a<$core.List<$core.int>>(9, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'networkSessionKey', $pb.PbFieldType.OY, protoName: 'networkSessionKey')
    ..a<$core.List<$core.int>>(10, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'appSessionKey', $pb.PbFieldType.OY, protoName: 'appSessionKey')
    ..hasRequiredFields = false
  ;

  LoraSettings._() : super();
  factory LoraSettings({
    $core.bool? available,
    $core.bool? modifying,
    $core.bool? clearing,
    $core.int? frequencyBand,
    $core.List<$core.int>? deviceEui,
    $core.List<$core.int>? appKey,
    $core.List<$core.int>? joinEui,
    $core.List<$core.int>? deviceAddress,
    $core.List<$core.int>? networkSessionKey,
    $core.List<$core.int>? appSessionKey,
  }) {
    final _result = create();
    if (available != null) {
      _result.available = available;
    }
    if (modifying != null) {
      _result.modifying = modifying;
    }
    if (clearing != null) {
      _result.clearing = clearing;
    }
    if (frequencyBand != null) {
      _result.frequencyBand = frequencyBand;
    }
    if (deviceEui != null) {
      _result.deviceEui = deviceEui;
    }
    if (appKey != null) {
      _result.appKey = appKey;
    }
    if (joinEui != null) {
      _result.joinEui = joinEui;
    }
    if (deviceAddress != null) {
      _result.deviceAddress = deviceAddress;
    }
    if (networkSessionKey != null) {
      _result.networkSessionKey = networkSessionKey;
    }
    if (appSessionKey != null) {
      _result.appSessionKey = appSessionKey;
    }
    return _result;
  }
  factory LoraSettings.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory LoraSettings.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  LoraSettings clone() => LoraSettings()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  LoraSettings copyWith(void Function(LoraSettings) updates) => super.copyWith((message) => updates(message as LoraSettings)) as LoraSettings; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static LoraSettings create() => LoraSettings._();
  LoraSettings createEmptyInstance() => create();
  static $pb.PbList<LoraSettings> createRepeated() => $pb.PbList<LoraSettings>();
  @$core.pragma('dart2js:noInline')
  static LoraSettings getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<LoraSettings>(create);
  static LoraSettings? _defaultInstance;

  @$pb.TagNumber(1)
  $core.bool get available => $_getBF(0);
  @$pb.TagNumber(1)
  set available($core.bool v) { $_setBool(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasAvailable() => $_has(0);
  @$pb.TagNumber(1)
  void clearAvailable() => clearField(1);

  @$pb.TagNumber(2)
  $core.bool get modifying => $_getBF(1);
  @$pb.TagNumber(2)
  set modifying($core.bool v) { $_setBool(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasModifying() => $_has(1);
  @$pb.TagNumber(2)
  void clearModifying() => clearField(2);

  @$pb.TagNumber(3)
  $core.bool get clearing => $_getBF(2);
  @$pb.TagNumber(3)
  set clearing($core.bool v) { $_setBool(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasClearing() => $_has(2);
  @$pb.TagNumber(3)
  void clearClearing() => clearField(3);

  @$pb.TagNumber(4)
  $core.int get frequencyBand => $_getIZ(3);
  @$pb.TagNumber(4)
  set frequencyBand($core.int v) { $_setUnsignedInt32(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasFrequencyBand() => $_has(3);
  @$pb.TagNumber(4)
  void clearFrequencyBand() => clearField(4);

  @$pb.TagNumber(5)
  $core.List<$core.int> get deviceEui => $_getN(4);
  @$pb.TagNumber(5)
  set deviceEui($core.List<$core.int> v) { $_setBytes(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasDeviceEui() => $_has(4);
  @$pb.TagNumber(5)
  void clearDeviceEui() => clearField(5);

  @$pb.TagNumber(6)
  $core.List<$core.int> get appKey => $_getN(5);
  @$pb.TagNumber(6)
  set appKey($core.List<$core.int> v) { $_setBytes(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasAppKey() => $_has(5);
  @$pb.TagNumber(6)
  void clearAppKey() => clearField(6);

  @$pb.TagNumber(7)
  $core.List<$core.int> get joinEui => $_getN(6);
  @$pb.TagNumber(7)
  set joinEui($core.List<$core.int> v) { $_setBytes(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasJoinEui() => $_has(6);
  @$pb.TagNumber(7)
  void clearJoinEui() => clearField(7);

  @$pb.TagNumber(8)
  $core.List<$core.int> get deviceAddress => $_getN(7);
  @$pb.TagNumber(8)
  set deviceAddress($core.List<$core.int> v) { $_setBytes(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasDeviceAddress() => $_has(7);
  @$pb.TagNumber(8)
  void clearDeviceAddress() => clearField(8);

  @$pb.TagNumber(9)
  $core.List<$core.int> get networkSessionKey => $_getN(8);
  @$pb.TagNumber(9)
  set networkSessionKey($core.List<$core.int> v) { $_setBytes(8, v); }
  @$pb.TagNumber(9)
  $core.bool hasNetworkSessionKey() => $_has(8);
  @$pb.TagNumber(9)
  void clearNetworkSessionKey() => clearField(9);

  @$pb.TagNumber(10)
  $core.List<$core.int> get appSessionKey => $_getN(9);
  @$pb.TagNumber(10)
  set appSessionKey($core.List<$core.int> v) { $_setBytes(9, v); }
  @$pb.TagNumber(10)
  $core.bool hasAppSessionKey() => $_has(9);
  @$pb.TagNumber(10)
  void clearAppSessionKey() => clearField(10);
}

class Location extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'Location', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..aOB(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'modifying')
    ..a<$core.double>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'longitude', $pb.PbFieldType.OF)
    ..a<$core.double>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'latitude', $pb.PbFieldType.OF)
    ..a<$fixnum.Int64>(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'time', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..hasRequiredFields = false
  ;

  Location._() : super();
  factory Location({
    $core.bool? modifying,
    $core.double? longitude,
    $core.double? latitude,
    $fixnum.Int64? time,
  }) {
    final _result = create();
    if (modifying != null) {
      _result.modifying = modifying;
    }
    if (longitude != null) {
      _result.longitude = longitude;
    }
    if (latitude != null) {
      _result.latitude = latitude;
    }
    if (time != null) {
      _result.time = time;
    }
    return _result;
  }
  factory Location.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Location.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Location clone() => Location()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Location copyWith(void Function(Location) updates) => super.copyWith((message) => updates(message as Location)) as Location; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static Location create() => Location._();
  Location createEmptyInstance() => create();
  static $pb.PbList<Location> createRepeated() => $pb.PbList<Location>();
  @$core.pragma('dart2js:noInline')
  static Location getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Location>(create);
  static Location? _defaultInstance;

  @$pb.TagNumber(1)
  $core.bool get modifying => $_getBF(0);
  @$pb.TagNumber(1)
  set modifying($core.bool v) { $_setBool(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasModifying() => $_has(0);
  @$pb.TagNumber(1)
  void clearModifying() => clearField(1);

  @$pb.TagNumber(2)
  $core.double get longitude => $_getN(1);
  @$pb.TagNumber(2)
  set longitude($core.double v) { $_setFloat(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasLongitude() => $_has(1);
  @$pb.TagNumber(2)
  void clearLongitude() => clearField(2);

  @$pb.TagNumber(3)
  $core.double get latitude => $_getN(2);
  @$pb.TagNumber(3)
  set latitude($core.double v) { $_setFloat(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasLatitude() => $_has(2);
  @$pb.TagNumber(3)
  void clearLatitude() => clearField(3);

  @$pb.TagNumber(4)
  $fixnum.Int64 get time => $_getI64(3);
  @$pb.TagNumber(4)
  set time($fixnum.Int64 v) { $_setInt64(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasTime() => $_has(3);
  @$pb.TagNumber(4)
  void clearTime() => clearField(4);
}

class WifiTransmission extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'WifiTransmission', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..aOB(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'modifying')
    ..aOS(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'url')
    ..aOS(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'token')
    ..aOB(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'enabled')
    ..hasRequiredFields = false
  ;

  WifiTransmission._() : super();
  factory WifiTransmission({
    $core.bool? modifying,
    $core.String? url,
    $core.String? token,
    $core.bool? enabled,
  }) {
    final _result = create();
    if (modifying != null) {
      _result.modifying = modifying;
    }
    if (url != null) {
      _result.url = url;
    }
    if (token != null) {
      _result.token = token;
    }
    if (enabled != null) {
      _result.enabled = enabled;
    }
    return _result;
  }
  factory WifiTransmission.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory WifiTransmission.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  WifiTransmission clone() => WifiTransmission()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  WifiTransmission copyWith(void Function(WifiTransmission) updates) => super.copyWith((message) => updates(message as WifiTransmission)) as WifiTransmission; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static WifiTransmission create() => WifiTransmission._();
  WifiTransmission createEmptyInstance() => create();
  static $pb.PbList<WifiTransmission> createRepeated() => $pb.PbList<WifiTransmission>();
  @$core.pragma('dart2js:noInline')
  static WifiTransmission getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<WifiTransmission>(create);
  static WifiTransmission? _defaultInstance;

  @$pb.TagNumber(1)
  $core.bool get modifying => $_getBF(0);
  @$pb.TagNumber(1)
  set modifying($core.bool v) { $_setBool(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasModifying() => $_has(0);
  @$pb.TagNumber(1)
  void clearModifying() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get url => $_getSZ(1);
  @$pb.TagNumber(2)
  set url($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasUrl() => $_has(1);
  @$pb.TagNumber(2)
  void clearUrl() => clearField(2);

  @$pb.TagNumber(3)
  $core.String get token => $_getSZ(2);
  @$pb.TagNumber(3)
  set token($core.String v) { $_setString(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasToken() => $_has(2);
  @$pb.TagNumber(3)
  void clearToken() => clearField(3);

  @$pb.TagNumber(4)
  $core.bool get enabled => $_getBF(3);
  @$pb.TagNumber(4)
  set enabled($core.bool v) { $_setBool(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasEnabled() => $_has(3);
  @$pb.TagNumber(4)
  void clearEnabled() => clearField(4);
}

class Transmission extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'Transmission', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..aOM<WifiTransmission>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'wifi', subBuilder: WifiTransmission.create)
    ..hasRequiredFields = false
  ;

  Transmission._() : super();
  factory Transmission({
    WifiTransmission? wifi,
  }) {
    final _result = create();
    if (wifi != null) {
      _result.wifi = wifi;
    }
    return _result;
  }
  factory Transmission.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Transmission.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Transmission clone() => Transmission()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Transmission copyWith(void Function(Transmission) updates) => super.copyWith((message) => updates(message as Transmission)) as Transmission; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static Transmission create() => Transmission._();
  Transmission createEmptyInstance() => create();
  static $pb.PbList<Transmission> createRepeated() => $pb.PbList<Transmission>();
  @$core.pragma('dart2js:noInline')
  static Transmission getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<Transmission>(create);
  static Transmission? _defaultInstance;

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

class ListDirectory extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'ListDirectory', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..aOS(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'path')
    ..a<$core.int>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'page', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  ListDirectory._() : super();
  factory ListDirectory({
    $core.String? path,
    $core.int? page,
  }) {
    final _result = create();
    if (path != null) {
      _result.path = path;
    }
    if (page != null) {
      _result.page = page;
    }
    return _result;
  }
  factory ListDirectory.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory ListDirectory.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  ListDirectory clone() => ListDirectory()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  ListDirectory copyWith(void Function(ListDirectory) updates) => super.copyWith((message) => updates(message as ListDirectory)) as ListDirectory; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static ListDirectory create() => ListDirectory._();
  ListDirectory createEmptyInstance() => create();
  static $pb.PbList<ListDirectory> createRepeated() => $pb.PbList<ListDirectory>();
  @$core.pragma('dart2js:noInline')
  static ListDirectory getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<ListDirectory>(create);
  static ListDirectory? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get path => $_getSZ(0);
  @$pb.TagNumber(1)
  set path($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasPath() => $_has(0);
  @$pb.TagNumber(1)
  void clearPath() => clearField(1);

  @$pb.TagNumber(2)
  $core.int get page => $_getIZ(1);
  @$pb.TagNumber(2)
  set page($core.int v) { $_setUnsignedInt32(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasPage() => $_has(1);
  @$pb.TagNumber(2)
  void clearPage() => clearField(2);
}

class HttpQuery extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'HttpQuery', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..e<QueryType>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'type', $pb.PbFieldType.OE, defaultOrMaker: QueryType.QUERY_NONE, valueOf: QueryType.valueOf, enumValues: QueryType.values)
    ..aOM<Identity>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'identity', subBuilder: Identity.create)
    ..aOM<Recording>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'recording', subBuilder: Recording.create)
    ..aOM<Schedules>(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'schedules', subBuilder: Schedules.create)
    ..a<$core.int>(5, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'flags', $pb.PbFieldType.OU3)
    ..aOM<NetworkSettings>(6, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'networkSettings', protoName: 'networkSettings', subBuilder: NetworkSettings.create)
    ..aOM<LoraSettings>(7, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'loraSettings', protoName: 'loraSettings', subBuilder: LoraSettings.create)
    ..a<$fixnum.Int64>(8, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'time', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..aOM<Location>(9, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'locate', subBuilder: Location.create)
    ..aOM<Transmission>(10, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'transmission', subBuilder: Transmission.create)
    ..aOM<ListDirectory>(11, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'directory', subBuilder: ListDirectory.create)
    ..a<$core.int>(12, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'counter', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  HttpQuery._() : super();
  factory HttpQuery({
    QueryType? type,
    Identity? identity,
    Recording? recording,
    Schedules? schedules,
    $core.int? flags,
    NetworkSettings? networkSettings,
    LoraSettings? loraSettings,
    $fixnum.Int64? time,
    Location? locate,
    Transmission? transmission,
    ListDirectory? directory,
    $core.int? counter,
  }) {
    final _result = create();
    if (type != null) {
      _result.type = type;
    }
    if (identity != null) {
      _result.identity = identity;
    }
    if (recording != null) {
      _result.recording = recording;
    }
    if (schedules != null) {
      _result.schedules = schedules;
    }
    if (flags != null) {
      _result.flags = flags;
    }
    if (networkSettings != null) {
      _result.networkSettings = networkSettings;
    }
    if (loraSettings != null) {
      _result.loraSettings = loraSettings;
    }
    if (time != null) {
      _result.time = time;
    }
    if (locate != null) {
      _result.locate = locate;
    }
    if (transmission != null) {
      _result.transmission = transmission;
    }
    if (directory != null) {
      _result.directory = directory;
    }
    if (counter != null) {
      _result.counter = counter;
    }
    return _result;
  }
  factory HttpQuery.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory HttpQuery.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  HttpQuery clone() => HttpQuery()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  HttpQuery copyWith(void Function(HttpQuery) updates) => super.copyWith((message) => updates(message as HttpQuery)) as HttpQuery; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static HttpQuery create() => HttpQuery._();
  HttpQuery createEmptyInstance() => create();
  static $pb.PbList<HttpQuery> createRepeated() => $pb.PbList<HttpQuery>();
  @$core.pragma('dart2js:noInline')
  static HttpQuery getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<HttpQuery>(create);
  static HttpQuery? _defaultInstance;

  @$pb.TagNumber(1)
  QueryType get type => $_getN(0);
  @$pb.TagNumber(1)
  set type(QueryType v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasType() => $_has(0);
  @$pb.TagNumber(1)
  void clearType() => clearField(1);

  @$pb.TagNumber(2)
  Identity get identity => $_getN(1);
  @$pb.TagNumber(2)
  set identity(Identity v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasIdentity() => $_has(1);
  @$pb.TagNumber(2)
  void clearIdentity() => clearField(2);
  @$pb.TagNumber(2)
  Identity ensureIdentity() => $_ensure(1);

  @$pb.TagNumber(3)
  Recording get recording => $_getN(2);
  @$pb.TagNumber(3)
  set recording(Recording v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasRecording() => $_has(2);
  @$pb.TagNumber(3)
  void clearRecording() => clearField(3);
  @$pb.TagNumber(3)
  Recording ensureRecording() => $_ensure(2);

  @$pb.TagNumber(4)
  Schedules get schedules => $_getN(3);
  @$pb.TagNumber(4)
  set schedules(Schedules v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasSchedules() => $_has(3);
  @$pb.TagNumber(4)
  void clearSchedules() => clearField(4);
  @$pb.TagNumber(4)
  Schedules ensureSchedules() => $_ensure(3);

  @$pb.TagNumber(5)
  $core.int get flags => $_getIZ(4);
  @$pb.TagNumber(5)
  set flags($core.int v) { $_setUnsignedInt32(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasFlags() => $_has(4);
  @$pb.TagNumber(5)
  void clearFlags() => clearField(5);

  @$pb.TagNumber(6)
  NetworkSettings get networkSettings => $_getN(5);
  @$pb.TagNumber(6)
  set networkSettings(NetworkSettings v) { setField(6, v); }
  @$pb.TagNumber(6)
  $core.bool hasNetworkSettings() => $_has(5);
  @$pb.TagNumber(6)
  void clearNetworkSettings() => clearField(6);
  @$pb.TagNumber(6)
  NetworkSettings ensureNetworkSettings() => $_ensure(5);

  @$pb.TagNumber(7)
  LoraSettings get loraSettings => $_getN(6);
  @$pb.TagNumber(7)
  set loraSettings(LoraSettings v) { setField(7, v); }
  @$pb.TagNumber(7)
  $core.bool hasLoraSettings() => $_has(6);
  @$pb.TagNumber(7)
  void clearLoraSettings() => clearField(7);
  @$pb.TagNumber(7)
  LoraSettings ensureLoraSettings() => $_ensure(6);

  @$pb.TagNumber(8)
  $fixnum.Int64 get time => $_getI64(7);
  @$pb.TagNumber(8)
  set time($fixnum.Int64 v) { $_setInt64(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasTime() => $_has(7);
  @$pb.TagNumber(8)
  void clearTime() => clearField(8);

  @$pb.TagNumber(9)
  Location get locate => $_getN(8);
  @$pb.TagNumber(9)
  set locate(Location v) { setField(9, v); }
  @$pb.TagNumber(9)
  $core.bool hasLocate() => $_has(8);
  @$pb.TagNumber(9)
  void clearLocate() => clearField(9);
  @$pb.TagNumber(9)
  Location ensureLocate() => $_ensure(8);

  @$pb.TagNumber(10)
  Transmission get transmission => $_getN(9);
  @$pb.TagNumber(10)
  set transmission(Transmission v) { setField(10, v); }
  @$pb.TagNumber(10)
  $core.bool hasTransmission() => $_has(9);
  @$pb.TagNumber(10)
  void clearTransmission() => clearField(10);
  @$pb.TagNumber(10)
  Transmission ensureTransmission() => $_ensure(9);

  @$pb.TagNumber(11)
  ListDirectory get directory => $_getN(10);
  @$pb.TagNumber(11)
  set directory(ListDirectory v) { setField(11, v); }
  @$pb.TagNumber(11)
  $core.bool hasDirectory() => $_has(10);
  @$pb.TagNumber(11)
  void clearDirectory() => clearField(11);
  @$pb.TagNumber(11)
  ListDirectory ensureDirectory() => $_ensure(10);

  @$pb.TagNumber(12)
  $core.int get counter => $_getIZ(11);
  @$pb.TagNumber(12)
  set counter($core.int v) { $_setUnsignedInt32(11, v); }
  @$pb.TagNumber(12)
  $core.bool hasCounter() => $_has(11);
  @$pb.TagNumber(12)
  void clearCounter() => clearField(12);
}

class DataStream extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'DataStream', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'id', $pb.PbFieldType.OU3)
    ..a<$fixnum.Int64>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'time', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$fixnum.Int64>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'size', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.int>(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'version', $pb.PbFieldType.OU3)
    ..a<$fixnum.Int64>(5, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'block', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..a<$core.List<$core.int>>(6, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'hash', $pb.PbFieldType.OY)
    ..aOS(7, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'name')
    ..aOS(8, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'path')
    ..hasRequiredFields = false
  ;

  DataStream._() : super();
  factory DataStream({
    $core.int? id,
    $fixnum.Int64? time,
    $fixnum.Int64? size,
    $core.int? version,
    $fixnum.Int64? block,
    $core.List<$core.int>? hash,
    $core.String? name,
    $core.String? path,
  }) {
    final _result = create();
    if (id != null) {
      _result.id = id;
    }
    if (time != null) {
      _result.time = time;
    }
    if (size != null) {
      _result.size = size;
    }
    if (version != null) {
      _result.version = version;
    }
    if (block != null) {
      _result.block = block;
    }
    if (hash != null) {
      _result.hash = hash;
    }
    if (name != null) {
      _result.name = name;
    }
    if (path != null) {
      _result.path = path;
    }
    return _result;
  }
  factory DataStream.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DataStream.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  DataStream clone() => DataStream()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  DataStream copyWith(void Function(DataStream) updates) => super.copyWith((message) => updates(message as DataStream)) as DataStream; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static DataStream create() => DataStream._();
  DataStream createEmptyInstance() => create();
  static $pb.PbList<DataStream> createRepeated() => $pb.PbList<DataStream>();
  @$core.pragma('dart2js:noInline')
  static DataStream getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DataStream>(create);
  static DataStream? _defaultInstance;

  @$pb.TagNumber(1)
  $core.int get id => $_getIZ(0);
  @$pb.TagNumber(1)
  set id($core.int v) { $_setUnsignedInt32(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasId() => $_has(0);
  @$pb.TagNumber(1)
  void clearId() => clearField(1);

  @$pb.TagNumber(2)
  $fixnum.Int64 get time => $_getI64(1);
  @$pb.TagNumber(2)
  set time($fixnum.Int64 v) { $_setInt64(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasTime() => $_has(1);
  @$pb.TagNumber(2)
  void clearTime() => clearField(2);

  @$pb.TagNumber(3)
  $fixnum.Int64 get size => $_getI64(2);
  @$pb.TagNumber(3)
  set size($fixnum.Int64 v) { $_setInt64(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasSize() => $_has(2);
  @$pb.TagNumber(3)
  void clearSize() => clearField(3);

  @$pb.TagNumber(4)
  $core.int get version => $_getIZ(3);
  @$pb.TagNumber(4)
  set version($core.int v) { $_setUnsignedInt32(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasVersion() => $_has(3);
  @$pb.TagNumber(4)
  void clearVersion() => clearField(4);

  @$pb.TagNumber(5)
  $fixnum.Int64 get block => $_getI64(4);
  @$pb.TagNumber(5)
  set block($fixnum.Int64 v) { $_setInt64(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasBlock() => $_has(4);
  @$pb.TagNumber(5)
  void clearBlock() => clearField(5);

  @$pb.TagNumber(6)
  $core.List<$core.int> get hash => $_getN(5);
  @$pb.TagNumber(6)
  set hash($core.List<$core.int> v) { $_setBytes(5, v); }
  @$pb.TagNumber(6)
  $core.bool hasHash() => $_has(5);
  @$pb.TagNumber(6)
  void clearHash() => clearField(6);

  @$pb.TagNumber(7)
  $core.String get name => $_getSZ(6);
  @$pb.TagNumber(7)
  set name($core.String v) { $_setString(6, v); }
  @$pb.TagNumber(7)
  $core.bool hasName() => $_has(6);
  @$pb.TagNumber(7)
  void clearName() => clearField(7);

  @$pb.TagNumber(8)
  $core.String get path => $_getSZ(7);
  @$pb.TagNumber(8)
  set path($core.String v) { $_setString(7, v); }
  @$pb.TagNumber(8)
  $core.bool hasPath() => $_has(7);
  @$pb.TagNumber(8)
  void clearPath() => clearField(8);
}

class LiveSensorReading extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'LiveSensorReading', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..aOM<SensorCapabilities>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'sensor', subBuilder: SensorCapabilities.create)
    ..a<$core.double>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'value', $pb.PbFieldType.OF)
    ..a<$core.double>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'uncalibrated', $pb.PbFieldType.OF)
    ..a<$core.double>(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'factory', $pb.PbFieldType.OF)
    ..a<$core.List<$core.int>>(5, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'adc', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  LiveSensorReading._() : super();
  factory LiveSensorReading({
    SensorCapabilities? sensor,
    $core.double? value,
    $core.double? uncalibrated,
    $core.double? factory,
    $core.List<$core.int>? adc,
  }) {
    final _result = create();
    if (sensor != null) {
      _result.sensor = sensor;
    }
    if (value != null) {
      _result.value = value;
    }
    if (uncalibrated != null) {
      _result.uncalibrated = uncalibrated;
    }
    if (factory != null) {
      _result.factory = factory;
    }
    if (adc != null) {
      _result.adc = adc;
    }
    return _result;
  }
  factory LiveSensorReading.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory LiveSensorReading.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  LiveSensorReading clone() => LiveSensorReading()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  LiveSensorReading copyWith(void Function(LiveSensorReading) updates) => super.copyWith((message) => updates(message as LiveSensorReading)) as LiveSensorReading; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static LiveSensorReading create() => LiveSensorReading._();
  LiveSensorReading createEmptyInstance() => create();
  static $pb.PbList<LiveSensorReading> createRepeated() => $pb.PbList<LiveSensorReading>();
  @$core.pragma('dart2js:noInline')
  static LiveSensorReading getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<LiveSensorReading>(create);
  static LiveSensorReading? _defaultInstance;

  @$pb.TagNumber(1)
  SensorCapabilities get sensor => $_getN(0);
  @$pb.TagNumber(1)
  set sensor(SensorCapabilities v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasSensor() => $_has(0);
  @$pb.TagNumber(1)
  void clearSensor() => clearField(1);
  @$pb.TagNumber(1)
  SensorCapabilities ensureSensor() => $_ensure(0);

  @$pb.TagNumber(2)
  $core.double get value => $_getN(1);
  @$pb.TagNumber(2)
  set value($core.double v) { $_setFloat(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasValue() => $_has(1);
  @$pb.TagNumber(2)
  void clearValue() => clearField(2);

  @$pb.TagNumber(3)
  $core.double get uncalibrated => $_getN(2);
  @$pb.TagNumber(3)
  set uncalibrated($core.double v) { $_setFloat(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasUncalibrated() => $_has(2);
  @$pb.TagNumber(3)
  void clearUncalibrated() => clearField(3);

  @$pb.TagNumber(4)
  $core.double get factory => $_getN(3);
  @$pb.TagNumber(4)
  set factory($core.double v) { $_setFloat(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasFactory() => $_has(3);
  @$pb.TagNumber(4)
  void clearFactory() => clearField(4);

  @$pb.TagNumber(5)
  $core.List<$core.int> get adc => $_getN(4);
  @$pb.TagNumber(5)
  set adc($core.List<$core.int> v) { $_setBytes(4, v); }
  @$pb.TagNumber(5)
  $core.bool hasAdc() => $_has(4);
  @$pb.TagNumber(5)
  void clearAdc() => clearField(5);
}

class LiveModuleReadings extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'LiveModuleReadings', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..aOM<ModuleCapabilities>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'module', subBuilder: ModuleCapabilities.create)
    ..pc<LiveSensorReading>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'readings', $pb.PbFieldType.PM, subBuilder: LiveSensorReading.create)
    ..hasRequiredFields = false
  ;

  LiveModuleReadings._() : super();
  factory LiveModuleReadings({
    ModuleCapabilities? module,
    $core.Iterable<LiveSensorReading>? readings,
  }) {
    final _result = create();
    if (module != null) {
      _result.module = module;
    }
    if (readings != null) {
      _result.readings.addAll(readings);
    }
    return _result;
  }
  factory LiveModuleReadings.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory LiveModuleReadings.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  LiveModuleReadings clone() => LiveModuleReadings()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  LiveModuleReadings copyWith(void Function(LiveModuleReadings) updates) => super.copyWith((message) => updates(message as LiveModuleReadings)) as LiveModuleReadings; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static LiveModuleReadings create() => LiveModuleReadings._();
  LiveModuleReadings createEmptyInstance() => create();
  static $pb.PbList<LiveModuleReadings> createRepeated() => $pb.PbList<LiveModuleReadings>();
  @$core.pragma('dart2js:noInline')
  static LiveModuleReadings getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<LiveModuleReadings>(create);
  static LiveModuleReadings? _defaultInstance;

  @$pb.TagNumber(1)
  ModuleCapabilities get module => $_getN(0);
  @$pb.TagNumber(1)
  set module(ModuleCapabilities v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasModule() => $_has(0);
  @$pb.TagNumber(1)
  void clearModule() => clearField(1);
  @$pb.TagNumber(1)
  ModuleCapabilities ensureModule() => $_ensure(0);

  @$pb.TagNumber(2)
  $core.List<LiveSensorReading> get readings => $_getList(1);
}

class LiveReadings extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'LiveReadings', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$fixnum.Int64>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'time', $pb.PbFieldType.OU6, defaultOrMaker: $fixnum.Int64.ZERO)
    ..pc<LiveModuleReadings>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'modules', $pb.PbFieldType.PM, subBuilder: LiveModuleReadings.create)
    ..hasRequiredFields = false
  ;

  LiveReadings._() : super();
  factory LiveReadings({
    $fixnum.Int64? time,
    $core.Iterable<LiveModuleReadings>? modules,
  }) {
    final _result = create();
    if (time != null) {
      _result.time = time;
    }
    if (modules != null) {
      _result.modules.addAll(modules);
    }
    return _result;
  }
  factory LiveReadings.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory LiveReadings.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  LiveReadings clone() => LiveReadings()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  LiveReadings copyWith(void Function(LiveReadings) updates) => super.copyWith((message) => updates(message as LiveReadings)) as LiveReadings; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static LiveReadings create() => LiveReadings._();
  LiveReadings createEmptyInstance() => create();
  static $pb.PbList<LiveReadings> createRepeated() => $pb.PbList<LiveReadings>();
  @$core.pragma('dart2js:noInline')
  static LiveReadings getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<LiveReadings>(create);
  static LiveReadings? _defaultInstance;

  @$pb.TagNumber(1)
  $fixnum.Int64 get time => $_getI64(0);
  @$pb.TagNumber(1)
  set time($fixnum.Int64 v) { $_setInt64(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasTime() => $_has(0);
  @$pb.TagNumber(1)
  void clearTime() => clearField(1);

  @$pb.TagNumber(2)
  $core.List<LiveModuleReadings> get modules => $_getList(1);
}

class DirectoryEntry extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'DirectoryEntry', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..aOS(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'name')
    ..aOS(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'path')
    ..a<$core.int>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'size', $pb.PbFieldType.OU3)
    ..aOB(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'directory')
    ..hasRequiredFields = false
  ;

  DirectoryEntry._() : super();
  factory DirectoryEntry({
    $core.String? name,
    $core.String? path,
    $core.int? size,
    $core.bool? directory,
  }) {
    final _result = create();
    if (name != null) {
      _result.name = name;
    }
    if (path != null) {
      _result.path = path;
    }
    if (size != null) {
      _result.size = size;
    }
    if (directory != null) {
      _result.directory = directory;
    }
    return _result;
  }
  factory DirectoryEntry.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DirectoryEntry.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  DirectoryEntry clone() => DirectoryEntry()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  DirectoryEntry copyWith(void Function(DirectoryEntry) updates) => super.copyWith((message) => updates(message as DirectoryEntry)) as DirectoryEntry; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static DirectoryEntry create() => DirectoryEntry._();
  DirectoryEntry createEmptyInstance() => create();
  static $pb.PbList<DirectoryEntry> createRepeated() => $pb.PbList<DirectoryEntry>();
  @$core.pragma('dart2js:noInline')
  static DirectoryEntry getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DirectoryEntry>(create);
  static DirectoryEntry? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get name => $_getSZ(0);
  @$pb.TagNumber(1)
  set name($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasName() => $_has(0);
  @$pb.TagNumber(1)
  void clearName() => clearField(1);

  @$pb.TagNumber(2)
  $core.String get path => $_getSZ(1);
  @$pb.TagNumber(2)
  set path($core.String v) { $_setString(1, v); }
  @$pb.TagNumber(2)
  $core.bool hasPath() => $_has(1);
  @$pb.TagNumber(2)
  void clearPath() => clearField(2);

  @$pb.TagNumber(3)
  $core.int get size => $_getIZ(2);
  @$pb.TagNumber(3)
  set size($core.int v) { $_setUnsignedInt32(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasSize() => $_has(2);
  @$pb.TagNumber(3)
  void clearSize() => clearField(3);

  @$pb.TagNumber(4)
  $core.bool get directory => $_getBF(3);
  @$pb.TagNumber(4)
  set directory($core.bool v) { $_setBool(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasDirectory() => $_has(3);
  @$pb.TagNumber(4)
  void clearDirectory() => clearField(4);
}

class DirectoryListing extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'DirectoryListing', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..aOS(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'path')
    ..pc<DirectoryEntry>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'entries', $pb.PbFieldType.PM, subBuilder: DirectoryEntry.create)
    ..a<$core.int>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'totalEntries', $pb.PbFieldType.OU3, protoName: 'totalEntries')
    ..hasRequiredFields = false
  ;

  DirectoryListing._() : super();
  factory DirectoryListing({
    $core.String? path,
    $core.Iterable<DirectoryEntry>? entries,
    $core.int? totalEntries,
  }) {
    final _result = create();
    if (path != null) {
      _result.path = path;
    }
    if (entries != null) {
      _result.entries.addAll(entries);
    }
    if (totalEntries != null) {
      _result.totalEntries = totalEntries;
    }
    return _result;
  }
  factory DirectoryListing.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory DirectoryListing.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  DirectoryListing clone() => DirectoryListing()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  DirectoryListing copyWith(void Function(DirectoryListing) updates) => super.copyWith((message) => updates(message as DirectoryListing)) as DirectoryListing; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static DirectoryListing create() => DirectoryListing._();
  DirectoryListing createEmptyInstance() => create();
  static $pb.PbList<DirectoryListing> createRepeated() => $pb.PbList<DirectoryListing>();
  @$core.pragma('dart2js:noInline')
  static DirectoryListing getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<DirectoryListing>(create);
  static DirectoryListing? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get path => $_getSZ(0);
  @$pb.TagNumber(1)
  set path($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasPath() => $_has(0);
  @$pb.TagNumber(1)
  void clearPath() => clearField(1);

  @$pb.TagNumber(2)
  $core.List<DirectoryEntry> get entries => $_getList(1);

  @$pb.TagNumber(3)
  $core.int get totalEntries => $_getIZ(2);
  @$pb.TagNumber(3)
  set totalEntries($core.int v) { $_setUnsignedInt32(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasTotalEntries() => $_has(2);
  @$pb.TagNumber(3)
  void clearTotalEntries() => clearField(3);
}

class NearbyNetwork extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'NearbyNetwork', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..aOS(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'ssid')
    ..hasRequiredFields = false
  ;

  NearbyNetwork._() : super();
  factory NearbyNetwork({
    $core.String? ssid,
  }) {
    final _result = create();
    if (ssid != null) {
      _result.ssid = ssid;
    }
    return _result;
  }
  factory NearbyNetwork.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory NearbyNetwork.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  NearbyNetwork clone() => NearbyNetwork()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  NearbyNetwork copyWith(void Function(NearbyNetwork) updates) => super.copyWith((message) => updates(message as NearbyNetwork)) as NearbyNetwork; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static NearbyNetwork create() => NearbyNetwork._();
  NearbyNetwork createEmptyInstance() => create();
  static $pb.PbList<NearbyNetwork> createRepeated() => $pb.PbList<NearbyNetwork>();
  @$core.pragma('dart2js:noInline')
  static NearbyNetwork getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<NearbyNetwork>(create);
  static NearbyNetwork? _defaultInstance;

  @$pb.TagNumber(1)
  $core.String get ssid => $_getSZ(0);
  @$pb.TagNumber(1)
  set ssid($core.String v) { $_setString(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasSsid() => $_has(0);
  @$pb.TagNumber(1)
  void clearSsid() => clearField(1);
}

class NearbyNetworks extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'NearbyNetworks', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..pc<NearbyNetwork>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'networks', $pb.PbFieldType.PM, subBuilder: NearbyNetwork.create)
    ..hasRequiredFields = false
  ;

  NearbyNetworks._() : super();
  factory NearbyNetworks({
    $core.Iterable<NearbyNetwork>? networks,
  }) {
    final _result = create();
    if (networks != null) {
      _result.networks.addAll(networks);
    }
    return _result;
  }
  factory NearbyNetworks.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory NearbyNetworks.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  NearbyNetworks clone() => NearbyNetworks()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  NearbyNetworks copyWith(void Function(NearbyNetworks) updates) => super.copyWith((message) => updates(message as NearbyNetworks)) as NearbyNetworks; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static NearbyNetworks create() => NearbyNetworks._();
  NearbyNetworks createEmptyInstance() => create();
  static $pb.PbList<NearbyNetworks> createRepeated() => $pb.PbList<NearbyNetworks>();
  @$core.pragma('dart2js:noInline')
  static NearbyNetworks getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<NearbyNetworks>(create);
  static NearbyNetworks? _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<NearbyNetwork> get networks => $_getList(0);
}

class Fault extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'Fault', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.int>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'time', $pb.PbFieldType.OU3)
    ..a<$core.int>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'code', $pb.PbFieldType.OU3)
    ..aOS(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'description')
    ..a<$core.List<$core.int>>(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'debug', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  Fault._() : super();
  factory Fault({
    $core.int? time,
    $core.int? code,
    $core.String? description,
    $core.List<$core.int>? debug,
  }) {
    final _result = create();
    if (time != null) {
      _result.time = time;
    }
    if (code != null) {
      _result.code = code;
    }
    if (description != null) {
      _result.description = description;
    }
    if (debug != null) {
      _result.debug = debug;
    }
    return _result;
  }
  factory Fault.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory Fault.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  Fault clone() => Fault()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  Fault copyWith(void Function(Fault) updates) => super.copyWith((message) => updates(message as Fault)) as Fault; // ignore: deprecated_member_use
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

class HttpReply extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'HttpReply', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..e<ReplyType>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'type', $pb.PbFieldType.OE, defaultOrMaker: ReplyType.REPLY_NONE, valueOf: ReplyType.valueOf, enumValues: ReplyType.values)
    ..pc<Error>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'errors', $pb.PbFieldType.PM, subBuilder: Error.create)
    ..aOM<Status>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'status', subBuilder: Status.create)
    ..aOM<NetworkSettings>(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'networkSettings', protoName: 'networkSettings', subBuilder: NetworkSettings.create)
    ..pc<ModuleCapabilities>(5, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'modules', $pb.PbFieldType.PM, subBuilder: ModuleCapabilities.create)
    ..pc<DataStream>(6, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'streams', $pb.PbFieldType.PM, subBuilder: DataStream.create)
    ..pc<LiveReadings>(7, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'liveReadings', $pb.PbFieldType.PM, protoName: 'liveReadings', subBuilder: LiveReadings.create)
    ..aOM<LoraSettings>(8, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'loraSettings', protoName: 'loraSettings', subBuilder: LoraSettings.create)
    ..aOM<Schedules>(9, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'schedules', subBuilder: Schedules.create)
    ..aOM<Transmission>(10, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'transmission', subBuilder: Transmission.create)
    ..aOM<DirectoryListing>(11, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'listing', subBuilder: DirectoryListing.create)
    ..aOM<NearbyNetworks>(12, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'nearbyNetworks', protoName: 'nearbyNetworks', subBuilder: NearbyNetworks.create)
    ..pc<Fault>(13, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'faults', $pb.PbFieldType.PM, subBuilder: Fault.create)
    ..hasRequiredFields = false
  ;

  HttpReply._() : super();
  factory HttpReply({
    ReplyType? type,
    $core.Iterable<Error>? errors,
    Status? status,
    NetworkSettings? networkSettings,
    $core.Iterable<ModuleCapabilities>? modules,
    $core.Iterable<DataStream>? streams,
    $core.Iterable<LiveReadings>? liveReadings,
    LoraSettings? loraSettings,
    Schedules? schedules,
    Transmission? transmission,
    DirectoryListing? listing,
    NearbyNetworks? nearbyNetworks,
    $core.Iterable<Fault>? faults,
  }) {
    final _result = create();
    if (type != null) {
      _result.type = type;
    }
    if (errors != null) {
      _result.errors.addAll(errors);
    }
    if (status != null) {
      _result.status = status;
    }
    if (networkSettings != null) {
      _result.networkSettings = networkSettings;
    }
    if (modules != null) {
      _result.modules.addAll(modules);
    }
    if (streams != null) {
      _result.streams.addAll(streams);
    }
    if (liveReadings != null) {
      _result.liveReadings.addAll(liveReadings);
    }
    if (loraSettings != null) {
      _result.loraSettings = loraSettings;
    }
    if (schedules != null) {
      _result.schedules = schedules;
    }
    if (transmission != null) {
      _result.transmission = transmission;
    }
    if (listing != null) {
      _result.listing = listing;
    }
    if (nearbyNetworks != null) {
      _result.nearbyNetworks = nearbyNetworks;
    }
    if (faults != null) {
      _result.faults.addAll(faults);
    }
    return _result;
  }
  factory HttpReply.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory HttpReply.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  HttpReply clone() => HttpReply()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  HttpReply copyWith(void Function(HttpReply) updates) => super.copyWith((message) => updates(message as HttpReply)) as HttpReply; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static HttpReply create() => HttpReply._();
  HttpReply createEmptyInstance() => create();
  static $pb.PbList<HttpReply> createRepeated() => $pb.PbList<HttpReply>();
  @$core.pragma('dart2js:noInline')
  static HttpReply getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<HttpReply>(create);
  static HttpReply? _defaultInstance;

  @$pb.TagNumber(1)
  ReplyType get type => $_getN(0);
  @$pb.TagNumber(1)
  set type(ReplyType v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasType() => $_has(0);
  @$pb.TagNumber(1)
  void clearType() => clearField(1);

  @$pb.TagNumber(2)
  $core.List<Error> get errors => $_getList(1);

  @$pb.TagNumber(3)
  Status get status => $_getN(2);
  @$pb.TagNumber(3)
  set status(Status v) { setField(3, v); }
  @$pb.TagNumber(3)
  $core.bool hasStatus() => $_has(2);
  @$pb.TagNumber(3)
  void clearStatus() => clearField(3);
  @$pb.TagNumber(3)
  Status ensureStatus() => $_ensure(2);

  @$pb.TagNumber(4)
  NetworkSettings get networkSettings => $_getN(3);
  @$pb.TagNumber(4)
  set networkSettings(NetworkSettings v) { setField(4, v); }
  @$pb.TagNumber(4)
  $core.bool hasNetworkSettings() => $_has(3);
  @$pb.TagNumber(4)
  void clearNetworkSettings() => clearField(4);
  @$pb.TagNumber(4)
  NetworkSettings ensureNetworkSettings() => $_ensure(3);

  @$pb.TagNumber(5)
  $core.List<ModuleCapabilities> get modules => $_getList(4);

  @$pb.TagNumber(6)
  $core.List<DataStream> get streams => $_getList(5);

  @$pb.TagNumber(7)
  $core.List<LiveReadings> get liveReadings => $_getList(6);

  @$pb.TagNumber(8)
  LoraSettings get loraSettings => $_getN(7);
  @$pb.TagNumber(8)
  set loraSettings(LoraSettings v) { setField(8, v); }
  @$pb.TagNumber(8)
  $core.bool hasLoraSettings() => $_has(7);
  @$pb.TagNumber(8)
  void clearLoraSettings() => clearField(8);
  @$pb.TagNumber(8)
  LoraSettings ensureLoraSettings() => $_ensure(7);

  @$pb.TagNumber(9)
  Schedules get schedules => $_getN(8);
  @$pb.TagNumber(9)
  set schedules(Schedules v) { setField(9, v); }
  @$pb.TagNumber(9)
  $core.bool hasSchedules() => $_has(8);
  @$pb.TagNumber(9)
  void clearSchedules() => clearField(9);
  @$pb.TagNumber(9)
  Schedules ensureSchedules() => $_ensure(8);

  @$pb.TagNumber(10)
  Transmission get transmission => $_getN(9);
  @$pb.TagNumber(10)
  set transmission(Transmission v) { setField(10, v); }
  @$pb.TagNumber(10)
  $core.bool hasTransmission() => $_has(9);
  @$pb.TagNumber(10)
  void clearTransmission() => clearField(10);
  @$pb.TagNumber(10)
  Transmission ensureTransmission() => $_ensure(9);

  @$pb.TagNumber(11)
  DirectoryListing get listing => $_getN(10);
  @$pb.TagNumber(11)
  set listing(DirectoryListing v) { setField(11, v); }
  @$pb.TagNumber(11)
  $core.bool hasListing() => $_has(10);
  @$pb.TagNumber(11)
  void clearListing() => clearField(11);
  @$pb.TagNumber(11)
  DirectoryListing ensureListing() => $_ensure(10);

  @$pb.TagNumber(12)
  NearbyNetworks get nearbyNetworks => $_getN(11);
  @$pb.TagNumber(12)
  set nearbyNetworks(NearbyNetworks v) { setField(12, v); }
  @$pb.TagNumber(12)
  $core.bool hasNearbyNetworks() => $_has(11);
  @$pb.TagNumber(12)
  void clearNearbyNetworks() => clearField(12);
  @$pb.TagNumber(12)
  NearbyNetworks ensureNearbyNetworks() => $_ensure(11);

  @$pb.TagNumber(13)
  $core.List<Fault> get faults => $_getList(12);
}

class ModuleHttpQuery extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'ModuleHttpQuery', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..e<ModuleQueryType>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'type', $pb.PbFieldType.OE, defaultOrMaker: ModuleQueryType.MODULE_QUERY_NONE, valueOf: ModuleQueryType.valueOf, enumValues: ModuleQueryType.values)
    ..pc<Error>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'errors', $pb.PbFieldType.PM, subBuilder: Error.create)
    ..a<$core.List<$core.int>>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'configuration', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  ModuleHttpQuery._() : super();
  factory ModuleHttpQuery({
    ModuleQueryType? type,
    $core.Iterable<Error>? errors,
    $core.List<$core.int>? configuration,
  }) {
    final _result = create();
    if (type != null) {
      _result.type = type;
    }
    if (errors != null) {
      _result.errors.addAll(errors);
    }
    if (configuration != null) {
      _result.configuration = configuration;
    }
    return _result;
  }
  factory ModuleHttpQuery.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory ModuleHttpQuery.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  ModuleHttpQuery clone() => ModuleHttpQuery()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  ModuleHttpQuery copyWith(void Function(ModuleHttpQuery) updates) => super.copyWith((message) => updates(message as ModuleHttpQuery)) as ModuleHttpQuery; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static ModuleHttpQuery create() => ModuleHttpQuery._();
  ModuleHttpQuery createEmptyInstance() => create();
  static $pb.PbList<ModuleHttpQuery> createRepeated() => $pb.PbList<ModuleHttpQuery>();
  @$core.pragma('dart2js:noInline')
  static ModuleHttpQuery getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<ModuleHttpQuery>(create);
  static ModuleHttpQuery? _defaultInstance;

  @$pb.TagNumber(1)
  ModuleQueryType get type => $_getN(0);
  @$pb.TagNumber(1)
  set type(ModuleQueryType v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasType() => $_has(0);
  @$pb.TagNumber(1)
  void clearType() => clearField(1);

  @$pb.TagNumber(2)
  $core.List<Error> get errors => $_getList(1);

  @$pb.TagNumber(3)
  $core.List<$core.int> get configuration => $_getN(2);
  @$pb.TagNumber(3)
  set configuration($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasConfiguration() => $_has(2);
  @$pb.TagNumber(3)
  void clearConfiguration() => clearField(3);
}

class ModuleHttpReply extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'ModuleHttpReply', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..e<ModuleReplyType>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'type', $pb.PbFieldType.OE, defaultOrMaker: ModuleReplyType.MODULE_REPLY_NONE, valueOf: ModuleReplyType.valueOf, enumValues: ModuleReplyType.values)
    ..pc<Error>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'errors', $pb.PbFieldType.PM, subBuilder: Error.create)
    ..a<$core.List<$core.int>>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'configuration', $pb.PbFieldType.OY)
    ..hasRequiredFields = false
  ;

  ModuleHttpReply._() : super();
  factory ModuleHttpReply({
    ModuleReplyType? type,
    $core.Iterable<Error>? errors,
    $core.List<$core.int>? configuration,
  }) {
    final _result = create();
    if (type != null) {
      _result.type = type;
    }
    if (errors != null) {
      _result.errors.addAll(errors);
    }
    if (configuration != null) {
      _result.configuration = configuration;
    }
    return _result;
  }
  factory ModuleHttpReply.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory ModuleHttpReply.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  ModuleHttpReply clone() => ModuleHttpReply()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  ModuleHttpReply copyWith(void Function(ModuleHttpReply) updates) => super.copyWith((message) => updates(message as ModuleHttpReply)) as ModuleHttpReply; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static ModuleHttpReply create() => ModuleHttpReply._();
  ModuleHttpReply createEmptyInstance() => create();
  static $pb.PbList<ModuleHttpReply> createRepeated() => $pb.PbList<ModuleHttpReply>();
  @$core.pragma('dart2js:noInline')
  static ModuleHttpReply getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<ModuleHttpReply>(create);
  static ModuleHttpReply? _defaultInstance;

  @$pb.TagNumber(1)
  ModuleReplyType get type => $_getN(0);
  @$pb.TagNumber(1)
  set type(ModuleReplyType v) { setField(1, v); }
  @$pb.TagNumber(1)
  $core.bool hasType() => $_has(0);
  @$pb.TagNumber(1)
  void clearType() => clearField(1);

  @$pb.TagNumber(2)
  $core.List<Error> get errors => $_getList(1);

  @$pb.TagNumber(3)
  $core.List<$core.int> get configuration => $_getN(2);
  @$pb.TagNumber(3)
  set configuration($core.List<$core.int> v) { $_setBytes(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasConfiguration() => $_has(2);
  @$pb.TagNumber(3)
  void clearConfiguration() => clearField(3);
}

class UdpMessage extends $pb.GeneratedMessage {
  static final $pb.BuilderInfo _i = $pb.BuilderInfo(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'UdpMessage', package: const $pb.PackageName(const $core.bool.fromEnvironment('protobuf.omit_message_names') ? '' : 'fk_app'), createEmptyInstance: create)
    ..a<$core.List<$core.int>>(1, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'deviceId', $pb.PbFieldType.OY, protoName: 'deviceId')
    ..e<UdpStatus>(2, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'status', $pb.PbFieldType.OE, defaultOrMaker: UdpStatus.UDP_STATUS_ONLINE, valueOf: UdpStatus.valueOf, enumValues: UdpStatus.values)
    ..a<$core.int>(3, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'counter', $pb.PbFieldType.OU3)
    ..a<$core.int>(4, const $core.bool.fromEnvironment('protobuf.omit_field_names') ? '' : 'port', $pb.PbFieldType.OU3)
    ..hasRequiredFields = false
  ;

  UdpMessage._() : super();
  factory UdpMessage({
    $core.List<$core.int>? deviceId,
    UdpStatus? status,
    $core.int? counter,
    $core.int? port,
  }) {
    final _result = create();
    if (deviceId != null) {
      _result.deviceId = deviceId;
    }
    if (status != null) {
      _result.status = status;
    }
    if (counter != null) {
      _result.counter = counter;
    }
    if (port != null) {
      _result.port = port;
    }
    return _result;
  }
  factory UdpMessage.fromBuffer($core.List<$core.int> i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromBuffer(i, r);
  factory UdpMessage.fromJson($core.String i, [$pb.ExtensionRegistry r = $pb.ExtensionRegistry.EMPTY]) => create()..mergeFromJson(i, r);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.deepCopy] instead. '
  'Will be removed in next major version')
  UdpMessage clone() => UdpMessage()..mergeFromMessage(this);
  @$core.Deprecated(
  'Using this can add significant overhead to your binary. '
  'Use [GeneratedMessageGenericExtensions.rebuild] instead. '
  'Will be removed in next major version')
  UdpMessage copyWith(void Function(UdpMessage) updates) => super.copyWith((message) => updates(message as UdpMessage)) as UdpMessage; // ignore: deprecated_member_use
  $pb.BuilderInfo get info_ => _i;
  @$core.pragma('dart2js:noInline')
  static UdpMessage create() => UdpMessage._();
  UdpMessage createEmptyInstance() => create();
  static $pb.PbList<UdpMessage> createRepeated() => $pb.PbList<UdpMessage>();
  @$core.pragma('dart2js:noInline')
  static UdpMessage getDefault() => _defaultInstance ??= $pb.GeneratedMessage.$_defaultFor<UdpMessage>(create);
  static UdpMessage? _defaultInstance;

  @$pb.TagNumber(1)
  $core.List<$core.int> get deviceId => $_getN(0);
  @$pb.TagNumber(1)
  set deviceId($core.List<$core.int> v) { $_setBytes(0, v); }
  @$pb.TagNumber(1)
  $core.bool hasDeviceId() => $_has(0);
  @$pb.TagNumber(1)
  void clearDeviceId() => clearField(1);

  @$pb.TagNumber(2)
  UdpStatus get status => $_getN(1);
  @$pb.TagNumber(2)
  set status(UdpStatus v) { setField(2, v); }
  @$pb.TagNumber(2)
  $core.bool hasStatus() => $_has(1);
  @$pb.TagNumber(2)
  void clearStatus() => clearField(2);

  @$pb.TagNumber(3)
  $core.int get counter => $_getIZ(2);
  @$pb.TagNumber(3)
  set counter($core.int v) { $_setUnsignedInt32(2, v); }
  @$pb.TagNumber(3)
  $core.bool hasCounter() => $_has(2);
  @$pb.TagNumber(3)
  void clearCounter() => clearField(3);

  @$pb.TagNumber(4)
  $core.int get port => $_getIZ(3);
  @$pb.TagNumber(4)
  set port($core.int v) { $_setUnsignedInt32(3, v); }
  @$pb.TagNumber(4)
  $core.bool hasPort() => $_has(3);
  @$pb.TagNumber(4)
  void clearPort() => clearField(4);
}

