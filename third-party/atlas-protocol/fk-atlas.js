/*eslint-disable block-scoped-var, id-length, no-control-regex, no-magic-numbers, no-prototype-builtins, no-redeclare, no-shadow, no-var, sort-vars*/
(function(global, factory) { /* global define, require, module */

    /* AMD */ if (typeof define === 'function' && define.amd)
        define(["protobufjs/minimal"], factory);

    /* CommonJS */ else if (typeof require === 'function' && typeof module === 'object' && module && module.exports)
        module.exports = factory(require("protobufjs/minimal"));

})(this, function($protobuf) {
    "use strict";

    // Common aliases
    var $Reader = $protobuf.Reader, $Writer = $protobuf.Writer, $util = $protobuf.util;
    
    // Exported root namespace
    var $root = $protobuf.roots["default"] || ($protobuf.roots["default"] = {});
    
    $root.fk_atlas = (function() {
    
        /**
         * Namespace fk_atlas.
         * @exports fk_atlas
         * @namespace
         */
        var fk_atlas = {};
    
        /**
         * SensorType enum.
         * @name fk_atlas.SensorType
         * @enum {string}
         * @property {number} SENSOR_NONE=0 SENSOR_NONE value
         * @property {number} SENSOR_PH=1 SENSOR_PH value
         * @property {number} SENSOR_TEMP=2 SENSOR_TEMP value
         * @property {number} SENSOR_ORP=3 SENSOR_ORP value
         * @property {number} SENSOR_DO=4 SENSOR_DO value
         * @property {number} SENSOR_EC=5 SENSOR_EC value
         */
        fk_atlas.SensorType = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "SENSOR_NONE"] = 0;
            values[valuesById[1] = "SENSOR_PH"] = 1;
            values[valuesById[2] = "SENSOR_TEMP"] = 2;
            values[valuesById[3] = "SENSOR_ORP"] = 3;
            values[valuesById[4] = "SENSOR_DO"] = 4;
            values[valuesById[5] = "SENSOR_EC"] = 5;
            return values;
        })();
    
        /**
         * QueryType enum.
         * @name fk_atlas.QueryType
         * @enum {string}
         * @property {number} QUERY_NONE=0 QUERY_NONE value
         */
        fk_atlas.QueryType = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "QUERY_NONE"] = 0;
            return values;
        })();
    
        /**
         * CalibrationOperation enum.
         * @name fk_atlas.CalibrationOperation
         * @enum {string}
         * @property {number} CALIBRATION_NONE=0 CALIBRATION_NONE value
         * @property {number} CALIBRATION_STATUS=1 CALIBRATION_STATUS value
         * @property {number} CALIBRATION_CLEAR=2 CALIBRATION_CLEAR value
         * @property {number} CALIBRATION_SET=3 CALIBRATION_SET value
         */
        fk_atlas.CalibrationOperation = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "CALIBRATION_NONE"] = 0;
            values[valuesById[1] = "CALIBRATION_STATUS"] = 1;
            values[valuesById[2] = "CALIBRATION_CLEAR"] = 2;
            values[valuesById[3] = "CALIBRATION_SET"] = 3;
            return values;
        })();
    
        /**
         * TempCalibrations enum.
         * @name fk_atlas.TempCalibrations
         * @enum {string}
         * @property {number} TEMP_NONE=0 TEMP_NONE value
         * @property {number} TEMP_SINGLE=1 TEMP_SINGLE value
         */
        fk_atlas.TempCalibrations = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "TEMP_NONE"] = 0;
            values[valuesById[1] = "TEMP_SINGLE"] = 1;
            return values;
        })();
    
        /**
         * TempCalibrateCommand enum.
         * @name fk_atlas.TempCalibrateCommand
         * @enum {string}
         * @property {number} CALIBRATE_TEMP_NONE=0 CALIBRATE_TEMP_NONE value
         * @property {number} CALIBRATE_TEMP_CLEAR=1 CALIBRATE_TEMP_CLEAR value
         * @property {number} CALIBRATE_TEMP_SINGLE=2 CALIBRATE_TEMP_SINGLE value
         */
        fk_atlas.TempCalibrateCommand = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "CALIBRATE_TEMP_NONE"] = 0;
            values[valuesById[1] = "CALIBRATE_TEMP_CLEAR"] = 1;
            values[valuesById[2] = "CALIBRATE_TEMP_SINGLE"] = 2;
            return values;
        })();
    
        /**
         * DoCalibrations enum.
         * @name fk_atlas.DoCalibrations
         * @enum {string}
         * @property {number} DO_NONE=0 DO_NONE value
         * @property {number} DO_ATMOSPHERE=1 DO_ATMOSPHERE value
         * @property {number} DO_ZERO=2 DO_ZERO value
         */
        fk_atlas.DoCalibrations = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "DO_NONE"] = 0;
            values[valuesById[1] = "DO_ATMOSPHERE"] = 1;
            values[valuesById[2] = "DO_ZERO"] = 2;
            return values;
        })();
    
        /**
         * DoCalibrateCommand enum.
         * @name fk_atlas.DoCalibrateCommand
         * @enum {string}
         * @property {number} CALIBRATE_DO_NONE=0 CALIBRATE_DO_NONE value
         * @property {number} CALIBRATE_DO_CLEAR=1 CALIBRATE_DO_CLEAR value
         * @property {number} CALIBRATE_DO_ATMOSPHERE=2 CALIBRATE_DO_ATMOSPHERE value
         * @property {number} CALIBRATE_DO_ZERO=3 CALIBRATE_DO_ZERO value
         */
        fk_atlas.DoCalibrateCommand = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "CALIBRATE_DO_NONE"] = 0;
            values[valuesById[1] = "CALIBRATE_DO_CLEAR"] = 1;
            values[valuesById[2] = "CALIBRATE_DO_ATMOSPHERE"] = 2;
            values[valuesById[3] = "CALIBRATE_DO_ZERO"] = 3;
            return values;
        })();
    
        /**
         * PhCalibrations enum.
         * @name fk_atlas.PhCalibrations
         * @enum {string}
         * @property {number} PH_NONE=0 PH_NONE value
         * @property {number} PH_LOW=1 PH_LOW value
         * @property {number} PH_MIDDLE=2 PH_MIDDLE value
         * @property {number} PH_HIGH=4 PH_HIGH value
         */
        fk_atlas.PhCalibrations = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "PH_NONE"] = 0;
            values[valuesById[1] = "PH_LOW"] = 1;
            values[valuesById[2] = "PH_MIDDLE"] = 2;
            values[valuesById[4] = "PH_HIGH"] = 4;
            return values;
        })();
    
        /**
         * PhCalibrateCommand enum.
         * @name fk_atlas.PhCalibrateCommand
         * @enum {string}
         * @property {number} CALIBRATE_PH_NONE=0 CALIBRATE_PH_NONE value
         * @property {number} CALIBRATE_PH_CLEAR=1 CALIBRATE_PH_CLEAR value
         * @property {number} CALIBRATE_PH_LOW=2 CALIBRATE_PH_LOW value
         * @property {number} CALIBRATE_PH_MIDDLE=3 CALIBRATE_PH_MIDDLE value
         * @property {number} CALIBRATE_PH_HIGH=4 CALIBRATE_PH_HIGH value
         */
        fk_atlas.PhCalibrateCommand = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "CALIBRATE_PH_NONE"] = 0;
            values[valuesById[1] = "CALIBRATE_PH_CLEAR"] = 1;
            values[valuesById[2] = "CALIBRATE_PH_LOW"] = 2;
            values[valuesById[3] = "CALIBRATE_PH_MIDDLE"] = 3;
            values[valuesById[4] = "CALIBRATE_PH_HIGH"] = 4;
            return values;
        })();
    
        /**
         * EcCalibrations enum.
         * @name fk_atlas.EcCalibrations
         * @enum {string}
         * @property {number} EC_NONE=0 EC_NONE value
         * @property {number} EC_DRY=1 EC_DRY value
         * @property {number} EC_SINGLE=2 EC_SINGLE value
         * @property {number} EC_DUAL_LOW=4 EC_DUAL_LOW value
         * @property {number} EC_DUAL_HIGH=8 EC_DUAL_HIGH value
         */
        fk_atlas.EcCalibrations = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "EC_NONE"] = 0;
            values[valuesById[1] = "EC_DRY"] = 1;
            values[valuesById[2] = "EC_SINGLE"] = 2;
            values[valuesById[4] = "EC_DUAL_LOW"] = 4;
            values[valuesById[8] = "EC_DUAL_HIGH"] = 8;
            return values;
        })();
    
        /**
         * EcCalibrateCommand enum.
         * @name fk_atlas.EcCalibrateCommand
         * @enum {string}
         * @property {number} CALIBRATE_EC_NONE=0 CALIBRATE_EC_NONE value
         * @property {number} CALIBRATE_EC_CLEAR=1 CALIBRATE_EC_CLEAR value
         * @property {number} CALIBRATE_EC_DRY=2 CALIBRATE_EC_DRY value
         * @property {number} CALIBRATE_EC_SINGLE=3 CALIBRATE_EC_SINGLE value
         * @property {number} CALIBRATE_EC_DUAL_LOW=4 CALIBRATE_EC_DUAL_LOW value
         * @property {number} CALIBRATE_EC_DUAL_HIGH=5 CALIBRATE_EC_DUAL_HIGH value
         */
        fk_atlas.EcCalibrateCommand = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "CALIBRATE_EC_NONE"] = 0;
            values[valuesById[1] = "CALIBRATE_EC_CLEAR"] = 1;
            values[valuesById[2] = "CALIBRATE_EC_DRY"] = 2;
            values[valuesById[3] = "CALIBRATE_EC_SINGLE"] = 3;
            values[valuesById[4] = "CALIBRATE_EC_DUAL_LOW"] = 4;
            values[valuesById[5] = "CALIBRATE_EC_DUAL_HIGH"] = 5;
            return values;
        })();
    
        /**
         * OrpCalibrations enum.
         * @name fk_atlas.OrpCalibrations
         * @enum {string}
         * @property {number} ORP_NONE=0 ORP_NONE value
         * @property {number} ORP_SINGLE=1 ORP_SINGLE value
         */
        fk_atlas.OrpCalibrations = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "ORP_NONE"] = 0;
            values[valuesById[1] = "ORP_SINGLE"] = 1;
            return values;
        })();
    
        /**
         * OrpCalibrateCommand enum.
         * @name fk_atlas.OrpCalibrateCommand
         * @enum {string}
         * @property {number} CALIBRATE_ORP_NONE=0 CALIBRATE_ORP_NONE value
         * @property {number} CALIBRATE_ORP_SINGLE=1 CALIBRATE_ORP_SINGLE value
         */
        fk_atlas.OrpCalibrateCommand = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "CALIBRATE_ORP_NONE"] = 0;
            values[valuesById[1] = "CALIBRATE_ORP_SINGLE"] = 1;
            return values;
        })();
    
        fk_atlas.AtlasCalibrationCommand = (function() {
    
            /**
             * Properties of an AtlasCalibrationCommand.
             * @memberof fk_atlas
             * @interface IAtlasCalibrationCommand
             * @property {fk_atlas.CalibrationOperation|null} [operation] AtlasCalibrationCommand operation
             * @property {fk_atlas.TempCalibrations|null} [temp] AtlasCalibrationCommand temp
             * @property {fk_atlas.DoCalibrations|null} [dissolvedOxygen] AtlasCalibrationCommand dissolvedOxygen
             * @property {fk_atlas.PhCalibrations|null} [ph] AtlasCalibrationCommand ph
             * @property {fk_atlas.EcCalibrations|null} [ec] AtlasCalibrationCommand ec
             * @property {fk_atlas.OrpCalibrations|null} [orp] AtlasCalibrationCommand orp
             * @property {number|null} [which] AtlasCalibrationCommand which
             * @property {number|null} [value] AtlasCalibrationCommand value
             * @property {Uint8Array|null} [configuration] AtlasCalibrationCommand configuration
             */
    
            /**
             * Constructs a new AtlasCalibrationCommand.
             * @memberof fk_atlas
             * @classdesc Represents an AtlasCalibrationCommand.
             * @implements IAtlasCalibrationCommand
             * @constructor
             * @param {fk_atlas.IAtlasCalibrationCommand=} [properties] Properties to set
             */
            function AtlasCalibrationCommand(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * AtlasCalibrationCommand operation.
             * @member {fk_atlas.CalibrationOperation} operation
             * @memberof fk_atlas.AtlasCalibrationCommand
             * @instance
             */
            AtlasCalibrationCommand.prototype.operation = 0;
    
            /**
             * AtlasCalibrationCommand temp.
             * @member {fk_atlas.TempCalibrations} temp
             * @memberof fk_atlas.AtlasCalibrationCommand
             * @instance
             */
            AtlasCalibrationCommand.prototype.temp = 0;
    
            /**
             * AtlasCalibrationCommand dissolvedOxygen.
             * @member {fk_atlas.DoCalibrations} dissolvedOxygen
             * @memberof fk_atlas.AtlasCalibrationCommand
             * @instance
             */
            AtlasCalibrationCommand.prototype.dissolvedOxygen = 0;
    
            /**
             * AtlasCalibrationCommand ph.
             * @member {fk_atlas.PhCalibrations} ph
             * @memberof fk_atlas.AtlasCalibrationCommand
             * @instance
             */
            AtlasCalibrationCommand.prototype.ph = 0;
    
            /**
             * AtlasCalibrationCommand ec.
             * @member {fk_atlas.EcCalibrations} ec
             * @memberof fk_atlas.AtlasCalibrationCommand
             * @instance
             */
            AtlasCalibrationCommand.prototype.ec = 0;
    
            /**
             * AtlasCalibrationCommand orp.
             * @member {fk_atlas.OrpCalibrations} orp
             * @memberof fk_atlas.AtlasCalibrationCommand
             * @instance
             */
            AtlasCalibrationCommand.prototype.orp = 0;
    
            /**
             * AtlasCalibrationCommand which.
             * @member {number} which
             * @memberof fk_atlas.AtlasCalibrationCommand
             * @instance
             */
            AtlasCalibrationCommand.prototype.which = 0;
    
            /**
             * AtlasCalibrationCommand value.
             * @member {number} value
             * @memberof fk_atlas.AtlasCalibrationCommand
             * @instance
             */
            AtlasCalibrationCommand.prototype.value = 0;
    
            /**
             * AtlasCalibrationCommand configuration.
             * @member {Uint8Array} configuration
             * @memberof fk_atlas.AtlasCalibrationCommand
             * @instance
             */
            AtlasCalibrationCommand.prototype.configuration = $util.newBuffer([]);
    
            /**
             * Creates a new AtlasCalibrationCommand instance using the specified properties.
             * @function create
             * @memberof fk_atlas.AtlasCalibrationCommand
             * @static
             * @param {fk_atlas.IAtlasCalibrationCommand=} [properties] Properties to set
             * @returns {fk_atlas.AtlasCalibrationCommand} AtlasCalibrationCommand instance
             */
            AtlasCalibrationCommand.create = function create(properties) {
                return new AtlasCalibrationCommand(properties);
            };
    
            /**
             * Encodes the specified AtlasCalibrationCommand message. Does not implicitly {@link fk_atlas.AtlasCalibrationCommand.verify|verify} messages.
             * @function encode
             * @memberof fk_atlas.AtlasCalibrationCommand
             * @static
             * @param {fk_atlas.IAtlasCalibrationCommand} message AtlasCalibrationCommand message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            AtlasCalibrationCommand.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.operation != null && message.hasOwnProperty("operation"))
                    writer.uint32(/* id 1, wireType 0 =*/8).int32(message.operation);
                if (message.temp != null && message.hasOwnProperty("temp"))
                    writer.uint32(/* id 2, wireType 0 =*/16).int32(message.temp);
                if (message.dissolvedOxygen != null && message.hasOwnProperty("dissolvedOxygen"))
                    writer.uint32(/* id 3, wireType 0 =*/24).int32(message.dissolvedOxygen);
                if (message.ph != null && message.hasOwnProperty("ph"))
                    writer.uint32(/* id 4, wireType 0 =*/32).int32(message.ph);
                if (message.ec != null && message.hasOwnProperty("ec"))
                    writer.uint32(/* id 5, wireType 0 =*/40).int32(message.ec);
                if (message.orp != null && message.hasOwnProperty("orp"))
                    writer.uint32(/* id 6, wireType 0 =*/48).int32(message.orp);
                if (message.value != null && message.hasOwnProperty("value"))
                    writer.uint32(/* id 7, wireType 5 =*/61).float(message.value);
                if (message.which != null && message.hasOwnProperty("which"))
                    writer.uint32(/* id 8, wireType 0 =*/64).uint32(message.which);
                if (message.configuration != null && message.hasOwnProperty("configuration"))
                    writer.uint32(/* id 9, wireType 2 =*/74).bytes(message.configuration);
                return writer;
            };
    
            /**
             * Encodes the specified AtlasCalibrationCommand message, length delimited. Does not implicitly {@link fk_atlas.AtlasCalibrationCommand.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_atlas.AtlasCalibrationCommand
             * @static
             * @param {fk_atlas.IAtlasCalibrationCommand} message AtlasCalibrationCommand message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            AtlasCalibrationCommand.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes an AtlasCalibrationCommand message from the specified reader or buffer.
             * @function decode
             * @memberof fk_atlas.AtlasCalibrationCommand
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_atlas.AtlasCalibrationCommand} AtlasCalibrationCommand
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            AtlasCalibrationCommand.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_atlas.AtlasCalibrationCommand();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.operation = reader.int32();
                        break;
                    case 2:
                        message.temp = reader.int32();
                        break;
                    case 3:
                        message.dissolvedOxygen = reader.int32();
                        break;
                    case 4:
                        message.ph = reader.int32();
                        break;
                    case 5:
                        message.ec = reader.int32();
                        break;
                    case 6:
                        message.orp = reader.int32();
                        break;
                    case 8:
                        message.which = reader.uint32();
                        break;
                    case 7:
                        message.value = reader.float();
                        break;
                    case 9:
                        message.configuration = reader.bytes();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes an AtlasCalibrationCommand message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_atlas.AtlasCalibrationCommand
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_atlas.AtlasCalibrationCommand} AtlasCalibrationCommand
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            AtlasCalibrationCommand.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies an AtlasCalibrationCommand message.
             * @function verify
             * @memberof fk_atlas.AtlasCalibrationCommand
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            AtlasCalibrationCommand.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.operation != null && message.hasOwnProperty("operation"))
                    switch (message.operation) {
                    default:
                        return "operation: enum value expected";
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                        break;
                    }
                if (message.temp != null && message.hasOwnProperty("temp"))
                    switch (message.temp) {
                    default:
                        return "temp: enum value expected";
                    case 0:
                    case 1:
                        break;
                    }
                if (message.dissolvedOxygen != null && message.hasOwnProperty("dissolvedOxygen"))
                    switch (message.dissolvedOxygen) {
                    default:
                        return "dissolvedOxygen: enum value expected";
                    case 0:
                    case 1:
                    case 2:
                        break;
                    }
                if (message.ph != null && message.hasOwnProperty("ph"))
                    switch (message.ph) {
                    default:
                        return "ph: enum value expected";
                    case 0:
                    case 1:
                    case 2:
                    case 4:
                        break;
                    }
                if (message.ec != null && message.hasOwnProperty("ec"))
                    switch (message.ec) {
                    default:
                        return "ec: enum value expected";
                    case 0:
                    case 1:
                    case 2:
                    case 4:
                    case 8:
                        break;
                    }
                if (message.orp != null && message.hasOwnProperty("orp"))
                    switch (message.orp) {
                    default:
                        return "orp: enum value expected";
                    case 0:
                    case 1:
                        break;
                    }
                if (message.which != null && message.hasOwnProperty("which"))
                    if (!$util.isInteger(message.which))
                        return "which: integer expected";
                if (message.value != null && message.hasOwnProperty("value"))
                    if (typeof message.value !== "number")
                        return "value: number expected";
                if (message.configuration != null && message.hasOwnProperty("configuration"))
                    if (!(message.configuration && typeof message.configuration.length === "number" || $util.isString(message.configuration)))
                        return "configuration: buffer expected";
                return null;
            };
    
            /**
             * Creates an AtlasCalibrationCommand message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_atlas.AtlasCalibrationCommand
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_atlas.AtlasCalibrationCommand} AtlasCalibrationCommand
             */
            AtlasCalibrationCommand.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_atlas.AtlasCalibrationCommand)
                    return object;
                var message = new $root.fk_atlas.AtlasCalibrationCommand();
                switch (object.operation) {
                case "CALIBRATION_NONE":
                case 0:
                    message.operation = 0;
                    break;
                case "CALIBRATION_STATUS":
                case 1:
                    message.operation = 1;
                    break;
                case "CALIBRATION_CLEAR":
                case 2:
                    message.operation = 2;
                    break;
                case "CALIBRATION_SET":
                case 3:
                    message.operation = 3;
                    break;
                }
                switch (object.temp) {
                case "TEMP_NONE":
                case 0:
                    message.temp = 0;
                    break;
                case "TEMP_SINGLE":
                case 1:
                    message.temp = 1;
                    break;
                }
                switch (object.dissolvedOxygen) {
                case "DO_NONE":
                case 0:
                    message.dissolvedOxygen = 0;
                    break;
                case "DO_ATMOSPHERE":
                case 1:
                    message.dissolvedOxygen = 1;
                    break;
                case "DO_ZERO":
                case 2:
                    message.dissolvedOxygen = 2;
                    break;
                }
                switch (object.ph) {
                case "PH_NONE":
                case 0:
                    message.ph = 0;
                    break;
                case "PH_LOW":
                case 1:
                    message.ph = 1;
                    break;
                case "PH_MIDDLE":
                case 2:
                    message.ph = 2;
                    break;
                case "PH_HIGH":
                case 4:
                    message.ph = 4;
                    break;
                }
                switch (object.ec) {
                case "EC_NONE":
                case 0:
                    message.ec = 0;
                    break;
                case "EC_DRY":
                case 1:
                    message.ec = 1;
                    break;
                case "EC_SINGLE":
                case 2:
                    message.ec = 2;
                    break;
                case "EC_DUAL_LOW":
                case 4:
                    message.ec = 4;
                    break;
                case "EC_DUAL_HIGH":
                case 8:
                    message.ec = 8;
                    break;
                }
                switch (object.orp) {
                case "ORP_NONE":
                case 0:
                    message.orp = 0;
                    break;
                case "ORP_SINGLE":
                case 1:
                    message.orp = 1;
                    break;
                }
                if (object.which != null)
                    message.which = object.which >>> 0;
                if (object.value != null)
                    message.value = Number(object.value);
                if (object.configuration != null)
                    if (typeof object.configuration === "string")
                        $util.base64.decode(object.configuration, message.configuration = $util.newBuffer($util.base64.length(object.configuration)), 0);
                    else if (object.configuration.length)
                        message.configuration = object.configuration;
                return message;
            };
    
            /**
             * Creates a plain object from an AtlasCalibrationCommand message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_atlas.AtlasCalibrationCommand
             * @static
             * @param {fk_atlas.AtlasCalibrationCommand} message AtlasCalibrationCommand
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            AtlasCalibrationCommand.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.operation = options.enums === String ? "CALIBRATION_NONE" : 0;
                    object.temp = options.enums === String ? "TEMP_NONE" : 0;
                    object.dissolvedOxygen = options.enums === String ? "DO_NONE" : 0;
                    object.ph = options.enums === String ? "PH_NONE" : 0;
                    object.ec = options.enums === String ? "EC_NONE" : 0;
                    object.orp = options.enums === String ? "ORP_NONE" : 0;
                    object.value = 0;
                    object.which = 0;
                    if (options.bytes === String)
                        object.configuration = "";
                    else {
                        object.configuration = [];
                        if (options.bytes !== Array)
                            object.configuration = $util.newBuffer(object.configuration);
                    }
                }
                if (message.operation != null && message.hasOwnProperty("operation"))
                    object.operation = options.enums === String ? $root.fk_atlas.CalibrationOperation[message.operation] : message.operation;
                if (message.temp != null && message.hasOwnProperty("temp"))
                    object.temp = options.enums === String ? $root.fk_atlas.TempCalibrations[message.temp] : message.temp;
                if (message.dissolvedOxygen != null && message.hasOwnProperty("dissolvedOxygen"))
                    object.dissolvedOxygen = options.enums === String ? $root.fk_atlas.DoCalibrations[message.dissolvedOxygen] : message.dissolvedOxygen;
                if (message.ph != null && message.hasOwnProperty("ph"))
                    object.ph = options.enums === String ? $root.fk_atlas.PhCalibrations[message.ph] : message.ph;
                if (message.ec != null && message.hasOwnProperty("ec"))
                    object.ec = options.enums === String ? $root.fk_atlas.EcCalibrations[message.ec] : message.ec;
                if (message.orp != null && message.hasOwnProperty("orp"))
                    object.orp = options.enums === String ? $root.fk_atlas.OrpCalibrations[message.orp] : message.orp;
                if (message.value != null && message.hasOwnProperty("value"))
                    object.value = options.json && !isFinite(message.value) ? String(message.value) : message.value;
                if (message.which != null && message.hasOwnProperty("which"))
                    object.which = message.which;
                if (message.configuration != null && message.hasOwnProperty("configuration"))
                    object.configuration = options.bytes === String ? $util.base64.encode(message.configuration, 0, message.configuration.length) : options.bytes === Array ? Array.prototype.slice.call(message.configuration) : message.configuration;
                return object;
            };
    
            /**
             * Converts this AtlasCalibrationCommand to JSON.
             * @function toJSON
             * @memberof fk_atlas.AtlasCalibrationCommand
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            AtlasCalibrationCommand.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return AtlasCalibrationCommand;
        })();
    
        fk_atlas.TwoWireQuery = (function() {
    
            /**
             * Properties of a TwoWireQuery.
             * @memberof fk_atlas
             * @interface ITwoWireQuery
             * @property {number|null} [read] TwoWireQuery read
             * @property {number|null} [write] TwoWireQuery write
             * @property {Uint8Array|null} [data] TwoWireQuery data
             */
    
            /**
             * Constructs a new TwoWireQuery.
             * @memberof fk_atlas
             * @classdesc Represents a TwoWireQuery.
             * @implements ITwoWireQuery
             * @constructor
             * @param {fk_atlas.ITwoWireQuery=} [properties] Properties to set
             */
            function TwoWireQuery(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * TwoWireQuery read.
             * @member {number} read
             * @memberof fk_atlas.TwoWireQuery
             * @instance
             */
            TwoWireQuery.prototype.read = 0;
    
            /**
             * TwoWireQuery write.
             * @member {number} write
             * @memberof fk_atlas.TwoWireQuery
             * @instance
             */
            TwoWireQuery.prototype.write = 0;
    
            /**
             * TwoWireQuery data.
             * @member {Uint8Array} data
             * @memberof fk_atlas.TwoWireQuery
             * @instance
             */
            TwoWireQuery.prototype.data = $util.newBuffer([]);
    
            /**
             * Creates a new TwoWireQuery instance using the specified properties.
             * @function create
             * @memberof fk_atlas.TwoWireQuery
             * @static
             * @param {fk_atlas.ITwoWireQuery=} [properties] Properties to set
             * @returns {fk_atlas.TwoWireQuery} TwoWireQuery instance
             */
            TwoWireQuery.create = function create(properties) {
                return new TwoWireQuery(properties);
            };
    
            /**
             * Encodes the specified TwoWireQuery message. Does not implicitly {@link fk_atlas.TwoWireQuery.verify|verify} messages.
             * @function encode
             * @memberof fk_atlas.TwoWireQuery
             * @static
             * @param {fk_atlas.ITwoWireQuery} message TwoWireQuery message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            TwoWireQuery.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.read != null && message.hasOwnProperty("read"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.read);
                if (message.write != null && message.hasOwnProperty("write"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint32(message.write);
                if (message.data != null && message.hasOwnProperty("data"))
                    writer.uint32(/* id 3, wireType 2 =*/26).bytes(message.data);
                return writer;
            };
    
            /**
             * Encodes the specified TwoWireQuery message, length delimited. Does not implicitly {@link fk_atlas.TwoWireQuery.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_atlas.TwoWireQuery
             * @static
             * @param {fk_atlas.ITwoWireQuery} message TwoWireQuery message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            TwoWireQuery.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a TwoWireQuery message from the specified reader or buffer.
             * @function decode
             * @memberof fk_atlas.TwoWireQuery
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_atlas.TwoWireQuery} TwoWireQuery
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            TwoWireQuery.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_atlas.TwoWireQuery();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.read = reader.uint32();
                        break;
                    case 2:
                        message.write = reader.uint32();
                        break;
                    case 3:
                        message.data = reader.bytes();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a TwoWireQuery message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_atlas.TwoWireQuery
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_atlas.TwoWireQuery} TwoWireQuery
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            TwoWireQuery.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a TwoWireQuery message.
             * @function verify
             * @memberof fk_atlas.TwoWireQuery
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            TwoWireQuery.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.read != null && message.hasOwnProperty("read"))
                    if (!$util.isInteger(message.read))
                        return "read: integer expected";
                if (message.write != null && message.hasOwnProperty("write"))
                    if (!$util.isInteger(message.write))
                        return "write: integer expected";
                if (message.data != null && message.hasOwnProperty("data"))
                    if (!(message.data && typeof message.data.length === "number" || $util.isString(message.data)))
                        return "data: buffer expected";
                return null;
            };
    
            /**
             * Creates a TwoWireQuery message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_atlas.TwoWireQuery
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_atlas.TwoWireQuery} TwoWireQuery
             */
            TwoWireQuery.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_atlas.TwoWireQuery)
                    return object;
                var message = new $root.fk_atlas.TwoWireQuery();
                if (object.read != null)
                    message.read = object.read >>> 0;
                if (object.write != null)
                    message.write = object.write >>> 0;
                if (object.data != null)
                    if (typeof object.data === "string")
                        $util.base64.decode(object.data, message.data = $util.newBuffer($util.base64.length(object.data)), 0);
                    else if (object.data.length)
                        message.data = object.data;
                return message;
            };
    
            /**
             * Creates a plain object from a TwoWireQuery message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_atlas.TwoWireQuery
             * @static
             * @param {fk_atlas.TwoWireQuery} message TwoWireQuery
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            TwoWireQuery.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.read = 0;
                    object.write = 0;
                    if (options.bytes === String)
                        object.data = "";
                    else {
                        object.data = [];
                        if (options.bytes !== Array)
                            object.data = $util.newBuffer(object.data);
                    }
                }
                if (message.read != null && message.hasOwnProperty("read"))
                    object.read = message.read;
                if (message.write != null && message.hasOwnProperty("write"))
                    object.write = message.write;
                if (message.data != null && message.hasOwnProperty("data"))
                    object.data = options.bytes === String ? $util.base64.encode(message.data, 0, message.data.length) : options.bytes === Array ? Array.prototype.slice.call(message.data) : message.data;
                return object;
            };
    
            /**
             * Converts this TwoWireQuery to JSON.
             * @function toJSON
             * @memberof fk_atlas.TwoWireQuery
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            TwoWireQuery.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return TwoWireQuery;
        })();
    
        fk_atlas.Compensations = (function() {
    
            /**
             * Properties of a Compensations.
             * @memberof fk_atlas
             * @interface ICompensations
             * @property {number|null} [temperature] Compensations temperature
             */
    
            /**
             * Constructs a new Compensations.
             * @memberof fk_atlas
             * @classdesc Represents a Compensations.
             * @implements ICompensations
             * @constructor
             * @param {fk_atlas.ICompensations=} [properties] Properties to set
             */
            function Compensations(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Compensations temperature.
             * @member {number} temperature
             * @memberof fk_atlas.Compensations
             * @instance
             */
            Compensations.prototype.temperature = 0;
    
            /**
             * Creates a new Compensations instance using the specified properties.
             * @function create
             * @memberof fk_atlas.Compensations
             * @static
             * @param {fk_atlas.ICompensations=} [properties] Properties to set
             * @returns {fk_atlas.Compensations} Compensations instance
             */
            Compensations.create = function create(properties) {
                return new Compensations(properties);
            };
    
            /**
             * Encodes the specified Compensations message. Does not implicitly {@link fk_atlas.Compensations.verify|verify} messages.
             * @function encode
             * @memberof fk_atlas.Compensations
             * @static
             * @param {fk_atlas.ICompensations} message Compensations message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Compensations.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.temperature != null && message.hasOwnProperty("temperature"))
                    writer.uint32(/* id 1, wireType 5 =*/13).float(message.temperature);
                return writer;
            };
    
            /**
             * Encodes the specified Compensations message, length delimited. Does not implicitly {@link fk_atlas.Compensations.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_atlas.Compensations
             * @static
             * @param {fk_atlas.ICompensations} message Compensations message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Compensations.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a Compensations message from the specified reader or buffer.
             * @function decode
             * @memberof fk_atlas.Compensations
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_atlas.Compensations} Compensations
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Compensations.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_atlas.Compensations();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.temperature = reader.float();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a Compensations message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_atlas.Compensations
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_atlas.Compensations} Compensations
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Compensations.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a Compensations message.
             * @function verify
             * @memberof fk_atlas.Compensations
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            Compensations.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.temperature != null && message.hasOwnProperty("temperature"))
                    if (typeof message.temperature !== "number")
                        return "temperature: number expected";
                return null;
            };
    
            /**
             * Creates a Compensations message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_atlas.Compensations
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_atlas.Compensations} Compensations
             */
            Compensations.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_atlas.Compensations)
                    return object;
                var message = new $root.fk_atlas.Compensations();
                if (object.temperature != null)
                    message.temperature = Number(object.temperature);
                return message;
            };
    
            /**
             * Creates a plain object from a Compensations message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_atlas.Compensations
             * @static
             * @param {fk_atlas.Compensations} message Compensations
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            Compensations.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults)
                    object.temperature = 0;
                if (message.temperature != null && message.hasOwnProperty("temperature"))
                    object.temperature = options.json && !isFinite(message.temperature) ? String(message.temperature) : message.temperature;
                return object;
            };
    
            /**
             * Converts this Compensations to JSON.
             * @function toJSON
             * @memberof fk_atlas.Compensations
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Compensations.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Compensations;
        })();
    
        fk_atlas.WireAtlasQuery = (function() {
    
            /**
             * Properties of a WireAtlasQuery.
             * @memberof fk_atlas
             * @interface IWireAtlasQuery
             * @property {fk_atlas.QueryType|null} [type] WireAtlasQuery type
             * @property {fk_atlas.IAtlasCalibrationCommand|null} [calibration] WireAtlasQuery calibration
             * @property {fk_atlas.ICompensations|null} [compensations] WireAtlasQuery compensations
             * @property {fk_atlas.ITwoWireQuery|null} [wire] WireAtlasQuery wire
             */
    
            /**
             * Constructs a new WireAtlasQuery.
             * @memberof fk_atlas
             * @classdesc Represents a WireAtlasQuery.
             * @implements IWireAtlasQuery
             * @constructor
             * @param {fk_atlas.IWireAtlasQuery=} [properties] Properties to set
             */
            function WireAtlasQuery(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * WireAtlasQuery type.
             * @member {fk_atlas.QueryType} type
             * @memberof fk_atlas.WireAtlasQuery
             * @instance
             */
            WireAtlasQuery.prototype.type = 0;
    
            /**
             * WireAtlasQuery calibration.
             * @member {fk_atlas.IAtlasCalibrationCommand|null|undefined} calibration
             * @memberof fk_atlas.WireAtlasQuery
             * @instance
             */
            WireAtlasQuery.prototype.calibration = null;
    
            /**
             * WireAtlasQuery compensations.
             * @member {fk_atlas.ICompensations|null|undefined} compensations
             * @memberof fk_atlas.WireAtlasQuery
             * @instance
             */
            WireAtlasQuery.prototype.compensations = null;
    
            /**
             * WireAtlasQuery wire.
             * @member {fk_atlas.ITwoWireQuery|null|undefined} wire
             * @memberof fk_atlas.WireAtlasQuery
             * @instance
             */
            WireAtlasQuery.prototype.wire = null;
    
            /**
             * Creates a new WireAtlasQuery instance using the specified properties.
             * @function create
             * @memberof fk_atlas.WireAtlasQuery
             * @static
             * @param {fk_atlas.IWireAtlasQuery=} [properties] Properties to set
             * @returns {fk_atlas.WireAtlasQuery} WireAtlasQuery instance
             */
            WireAtlasQuery.create = function create(properties) {
                return new WireAtlasQuery(properties);
            };
    
            /**
             * Encodes the specified WireAtlasQuery message. Does not implicitly {@link fk_atlas.WireAtlasQuery.verify|verify} messages.
             * @function encode
             * @memberof fk_atlas.WireAtlasQuery
             * @static
             * @param {fk_atlas.IWireAtlasQuery} message WireAtlasQuery message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            WireAtlasQuery.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.type != null && message.hasOwnProperty("type"))
                    writer.uint32(/* id 1, wireType 0 =*/8).int32(message.type);
                if (message.calibration != null && message.hasOwnProperty("calibration"))
                    $root.fk_atlas.AtlasCalibrationCommand.encode(message.calibration, writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
                if (message.compensations != null && message.hasOwnProperty("compensations"))
                    $root.fk_atlas.Compensations.encode(message.compensations, writer.uint32(/* id 3, wireType 2 =*/26).fork()).ldelim();
                if (message.wire != null && message.hasOwnProperty("wire"))
                    $root.fk_atlas.TwoWireQuery.encode(message.wire, writer.uint32(/* id 4, wireType 2 =*/34).fork()).ldelim();
                return writer;
            };
    
            /**
             * Encodes the specified WireAtlasQuery message, length delimited. Does not implicitly {@link fk_atlas.WireAtlasQuery.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_atlas.WireAtlasQuery
             * @static
             * @param {fk_atlas.IWireAtlasQuery} message WireAtlasQuery message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            WireAtlasQuery.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a WireAtlasQuery message from the specified reader or buffer.
             * @function decode
             * @memberof fk_atlas.WireAtlasQuery
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_atlas.WireAtlasQuery} WireAtlasQuery
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            WireAtlasQuery.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_atlas.WireAtlasQuery();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.type = reader.int32();
                        break;
                    case 2:
                        message.calibration = $root.fk_atlas.AtlasCalibrationCommand.decode(reader, reader.uint32());
                        break;
                    case 3:
                        message.compensations = $root.fk_atlas.Compensations.decode(reader, reader.uint32());
                        break;
                    case 4:
                        message.wire = $root.fk_atlas.TwoWireQuery.decode(reader, reader.uint32());
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a WireAtlasQuery message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_atlas.WireAtlasQuery
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_atlas.WireAtlasQuery} WireAtlasQuery
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            WireAtlasQuery.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a WireAtlasQuery message.
             * @function verify
             * @memberof fk_atlas.WireAtlasQuery
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            WireAtlasQuery.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.type != null && message.hasOwnProperty("type"))
                    switch (message.type) {
                    default:
                        return "type: enum value expected";
                    case 0:
                        break;
                    }
                if (message.calibration != null && message.hasOwnProperty("calibration")) {
                    var error = $root.fk_atlas.AtlasCalibrationCommand.verify(message.calibration);
                    if (error)
                        return "calibration." + error;
                }
                if (message.compensations != null && message.hasOwnProperty("compensations")) {
                    var error = $root.fk_atlas.Compensations.verify(message.compensations);
                    if (error)
                        return "compensations." + error;
                }
                if (message.wire != null && message.hasOwnProperty("wire")) {
                    var error = $root.fk_atlas.TwoWireQuery.verify(message.wire);
                    if (error)
                        return "wire." + error;
                }
                return null;
            };
    
            /**
             * Creates a WireAtlasQuery message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_atlas.WireAtlasQuery
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_atlas.WireAtlasQuery} WireAtlasQuery
             */
            WireAtlasQuery.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_atlas.WireAtlasQuery)
                    return object;
                var message = new $root.fk_atlas.WireAtlasQuery();
                switch (object.type) {
                case "QUERY_NONE":
                case 0:
                    message.type = 0;
                    break;
                }
                if (object.calibration != null) {
                    if (typeof object.calibration !== "object")
                        throw TypeError(".fk_atlas.WireAtlasQuery.calibration: object expected");
                    message.calibration = $root.fk_atlas.AtlasCalibrationCommand.fromObject(object.calibration);
                }
                if (object.compensations != null) {
                    if (typeof object.compensations !== "object")
                        throw TypeError(".fk_atlas.WireAtlasQuery.compensations: object expected");
                    message.compensations = $root.fk_atlas.Compensations.fromObject(object.compensations);
                }
                if (object.wire != null) {
                    if (typeof object.wire !== "object")
                        throw TypeError(".fk_atlas.WireAtlasQuery.wire: object expected");
                    message.wire = $root.fk_atlas.TwoWireQuery.fromObject(object.wire);
                }
                return message;
            };
    
            /**
             * Creates a plain object from a WireAtlasQuery message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_atlas.WireAtlasQuery
             * @static
             * @param {fk_atlas.WireAtlasQuery} message WireAtlasQuery
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            WireAtlasQuery.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.type = options.enums === String ? "QUERY_NONE" : 0;
                    object.calibration = null;
                    object.compensations = null;
                    object.wire = null;
                }
                if (message.type != null && message.hasOwnProperty("type"))
                    object.type = options.enums === String ? $root.fk_atlas.QueryType[message.type] : message.type;
                if (message.calibration != null && message.hasOwnProperty("calibration"))
                    object.calibration = $root.fk_atlas.AtlasCalibrationCommand.toObject(message.calibration, options);
                if (message.compensations != null && message.hasOwnProperty("compensations"))
                    object.compensations = $root.fk_atlas.Compensations.toObject(message.compensations, options);
                if (message.wire != null && message.hasOwnProperty("wire"))
                    object.wire = $root.fk_atlas.TwoWireQuery.toObject(message.wire, options);
                return object;
            };
    
            /**
             * Converts this WireAtlasQuery to JSON.
             * @function toJSON
             * @memberof fk_atlas.WireAtlasQuery
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            WireAtlasQuery.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return WireAtlasQuery;
        })();
    
        fk_atlas.AtlasCalibrationStatus = (function() {
    
            /**
             * Properties of an AtlasCalibrationStatus.
             * @memberof fk_atlas
             * @interface IAtlasCalibrationStatus
             * @property {fk_atlas.SensorType|null} [type] AtlasCalibrationStatus type
             * @property {number|null} [time] AtlasCalibrationStatus time
             * @property {number|null} [raw] AtlasCalibrationStatus raw
             * @property {fk_atlas.TempCalibrations|null} [temp] AtlasCalibrationStatus temp
             * @property {fk_atlas.DoCalibrations|null} [dissolvedOxygen] AtlasCalibrationStatus dissolvedOxygen
             * @property {fk_atlas.PhCalibrations|null} [ph] AtlasCalibrationStatus ph
             * @property {fk_atlas.EcCalibrations|null} [ec] AtlasCalibrationStatus ec
             * @property {fk_atlas.OrpCalibrations|null} [orp] AtlasCalibrationStatus orp
             * @property {Uint8Array|null} [configuration] AtlasCalibrationStatus configuration
             */
    
            /**
             * Constructs a new AtlasCalibrationStatus.
             * @memberof fk_atlas
             * @classdesc Represents an AtlasCalibrationStatus.
             * @implements IAtlasCalibrationStatus
             * @constructor
             * @param {fk_atlas.IAtlasCalibrationStatus=} [properties] Properties to set
             */
            function AtlasCalibrationStatus(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * AtlasCalibrationStatus type.
             * @member {fk_atlas.SensorType} type
             * @memberof fk_atlas.AtlasCalibrationStatus
             * @instance
             */
            AtlasCalibrationStatus.prototype.type = 0;
    
            /**
             * AtlasCalibrationStatus time.
             * @member {number} time
             * @memberof fk_atlas.AtlasCalibrationStatus
             * @instance
             */
            AtlasCalibrationStatus.prototype.time = 0;
    
            /**
             * AtlasCalibrationStatus raw.
             * @member {number} raw
             * @memberof fk_atlas.AtlasCalibrationStatus
             * @instance
             */
            AtlasCalibrationStatus.prototype.raw = 0;
    
            /**
             * AtlasCalibrationStatus temp.
             * @member {fk_atlas.TempCalibrations} temp
             * @memberof fk_atlas.AtlasCalibrationStatus
             * @instance
             */
            AtlasCalibrationStatus.prototype.temp = 0;
    
            /**
             * AtlasCalibrationStatus dissolvedOxygen.
             * @member {fk_atlas.DoCalibrations} dissolvedOxygen
             * @memberof fk_atlas.AtlasCalibrationStatus
             * @instance
             */
            AtlasCalibrationStatus.prototype.dissolvedOxygen = 0;
    
            /**
             * AtlasCalibrationStatus ph.
             * @member {fk_atlas.PhCalibrations} ph
             * @memberof fk_atlas.AtlasCalibrationStatus
             * @instance
             */
            AtlasCalibrationStatus.prototype.ph = 0;
    
            /**
             * AtlasCalibrationStatus ec.
             * @member {fk_atlas.EcCalibrations} ec
             * @memberof fk_atlas.AtlasCalibrationStatus
             * @instance
             */
            AtlasCalibrationStatus.prototype.ec = 0;
    
            /**
             * AtlasCalibrationStatus orp.
             * @member {fk_atlas.OrpCalibrations} orp
             * @memberof fk_atlas.AtlasCalibrationStatus
             * @instance
             */
            AtlasCalibrationStatus.prototype.orp = 0;
    
            /**
             * AtlasCalibrationStatus configuration.
             * @member {Uint8Array} configuration
             * @memberof fk_atlas.AtlasCalibrationStatus
             * @instance
             */
            AtlasCalibrationStatus.prototype.configuration = $util.newBuffer([]);
    
            /**
             * Creates a new AtlasCalibrationStatus instance using the specified properties.
             * @function create
             * @memberof fk_atlas.AtlasCalibrationStatus
             * @static
             * @param {fk_atlas.IAtlasCalibrationStatus=} [properties] Properties to set
             * @returns {fk_atlas.AtlasCalibrationStatus} AtlasCalibrationStatus instance
             */
            AtlasCalibrationStatus.create = function create(properties) {
                return new AtlasCalibrationStatus(properties);
            };
    
            /**
             * Encodes the specified AtlasCalibrationStatus message. Does not implicitly {@link fk_atlas.AtlasCalibrationStatus.verify|verify} messages.
             * @function encode
             * @memberof fk_atlas.AtlasCalibrationStatus
             * @static
             * @param {fk_atlas.IAtlasCalibrationStatus} message AtlasCalibrationStatus message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            AtlasCalibrationStatus.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.type != null && message.hasOwnProperty("type"))
                    writer.uint32(/* id 1, wireType 0 =*/8).int32(message.type);
                if (message.time != null && message.hasOwnProperty("time"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint32(message.time);
                if (message.temp != null && message.hasOwnProperty("temp"))
                    writer.uint32(/* id 3, wireType 0 =*/24).int32(message.temp);
                if (message.dissolvedOxygen != null && message.hasOwnProperty("dissolvedOxygen"))
                    writer.uint32(/* id 4, wireType 0 =*/32).int32(message.dissolvedOxygen);
                if (message.ph != null && message.hasOwnProperty("ph"))
                    writer.uint32(/* id 5, wireType 0 =*/40).int32(message.ph);
                if (message.ec != null && message.hasOwnProperty("ec"))
                    writer.uint32(/* id 6, wireType 0 =*/48).int32(message.ec);
                if (message.orp != null && message.hasOwnProperty("orp"))
                    writer.uint32(/* id 7, wireType 0 =*/56).int32(message.orp);
                if (message.raw != null && message.hasOwnProperty("raw"))
                    writer.uint32(/* id 8, wireType 0 =*/64).uint32(message.raw);
                if (message.configuration != null && message.hasOwnProperty("configuration"))
                    writer.uint32(/* id 9, wireType 2 =*/74).bytes(message.configuration);
                return writer;
            };
    
            /**
             * Encodes the specified AtlasCalibrationStatus message, length delimited. Does not implicitly {@link fk_atlas.AtlasCalibrationStatus.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_atlas.AtlasCalibrationStatus
             * @static
             * @param {fk_atlas.IAtlasCalibrationStatus} message AtlasCalibrationStatus message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            AtlasCalibrationStatus.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes an AtlasCalibrationStatus message from the specified reader or buffer.
             * @function decode
             * @memberof fk_atlas.AtlasCalibrationStatus
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_atlas.AtlasCalibrationStatus} AtlasCalibrationStatus
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            AtlasCalibrationStatus.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_atlas.AtlasCalibrationStatus();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.type = reader.int32();
                        break;
                    case 2:
                        message.time = reader.uint32();
                        break;
                    case 8:
                        message.raw = reader.uint32();
                        break;
                    case 3:
                        message.temp = reader.int32();
                        break;
                    case 4:
                        message.dissolvedOxygen = reader.int32();
                        break;
                    case 5:
                        message.ph = reader.int32();
                        break;
                    case 6:
                        message.ec = reader.int32();
                        break;
                    case 7:
                        message.orp = reader.int32();
                        break;
                    case 9:
                        message.configuration = reader.bytes();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes an AtlasCalibrationStatus message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_atlas.AtlasCalibrationStatus
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_atlas.AtlasCalibrationStatus} AtlasCalibrationStatus
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            AtlasCalibrationStatus.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies an AtlasCalibrationStatus message.
             * @function verify
             * @memberof fk_atlas.AtlasCalibrationStatus
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            AtlasCalibrationStatus.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.type != null && message.hasOwnProperty("type"))
                    switch (message.type) {
                    default:
                        return "type: enum value expected";
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                        break;
                    }
                if (message.time != null && message.hasOwnProperty("time"))
                    if (!$util.isInteger(message.time))
                        return "time: integer expected";
                if (message.raw != null && message.hasOwnProperty("raw"))
                    if (!$util.isInteger(message.raw))
                        return "raw: integer expected";
                if (message.temp != null && message.hasOwnProperty("temp"))
                    switch (message.temp) {
                    default:
                        return "temp: enum value expected";
                    case 0:
                    case 1:
                        break;
                    }
                if (message.dissolvedOxygen != null && message.hasOwnProperty("dissolvedOxygen"))
                    switch (message.dissolvedOxygen) {
                    default:
                        return "dissolvedOxygen: enum value expected";
                    case 0:
                    case 1:
                    case 2:
                        break;
                    }
                if (message.ph != null && message.hasOwnProperty("ph"))
                    switch (message.ph) {
                    default:
                        return "ph: enum value expected";
                    case 0:
                    case 1:
                    case 2:
                    case 4:
                        break;
                    }
                if (message.ec != null && message.hasOwnProperty("ec"))
                    switch (message.ec) {
                    default:
                        return "ec: enum value expected";
                    case 0:
                    case 1:
                    case 2:
                    case 4:
                    case 8:
                        break;
                    }
                if (message.orp != null && message.hasOwnProperty("orp"))
                    switch (message.orp) {
                    default:
                        return "orp: enum value expected";
                    case 0:
                    case 1:
                        break;
                    }
                if (message.configuration != null && message.hasOwnProperty("configuration"))
                    if (!(message.configuration && typeof message.configuration.length === "number" || $util.isString(message.configuration)))
                        return "configuration: buffer expected";
                return null;
            };
    
            /**
             * Creates an AtlasCalibrationStatus message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_atlas.AtlasCalibrationStatus
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_atlas.AtlasCalibrationStatus} AtlasCalibrationStatus
             */
            AtlasCalibrationStatus.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_atlas.AtlasCalibrationStatus)
                    return object;
                var message = new $root.fk_atlas.AtlasCalibrationStatus();
                switch (object.type) {
                case "SENSOR_NONE":
                case 0:
                    message.type = 0;
                    break;
                case "SENSOR_PH":
                case 1:
                    message.type = 1;
                    break;
                case "SENSOR_TEMP":
                case 2:
                    message.type = 2;
                    break;
                case "SENSOR_ORP":
                case 3:
                    message.type = 3;
                    break;
                case "SENSOR_DO":
                case 4:
                    message.type = 4;
                    break;
                case "SENSOR_EC":
                case 5:
                    message.type = 5;
                    break;
                }
                if (object.time != null)
                    message.time = object.time >>> 0;
                if (object.raw != null)
                    message.raw = object.raw >>> 0;
                switch (object.temp) {
                case "TEMP_NONE":
                case 0:
                    message.temp = 0;
                    break;
                case "TEMP_SINGLE":
                case 1:
                    message.temp = 1;
                    break;
                }
                switch (object.dissolvedOxygen) {
                case "DO_NONE":
                case 0:
                    message.dissolvedOxygen = 0;
                    break;
                case "DO_ATMOSPHERE":
                case 1:
                    message.dissolvedOxygen = 1;
                    break;
                case "DO_ZERO":
                case 2:
                    message.dissolvedOxygen = 2;
                    break;
                }
                switch (object.ph) {
                case "PH_NONE":
                case 0:
                    message.ph = 0;
                    break;
                case "PH_LOW":
                case 1:
                    message.ph = 1;
                    break;
                case "PH_MIDDLE":
                case 2:
                    message.ph = 2;
                    break;
                case "PH_HIGH":
                case 4:
                    message.ph = 4;
                    break;
                }
                switch (object.ec) {
                case "EC_NONE":
                case 0:
                    message.ec = 0;
                    break;
                case "EC_DRY":
                case 1:
                    message.ec = 1;
                    break;
                case "EC_SINGLE":
                case 2:
                    message.ec = 2;
                    break;
                case "EC_DUAL_LOW":
                case 4:
                    message.ec = 4;
                    break;
                case "EC_DUAL_HIGH":
                case 8:
                    message.ec = 8;
                    break;
                }
                switch (object.orp) {
                case "ORP_NONE":
                case 0:
                    message.orp = 0;
                    break;
                case "ORP_SINGLE":
                case 1:
                    message.orp = 1;
                    break;
                }
                if (object.configuration != null)
                    if (typeof object.configuration === "string")
                        $util.base64.decode(object.configuration, message.configuration = $util.newBuffer($util.base64.length(object.configuration)), 0);
                    else if (object.configuration.length)
                        message.configuration = object.configuration;
                return message;
            };
    
            /**
             * Creates a plain object from an AtlasCalibrationStatus message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_atlas.AtlasCalibrationStatus
             * @static
             * @param {fk_atlas.AtlasCalibrationStatus} message AtlasCalibrationStatus
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            AtlasCalibrationStatus.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.type = options.enums === String ? "SENSOR_NONE" : 0;
                    object.time = 0;
                    object.temp = options.enums === String ? "TEMP_NONE" : 0;
                    object.dissolvedOxygen = options.enums === String ? "DO_NONE" : 0;
                    object.ph = options.enums === String ? "PH_NONE" : 0;
                    object.ec = options.enums === String ? "EC_NONE" : 0;
                    object.orp = options.enums === String ? "ORP_NONE" : 0;
                    object.raw = 0;
                    if (options.bytes === String)
                        object.configuration = "";
                    else {
                        object.configuration = [];
                        if (options.bytes !== Array)
                            object.configuration = $util.newBuffer(object.configuration);
                    }
                }
                if (message.type != null && message.hasOwnProperty("type"))
                    object.type = options.enums === String ? $root.fk_atlas.SensorType[message.type] : message.type;
                if (message.time != null && message.hasOwnProperty("time"))
                    object.time = message.time;
                if (message.temp != null && message.hasOwnProperty("temp"))
                    object.temp = options.enums === String ? $root.fk_atlas.TempCalibrations[message.temp] : message.temp;
                if (message.dissolvedOxygen != null && message.hasOwnProperty("dissolvedOxygen"))
                    object.dissolvedOxygen = options.enums === String ? $root.fk_atlas.DoCalibrations[message.dissolvedOxygen] : message.dissolvedOxygen;
                if (message.ph != null && message.hasOwnProperty("ph"))
                    object.ph = options.enums === String ? $root.fk_atlas.PhCalibrations[message.ph] : message.ph;
                if (message.ec != null && message.hasOwnProperty("ec"))
                    object.ec = options.enums === String ? $root.fk_atlas.EcCalibrations[message.ec] : message.ec;
                if (message.orp != null && message.hasOwnProperty("orp"))
                    object.orp = options.enums === String ? $root.fk_atlas.OrpCalibrations[message.orp] : message.orp;
                if (message.raw != null && message.hasOwnProperty("raw"))
                    object.raw = message.raw;
                if (message.configuration != null && message.hasOwnProperty("configuration"))
                    object.configuration = options.bytes === String ? $util.base64.encode(message.configuration, 0, message.configuration.length) : options.bytes === Array ? Array.prototype.slice.call(message.configuration) : message.configuration;
                return object;
            };
    
            /**
             * Converts this AtlasCalibrationStatus to JSON.
             * @function toJSON
             * @memberof fk_atlas.AtlasCalibrationStatus
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            AtlasCalibrationStatus.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return AtlasCalibrationStatus;
        })();
    
        fk_atlas.TwoWireReply = (function() {
    
            /**
             * Properties of a TwoWireReply.
             * @memberof fk_atlas
             * @interface ITwoWireReply
             * @property {Uint8Array|null} [data] TwoWireReply data
             */
    
            /**
             * Constructs a new TwoWireReply.
             * @memberof fk_atlas
             * @classdesc Represents a TwoWireReply.
             * @implements ITwoWireReply
             * @constructor
             * @param {fk_atlas.ITwoWireReply=} [properties] Properties to set
             */
            function TwoWireReply(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * TwoWireReply data.
             * @member {Uint8Array} data
             * @memberof fk_atlas.TwoWireReply
             * @instance
             */
            TwoWireReply.prototype.data = $util.newBuffer([]);
    
            /**
             * Creates a new TwoWireReply instance using the specified properties.
             * @function create
             * @memberof fk_atlas.TwoWireReply
             * @static
             * @param {fk_atlas.ITwoWireReply=} [properties] Properties to set
             * @returns {fk_atlas.TwoWireReply} TwoWireReply instance
             */
            TwoWireReply.create = function create(properties) {
                return new TwoWireReply(properties);
            };
    
            /**
             * Encodes the specified TwoWireReply message. Does not implicitly {@link fk_atlas.TwoWireReply.verify|verify} messages.
             * @function encode
             * @memberof fk_atlas.TwoWireReply
             * @static
             * @param {fk_atlas.ITwoWireReply} message TwoWireReply message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            TwoWireReply.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.data != null && message.hasOwnProperty("data"))
                    writer.uint32(/* id 1, wireType 2 =*/10).bytes(message.data);
                return writer;
            };
    
            /**
             * Encodes the specified TwoWireReply message, length delimited. Does not implicitly {@link fk_atlas.TwoWireReply.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_atlas.TwoWireReply
             * @static
             * @param {fk_atlas.ITwoWireReply} message TwoWireReply message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            TwoWireReply.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a TwoWireReply message from the specified reader or buffer.
             * @function decode
             * @memberof fk_atlas.TwoWireReply
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_atlas.TwoWireReply} TwoWireReply
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            TwoWireReply.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_atlas.TwoWireReply();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.data = reader.bytes();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a TwoWireReply message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_atlas.TwoWireReply
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_atlas.TwoWireReply} TwoWireReply
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            TwoWireReply.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a TwoWireReply message.
             * @function verify
             * @memberof fk_atlas.TwoWireReply
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            TwoWireReply.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.data != null && message.hasOwnProperty("data"))
                    if (!(message.data && typeof message.data.length === "number" || $util.isString(message.data)))
                        return "data: buffer expected";
                return null;
            };
    
            /**
             * Creates a TwoWireReply message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_atlas.TwoWireReply
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_atlas.TwoWireReply} TwoWireReply
             */
            TwoWireReply.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_atlas.TwoWireReply)
                    return object;
                var message = new $root.fk_atlas.TwoWireReply();
                if (object.data != null)
                    if (typeof object.data === "string")
                        $util.base64.decode(object.data, message.data = $util.newBuffer($util.base64.length(object.data)), 0);
                    else if (object.data.length)
                        message.data = object.data;
                return message;
            };
    
            /**
             * Creates a plain object from a TwoWireReply message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_atlas.TwoWireReply
             * @static
             * @param {fk_atlas.TwoWireReply} message TwoWireReply
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            TwoWireReply.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults)
                    if (options.bytes === String)
                        object.data = "";
                    else {
                        object.data = [];
                        if (options.bytes !== Array)
                            object.data = $util.newBuffer(object.data);
                    }
                if (message.data != null && message.hasOwnProperty("data"))
                    object.data = options.bytes === String ? $util.base64.encode(message.data, 0, message.data.length) : options.bytes === Array ? Array.prototype.slice.call(message.data) : message.data;
                return object;
            };
    
            /**
             * Converts this TwoWireReply to JSON.
             * @function toJSON
             * @memberof fk_atlas.TwoWireReply
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            TwoWireReply.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return TwoWireReply;
        })();
    
        /**
         * ReplyType enum.
         * @name fk_atlas.ReplyType
         * @enum {string}
         * @property {number} REPLY_NONE=0 REPLY_NONE value
         * @property {number} REPLY_RETRY=1 REPLY_RETRY value
         * @property {number} REPLY_ERROR=2 REPLY_ERROR value
         * @property {number} REPLY_STATUS=3 REPLY_STATUS value
         * @property {number} REPLY_COMMAND=4 REPLY_COMMAND value
         */
        fk_atlas.ReplyType = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "REPLY_NONE"] = 0;
            values[valuesById[1] = "REPLY_RETRY"] = 1;
            values[valuesById[2] = "REPLY_ERROR"] = 2;
            values[valuesById[3] = "REPLY_STATUS"] = 3;
            values[valuesById[4] = "REPLY_COMMAND"] = 4;
            return values;
        })();
    
        /**
         * ErrorType enum.
         * @name fk_atlas.ErrorType
         * @enum {string}
         * @property {number} NONE=0 NONE value
         * @property {number} GENERAL=1 GENERAL value
         * @property {number} UNEXPECTED=2 UNEXPECTED value
         */
        fk_atlas.ErrorType = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "NONE"] = 0;
            values[valuesById[1] = "GENERAL"] = 1;
            values[valuesById[2] = "UNEXPECTED"] = 2;
            return values;
        })();
    
        fk_atlas.Error = (function() {
    
            /**
             * Properties of an Error.
             * @memberof fk_atlas
             * @interface IError
             * @property {fk_atlas.ErrorType|null} [type] Error type
             * @property {string|null} [message] Error message
             */
    
            /**
             * Constructs a new Error.
             * @memberof fk_atlas
             * @classdesc Represents an Error.
             * @implements IError
             * @constructor
             * @param {fk_atlas.IError=} [properties] Properties to set
             */
            function Error(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Error type.
             * @member {fk_atlas.ErrorType} type
             * @memberof fk_atlas.Error
             * @instance
             */
            Error.prototype.type = 0;
    
            /**
             * Error message.
             * @member {string} message
             * @memberof fk_atlas.Error
             * @instance
             */
            Error.prototype.message = "";
    
            /**
             * Creates a new Error instance using the specified properties.
             * @function create
             * @memberof fk_atlas.Error
             * @static
             * @param {fk_atlas.IError=} [properties] Properties to set
             * @returns {fk_atlas.Error} Error instance
             */
            Error.create = function create(properties) {
                return new Error(properties);
            };
    
            /**
             * Encodes the specified Error message. Does not implicitly {@link fk_atlas.Error.verify|verify} messages.
             * @function encode
             * @memberof fk_atlas.Error
             * @static
             * @param {fk_atlas.IError} message Error message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Error.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.type != null && message.hasOwnProperty("type"))
                    writer.uint32(/* id 1, wireType 0 =*/8).int32(message.type);
                if (message.message != null && message.hasOwnProperty("message"))
                    writer.uint32(/* id 2, wireType 2 =*/18).string(message.message);
                return writer;
            };
    
            /**
             * Encodes the specified Error message, length delimited. Does not implicitly {@link fk_atlas.Error.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_atlas.Error
             * @static
             * @param {fk_atlas.IError} message Error message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Error.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes an Error message from the specified reader or buffer.
             * @function decode
             * @memberof fk_atlas.Error
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_atlas.Error} Error
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Error.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_atlas.Error();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.type = reader.int32();
                        break;
                    case 2:
                        message.message = reader.string();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes an Error message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_atlas.Error
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_atlas.Error} Error
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Error.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies an Error message.
             * @function verify
             * @memberof fk_atlas.Error
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            Error.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.type != null && message.hasOwnProperty("type"))
                    switch (message.type) {
                    default:
                        return "type: enum value expected";
                    case 0:
                    case 1:
                    case 2:
                        break;
                    }
                if (message.message != null && message.hasOwnProperty("message"))
                    if (!$util.isString(message.message))
                        return "message: string expected";
                return null;
            };
    
            /**
             * Creates an Error message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_atlas.Error
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_atlas.Error} Error
             */
            Error.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_atlas.Error)
                    return object;
                var message = new $root.fk_atlas.Error();
                switch (object.type) {
                case "NONE":
                case 0:
                    message.type = 0;
                    break;
                case "GENERAL":
                case 1:
                    message.type = 1;
                    break;
                case "UNEXPECTED":
                case 2:
                    message.type = 2;
                    break;
                }
                if (object.message != null)
                    message.message = String(object.message);
                return message;
            };
    
            /**
             * Creates a plain object from an Error message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_atlas.Error
             * @static
             * @param {fk_atlas.Error} message Error
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            Error.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.type = options.enums === String ? "NONE" : 0;
                    object.message = "";
                }
                if (message.type != null && message.hasOwnProperty("type"))
                    object.type = options.enums === String ? $root.fk_atlas.ErrorType[message.type] : message.type;
                if (message.message != null && message.hasOwnProperty("message"))
                    object.message = message.message;
                return object;
            };
    
            /**
             * Converts this Error to JSON.
             * @function toJSON
             * @memberof fk_atlas.Error
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Error.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Error;
        })();
    
        fk_atlas.WireAtlasReply = (function() {
    
            /**
             * Properties of a WireAtlasReply.
             * @memberof fk_atlas
             * @interface IWireAtlasReply
             * @property {fk_atlas.ReplyType|null} [type] WireAtlasReply type
             * @property {Array.<fk_atlas.IError>|null} [errors] WireAtlasReply errors
             * @property {fk_atlas.IAtlasCalibrationStatus|null} [calibration] WireAtlasReply calibration
             * @property {fk_atlas.ITwoWireReply|null} [wire] WireAtlasReply wire
             */
    
            /**
             * Constructs a new WireAtlasReply.
             * @memberof fk_atlas
             * @classdesc Represents a WireAtlasReply.
             * @implements IWireAtlasReply
             * @constructor
             * @param {fk_atlas.IWireAtlasReply=} [properties] Properties to set
             */
            function WireAtlasReply(properties) {
                this.errors = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * WireAtlasReply type.
             * @member {fk_atlas.ReplyType} type
             * @memberof fk_atlas.WireAtlasReply
             * @instance
             */
            WireAtlasReply.prototype.type = 0;
    
            /**
             * WireAtlasReply errors.
             * @member {Array.<fk_atlas.IError>} errors
             * @memberof fk_atlas.WireAtlasReply
             * @instance
             */
            WireAtlasReply.prototype.errors = $util.emptyArray;
    
            /**
             * WireAtlasReply calibration.
             * @member {fk_atlas.IAtlasCalibrationStatus|null|undefined} calibration
             * @memberof fk_atlas.WireAtlasReply
             * @instance
             */
            WireAtlasReply.prototype.calibration = null;
    
            /**
             * WireAtlasReply wire.
             * @member {fk_atlas.ITwoWireReply|null|undefined} wire
             * @memberof fk_atlas.WireAtlasReply
             * @instance
             */
            WireAtlasReply.prototype.wire = null;
    
            /**
             * Creates a new WireAtlasReply instance using the specified properties.
             * @function create
             * @memberof fk_atlas.WireAtlasReply
             * @static
             * @param {fk_atlas.IWireAtlasReply=} [properties] Properties to set
             * @returns {fk_atlas.WireAtlasReply} WireAtlasReply instance
             */
            WireAtlasReply.create = function create(properties) {
                return new WireAtlasReply(properties);
            };
    
            /**
             * Encodes the specified WireAtlasReply message. Does not implicitly {@link fk_atlas.WireAtlasReply.verify|verify} messages.
             * @function encode
             * @memberof fk_atlas.WireAtlasReply
             * @static
             * @param {fk_atlas.IWireAtlasReply} message WireAtlasReply message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            WireAtlasReply.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.type != null && message.hasOwnProperty("type"))
                    writer.uint32(/* id 1, wireType 0 =*/8).int32(message.type);
                if (message.errors != null && message.errors.length)
                    for (var i = 0; i < message.errors.length; ++i)
                        $root.fk_atlas.Error.encode(message.errors[i], writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
                if (message.calibration != null && message.hasOwnProperty("calibration"))
                    $root.fk_atlas.AtlasCalibrationStatus.encode(message.calibration, writer.uint32(/* id 3, wireType 2 =*/26).fork()).ldelim();
                if (message.wire != null && message.hasOwnProperty("wire"))
                    $root.fk_atlas.TwoWireReply.encode(message.wire, writer.uint32(/* id 4, wireType 2 =*/34).fork()).ldelim();
                return writer;
            };
    
            /**
             * Encodes the specified WireAtlasReply message, length delimited. Does not implicitly {@link fk_atlas.WireAtlasReply.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_atlas.WireAtlasReply
             * @static
             * @param {fk_atlas.IWireAtlasReply} message WireAtlasReply message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            WireAtlasReply.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a WireAtlasReply message from the specified reader or buffer.
             * @function decode
             * @memberof fk_atlas.WireAtlasReply
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_atlas.WireAtlasReply} WireAtlasReply
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            WireAtlasReply.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_atlas.WireAtlasReply();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.type = reader.int32();
                        break;
                    case 2:
                        if (!(message.errors && message.errors.length))
                            message.errors = [];
                        message.errors.push($root.fk_atlas.Error.decode(reader, reader.uint32()));
                        break;
                    case 3:
                        message.calibration = $root.fk_atlas.AtlasCalibrationStatus.decode(reader, reader.uint32());
                        break;
                    case 4:
                        message.wire = $root.fk_atlas.TwoWireReply.decode(reader, reader.uint32());
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a WireAtlasReply message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_atlas.WireAtlasReply
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_atlas.WireAtlasReply} WireAtlasReply
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            WireAtlasReply.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a WireAtlasReply message.
             * @function verify
             * @memberof fk_atlas.WireAtlasReply
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            WireAtlasReply.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.type != null && message.hasOwnProperty("type"))
                    switch (message.type) {
                    default:
                        return "type: enum value expected";
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                        break;
                    }
                if (message.errors != null && message.hasOwnProperty("errors")) {
                    if (!Array.isArray(message.errors))
                        return "errors: array expected";
                    for (var i = 0; i < message.errors.length; ++i) {
                        var error = $root.fk_atlas.Error.verify(message.errors[i]);
                        if (error)
                            return "errors." + error;
                    }
                }
                if (message.calibration != null && message.hasOwnProperty("calibration")) {
                    var error = $root.fk_atlas.AtlasCalibrationStatus.verify(message.calibration);
                    if (error)
                        return "calibration." + error;
                }
                if (message.wire != null && message.hasOwnProperty("wire")) {
                    var error = $root.fk_atlas.TwoWireReply.verify(message.wire);
                    if (error)
                        return "wire." + error;
                }
                return null;
            };
    
            /**
             * Creates a WireAtlasReply message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_atlas.WireAtlasReply
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_atlas.WireAtlasReply} WireAtlasReply
             */
            WireAtlasReply.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_atlas.WireAtlasReply)
                    return object;
                var message = new $root.fk_atlas.WireAtlasReply();
                switch (object.type) {
                case "REPLY_NONE":
                case 0:
                    message.type = 0;
                    break;
                case "REPLY_RETRY":
                case 1:
                    message.type = 1;
                    break;
                case "REPLY_ERROR":
                case 2:
                    message.type = 2;
                    break;
                case "REPLY_STATUS":
                case 3:
                    message.type = 3;
                    break;
                case "REPLY_COMMAND":
                case 4:
                    message.type = 4;
                    break;
                }
                if (object.errors) {
                    if (!Array.isArray(object.errors))
                        throw TypeError(".fk_atlas.WireAtlasReply.errors: array expected");
                    message.errors = [];
                    for (var i = 0; i < object.errors.length; ++i) {
                        if (typeof object.errors[i] !== "object")
                            throw TypeError(".fk_atlas.WireAtlasReply.errors: object expected");
                        message.errors[i] = $root.fk_atlas.Error.fromObject(object.errors[i]);
                    }
                }
                if (object.calibration != null) {
                    if (typeof object.calibration !== "object")
                        throw TypeError(".fk_atlas.WireAtlasReply.calibration: object expected");
                    message.calibration = $root.fk_atlas.AtlasCalibrationStatus.fromObject(object.calibration);
                }
                if (object.wire != null) {
                    if (typeof object.wire !== "object")
                        throw TypeError(".fk_atlas.WireAtlasReply.wire: object expected");
                    message.wire = $root.fk_atlas.TwoWireReply.fromObject(object.wire);
                }
                return message;
            };
    
            /**
             * Creates a plain object from a WireAtlasReply message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_atlas.WireAtlasReply
             * @static
             * @param {fk_atlas.WireAtlasReply} message WireAtlasReply
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            WireAtlasReply.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults)
                    object.errors = [];
                if (options.defaults) {
                    object.type = options.enums === String ? "REPLY_NONE" : 0;
                    object.calibration = null;
                    object.wire = null;
                }
                if (message.type != null && message.hasOwnProperty("type"))
                    object.type = options.enums === String ? $root.fk_atlas.ReplyType[message.type] : message.type;
                if (message.errors && message.errors.length) {
                    object.errors = [];
                    for (var j = 0; j < message.errors.length; ++j)
                        object.errors[j] = $root.fk_atlas.Error.toObject(message.errors[j], options);
                }
                if (message.calibration != null && message.hasOwnProperty("calibration"))
                    object.calibration = $root.fk_atlas.AtlasCalibrationStatus.toObject(message.calibration, options);
                if (message.wire != null && message.hasOwnProperty("wire"))
                    object.wire = $root.fk_atlas.TwoWireReply.toObject(message.wire, options);
                return object;
            };
    
            /**
             * Converts this WireAtlasReply to JSON.
             * @function toJSON
             * @memberof fk_atlas.WireAtlasReply
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            WireAtlasReply.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return WireAtlasReply;
        })();
    
        return fk_atlas;
    })();

    return $root;
});
