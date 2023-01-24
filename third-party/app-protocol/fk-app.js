/*eslint-disable block-scoped-var, no-redeclare, no-control-regex, no-prototype-builtins*/
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
    
    $root.fk_app = (function() {
    
        /**
         * Namespace fk_app.
         * @exports fk_app
         * @namespace
         */
        var fk_app = {};
    
        /**
         * QueryFlags enum.
         * @enum {string}
         * @property {number} QUERY_FLAGS_NONE=0 QUERY_FLAGS_NONE value
         * @property {number} QUERY_FLAGS_LOGS=1 QUERY_FLAGS_LOGS value
         */
        fk_app.QueryFlags = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "QUERY_FLAGS_NONE"] = 0;
            values[valuesById[1] = "QUERY_FLAGS_LOGS"] = 1;
            return values;
        })();
    
        /**
         * QueryType enum.
         * @enum {string}
         * @property {number} QUERY_NONE=0 QUERY_NONE value
         * @property {number} QUERY_CAPABILITIES=1 QUERY_CAPABILITIES value
         * @property {number} QUERY_CONFIGURE_SENSOR=2 QUERY_CONFIGURE_SENSOR value
         * @property {number} QUERY_LIVE_DATA_POLL=7 QUERY_LIVE_DATA_POLL value
         * @property {number} QUERY_SCHEDULES=8 QUERY_SCHEDULES value
         * @property {number} QUERY_CONFIGURE_SCHEDULES=9 QUERY_CONFIGURE_SCHEDULES value
         * @property {number} QUERY_FILES_SD=10 QUERY_FILES_SD value
         * @property {number} QUERY_DOWNLOAD_FILE=11 QUERY_DOWNLOAD_FILE value
         * @property {number} QUERY_ERASE_FILE=12 QUERY_ERASE_FILE value
         * @property {number} QUERY_RESET=13 QUERY_RESET value
         * @property {number} QUERY_NETWORK_SETTINGS=14 QUERY_NETWORK_SETTINGS value
         * @property {number} QUERY_CONFIGURE_NETWORK_SETTINGS=15 QUERY_CONFIGURE_NETWORK_SETTINGS value
         * @property {number} QUERY_SCAN_MODULES=16 QUERY_SCAN_MODULES value
         * @property {number} QUERY_CONFIGURE_IDENTITY=17 QUERY_CONFIGURE_IDENTITY value
         * @property {number} QUERY_STATUS=18 QUERY_STATUS value
         * @property {number} QUERY_MODULE=19 QUERY_MODULE value
         * @property {number} QUERY_METADATA=20 QUERY_METADATA value
         * @property {number} QUERY_FORMAT=21 QUERY_FORMAT value
         * @property {number} QUERY_GET_READINGS=22 QUERY_GET_READINGS value
         * @property {number} QUERY_TAKE_READINGS=23 QUERY_TAKE_READINGS value
         * @property {number} QUERY_RECORDING_CONTROL=24 QUERY_RECORDING_CONTROL value
         * @property {number} QUERY_CONFIGURE=25 QUERY_CONFIGURE value
         * @property {number} QUERY_SCAN_NETWORKS=26 QUERY_SCAN_NETWORKS value
         * @property {number} QUERY_FILES_SPI=27 QUERY_FILES_SPI value
         * @property {number} QUERY_FILES_QSPI=28 QUERY_FILES_QSPI value
         */
        fk_app.QueryType = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "QUERY_NONE"] = 0;
            values[valuesById[1] = "QUERY_CAPABILITIES"] = 1;
            values[valuesById[2] = "QUERY_CONFIGURE_SENSOR"] = 2;
            values[valuesById[7] = "QUERY_LIVE_DATA_POLL"] = 7;
            values[valuesById[8] = "QUERY_SCHEDULES"] = 8;
            values[valuesById[9] = "QUERY_CONFIGURE_SCHEDULES"] = 9;
            values[valuesById[10] = "QUERY_FILES_SD"] = 10;
            values[valuesById[11] = "QUERY_DOWNLOAD_FILE"] = 11;
            values[valuesById[12] = "QUERY_ERASE_FILE"] = 12;
            values[valuesById[13] = "QUERY_RESET"] = 13;
            values[valuesById[14] = "QUERY_NETWORK_SETTINGS"] = 14;
            values[valuesById[15] = "QUERY_CONFIGURE_NETWORK_SETTINGS"] = 15;
            values[valuesById[16] = "QUERY_SCAN_MODULES"] = 16;
            values[valuesById[17] = "QUERY_CONFIGURE_IDENTITY"] = 17;
            values[valuesById[18] = "QUERY_STATUS"] = 18;
            values[valuesById[19] = "QUERY_MODULE"] = 19;
            values[valuesById[20] = "QUERY_METADATA"] = 20;
            values[valuesById[21] = "QUERY_FORMAT"] = 21;
            values[valuesById[22] = "QUERY_GET_READINGS"] = 22;
            values[valuesById[23] = "QUERY_TAKE_READINGS"] = 23;
            values[valuesById[24] = "QUERY_RECORDING_CONTROL"] = 24;
            values[valuesById[25] = "QUERY_CONFIGURE"] = 25;
            values[valuesById[26] = "QUERY_SCAN_NETWORKS"] = 26;
            values[valuesById[27] = "QUERY_FILES_SPI"] = 27;
            values[valuesById[28] = "QUERY_FILES_QSPI"] = 28;
            return values;
        })();
    
        /**
         * ReplyType enum.
         * @enum {string}
         * @property {number} REPLY_NONE=0 REPLY_NONE value
         * @property {number} REPLY_SUCCESS=1 REPLY_SUCCESS value
         * @property {number} REPLY_BUSY=2 REPLY_BUSY value
         * @property {number} REPLY_ERROR=3 REPLY_ERROR value
         * @property {number} REPLY_CAPABILITIES=4 REPLY_CAPABILITIES value
         * @property {number} REPLY_LIVE_DATA_POLL=8 REPLY_LIVE_DATA_POLL value
         * @property {number} REPLY_SCHEDULES=9 REPLY_SCHEDULES value
         * @property {number} REPLY_FILES=10 REPLY_FILES value
         * @property {number} REPLY_DOWNLOAD_FILE=11 REPLY_DOWNLOAD_FILE value
         * @property {number} REPLY_RESET=12 REPLY_RESET value
         * @property {number} REPLY_NETWORK_SETTINGS=13 REPLY_NETWORK_SETTINGS value
         * @property {number} REPLY_IDENTITY=14 REPLY_IDENTITY value
         * @property {number} REPLY_STATUS=15 REPLY_STATUS value
         * @property {number} REPLY_MODULE=16 REPLY_MODULE value
         * @property {number} REPLY_METADATA=17 REPLY_METADATA value
         * @property {number} REPLY_READINGS=18 REPLY_READINGS value
         * @property {number} REPLY_NETWORKS=19 REPLY_NETWORKS value
         */
        fk_app.ReplyType = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "REPLY_NONE"] = 0;
            values[valuesById[1] = "REPLY_SUCCESS"] = 1;
            values[valuesById[2] = "REPLY_BUSY"] = 2;
            values[valuesById[3] = "REPLY_ERROR"] = 3;
            values[valuesById[4] = "REPLY_CAPABILITIES"] = 4;
            values[valuesById[8] = "REPLY_LIVE_DATA_POLL"] = 8;
            values[valuesById[9] = "REPLY_SCHEDULES"] = 9;
            values[valuesById[10] = "REPLY_FILES"] = 10;
            values[valuesById[11] = "REPLY_DOWNLOAD_FILE"] = 11;
            values[valuesById[12] = "REPLY_RESET"] = 12;
            values[valuesById[13] = "REPLY_NETWORK_SETTINGS"] = 13;
            values[valuesById[14] = "REPLY_IDENTITY"] = 14;
            values[valuesById[15] = "REPLY_STATUS"] = 15;
            values[valuesById[16] = "REPLY_MODULE"] = 16;
            values[valuesById[17] = "REPLY_METADATA"] = 17;
            values[valuesById[18] = "REPLY_READINGS"] = 18;
            values[valuesById[19] = "REPLY_NETWORKS"] = 19;
            return values;
        })();
    
        /**
         * DownloadFlags enum.
         * @enum {string}
         * @property {number} DOWNLOAD_FLAG_NONE=0 DOWNLOAD_FLAG_NONE value
         * @property {number} DOWNLOAD_FLAG_METADATA_PREPEND=1 DOWNLOAD_FLAG_METADATA_PREPEND value
         * @property {number} DOWNLOAD_FLAG_METADATA_ONLY=2 DOWNLOAD_FLAG_METADATA_ONLY value
         */
        fk_app.DownloadFlags = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "DOWNLOAD_FLAG_NONE"] = 0;
            values[valuesById[1] = "DOWNLOAD_FLAG_METADATA_PREPEND"] = 1;
            values[valuesById[2] = "DOWNLOAD_FLAG_METADATA_ONLY"] = 2;
            return values;
        })();
    
        /**
         * ModuleFlags enum.
         * @enum {string}
         * @property {number} MODULE_FLAG_NONE=0 MODULE_FLAG_NONE value
         * @property {number} MODULE_FLAG_INTERNAL=1 MODULE_FLAG_INTERNAL value
         */
        fk_app.ModuleFlags = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "MODULE_FLAG_NONE"] = 0;
            values[valuesById[1] = "MODULE_FLAG_INTERNAL"] = 1;
            return values;
        })();
    
        /**
         * SensorFlags enum.
         * @enum {string}
         * @property {number} SENSOR_FLAG_NONE=0 SENSOR_FLAG_NONE value
         */
        fk_app.SensorFlags = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "SENSOR_FLAG_NONE"] = 0;
            return values;
        })();
    
        fk_app.QueryCapabilities = (function() {
    
            /**
             * Properties of a QueryCapabilities.
             * @memberof fk_app
             * @interface IQueryCapabilities
             * @property {number} [version] QueryCapabilities version
             * @property {number} [callerTime] QueryCapabilities callerTime
             */
    
            /**
             * Constructs a new QueryCapabilities.
             * @memberof fk_app
             * @classdesc Represents a QueryCapabilities.
             * @constructor
             * @param {fk_app.IQueryCapabilities=} [properties] Properties to set
             */
            function QueryCapabilities(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * QueryCapabilities version.
             * @member {number}version
             * @memberof fk_app.QueryCapabilities
             * @instance
             */
            QueryCapabilities.prototype.version = 0;
    
            /**
             * QueryCapabilities callerTime.
             * @member {number}callerTime
             * @memberof fk_app.QueryCapabilities
             * @instance
             */
            QueryCapabilities.prototype.callerTime = 0;
    
            /**
             * Creates a new QueryCapabilities instance using the specified properties.
             * @function create
             * @memberof fk_app.QueryCapabilities
             * @static
             * @param {fk_app.IQueryCapabilities=} [properties] Properties to set
             * @returns {fk_app.QueryCapabilities} QueryCapabilities instance
             */
            QueryCapabilities.create = function create(properties) {
                return new QueryCapabilities(properties);
            };
    
            /**
             * Encodes the specified QueryCapabilities message. Does not implicitly {@link fk_app.QueryCapabilities.verify|verify} messages.
             * @function encode
             * @memberof fk_app.QueryCapabilities
             * @static
             * @param {fk_app.IQueryCapabilities} message QueryCapabilities message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            QueryCapabilities.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.version != null && message.hasOwnProperty("version"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.version);
                if (message.callerTime != null && message.hasOwnProperty("callerTime"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint32(message.callerTime);
                return writer;
            };
    
            /**
             * Encodes the specified QueryCapabilities message, length delimited. Does not implicitly {@link fk_app.QueryCapabilities.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.QueryCapabilities
             * @static
             * @param {fk_app.IQueryCapabilities} message QueryCapabilities message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            QueryCapabilities.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a QueryCapabilities message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.QueryCapabilities
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.QueryCapabilities} QueryCapabilities
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            QueryCapabilities.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.QueryCapabilities();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.version = reader.uint32();
                        break;
                    case 2:
                        message.callerTime = reader.uint32();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a QueryCapabilities message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.QueryCapabilities
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.QueryCapabilities} QueryCapabilities
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            QueryCapabilities.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a QueryCapabilities message.
             * @function verify
             * @memberof fk_app.QueryCapabilities
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            QueryCapabilities.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.version != null && message.hasOwnProperty("version"))
                    if (!$util.isInteger(message.version))
                        return "version: integer expected";
                if (message.callerTime != null && message.hasOwnProperty("callerTime"))
                    if (!$util.isInteger(message.callerTime))
                        return "callerTime: integer expected";
                return null;
            };
    
            /**
             * Creates a QueryCapabilities message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.QueryCapabilities
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.QueryCapabilities} QueryCapabilities
             */
            QueryCapabilities.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.QueryCapabilities)
                    return object;
                var message = new $root.fk_app.QueryCapabilities();
                if (object.version != null)
                    message.version = object.version >>> 0;
                if (object.callerTime != null)
                    message.callerTime = object.callerTime >>> 0;
                return message;
            };
    
            /**
             * Creates a plain object from a QueryCapabilities message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.QueryCapabilities
             * @static
             * @param {fk_app.QueryCapabilities} message QueryCapabilities
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            QueryCapabilities.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.version = 0;
                    object.callerTime = 0;
                }
                if (message.version != null && message.hasOwnProperty("version"))
                    object.version = message.version;
                if (message.callerTime != null && message.hasOwnProperty("callerTime"))
                    object.callerTime = message.callerTime;
                return object;
            };
    
            /**
             * Converts this QueryCapabilities to JSON.
             * @function toJSON
             * @memberof fk_app.QueryCapabilities
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            QueryCapabilities.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return QueryCapabilities;
        })();
    
        fk_app.LiveValue = (function() {
    
            /**
             * Properties of a LiveValue.
             * @memberof fk_app
             * @interface ILiveValue
             * @property {boolean} [valid] LiveValue valid
             * @property {number} [value] LiveValue value
             * @property {number} [uncalibrated] LiveValue uncalibrated
             */
    
            /**
             * Constructs a new LiveValue.
             * @memberof fk_app
             * @classdesc Represents a LiveValue.
             * @constructor
             * @param {fk_app.ILiveValue=} [properties] Properties to set
             */
            function LiveValue(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * LiveValue valid.
             * @member {boolean}valid
             * @memberof fk_app.LiveValue
             * @instance
             */
            LiveValue.prototype.valid = false;
    
            /**
             * LiveValue value.
             * @member {number}value
             * @memberof fk_app.LiveValue
             * @instance
             */
            LiveValue.prototype.value = 0;
    
            /**
             * LiveValue uncalibrated.
             * @member {number}uncalibrated
             * @memberof fk_app.LiveValue
             * @instance
             */
            LiveValue.prototype.uncalibrated = 0;
    
            /**
             * Creates a new LiveValue instance using the specified properties.
             * @function create
             * @memberof fk_app.LiveValue
             * @static
             * @param {fk_app.ILiveValue=} [properties] Properties to set
             * @returns {fk_app.LiveValue} LiveValue instance
             */
            LiveValue.create = function create(properties) {
                return new LiveValue(properties);
            };
    
            /**
             * Encodes the specified LiveValue message. Does not implicitly {@link fk_app.LiveValue.verify|verify} messages.
             * @function encode
             * @memberof fk_app.LiveValue
             * @static
             * @param {fk_app.ILiveValue} message LiveValue message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            LiveValue.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.valid != null && message.hasOwnProperty("valid"))
                    writer.uint32(/* id 1, wireType 0 =*/8).bool(message.valid);
                if (message.value != null && message.hasOwnProperty("value"))
                    writer.uint32(/* id 2, wireType 5 =*/21).float(message.value);
                if (message.uncalibrated != null && message.hasOwnProperty("uncalibrated"))
                    writer.uint32(/* id 3, wireType 5 =*/29).float(message.uncalibrated);
                return writer;
            };
    
            /**
             * Encodes the specified LiveValue message, length delimited. Does not implicitly {@link fk_app.LiveValue.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.LiveValue
             * @static
             * @param {fk_app.ILiveValue} message LiveValue message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            LiveValue.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a LiveValue message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.LiveValue
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.LiveValue} LiveValue
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            LiveValue.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.LiveValue();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.valid = reader.bool();
                        break;
                    case 2:
                        message.value = reader.float();
                        break;
                    case 3:
                        message.uncalibrated = reader.float();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a LiveValue message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.LiveValue
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.LiveValue} LiveValue
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            LiveValue.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a LiveValue message.
             * @function verify
             * @memberof fk_app.LiveValue
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            LiveValue.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.valid != null && message.hasOwnProperty("valid"))
                    if (typeof message.valid !== "boolean")
                        return "valid: boolean expected";
                if (message.value != null && message.hasOwnProperty("value"))
                    if (typeof message.value !== "number")
                        return "value: number expected";
                if (message.uncalibrated != null && message.hasOwnProperty("uncalibrated"))
                    if (typeof message.uncalibrated !== "number")
                        return "uncalibrated: number expected";
                return null;
            };
    
            /**
             * Creates a LiveValue message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.LiveValue
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.LiveValue} LiveValue
             */
            LiveValue.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.LiveValue)
                    return object;
                var message = new $root.fk_app.LiveValue();
                if (object.valid != null)
                    message.valid = Boolean(object.valid);
                if (object.value != null)
                    message.value = Number(object.value);
                if (object.uncalibrated != null)
                    message.uncalibrated = Number(object.uncalibrated);
                return message;
            };
    
            /**
             * Creates a plain object from a LiveValue message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.LiveValue
             * @static
             * @param {fk_app.LiveValue} message LiveValue
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            LiveValue.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.valid = false;
                    object.value = 0;
                    object.uncalibrated = 0;
                }
                if (message.valid != null && message.hasOwnProperty("valid"))
                    object.valid = message.valid;
                if (message.value != null && message.hasOwnProperty("value"))
                    object.value = options.json && !isFinite(message.value) ? String(message.value) : message.value;
                if (message.uncalibrated != null && message.hasOwnProperty("uncalibrated"))
                    object.uncalibrated = options.json && !isFinite(message.uncalibrated) ? String(message.uncalibrated) : message.uncalibrated;
                return object;
            };
    
            /**
             * Converts this LiveValue to JSON.
             * @function toJSON
             * @memberof fk_app.LiveValue
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            LiveValue.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return LiveValue;
        })();
    
        fk_app.SensorCapabilities = (function() {
    
            /**
             * Properties of a SensorCapabilities.
             * @memberof fk_app
             * @interface ISensorCapabilities
             * @property {number} [number] SensorCapabilities number
             * @property {number} [module] SensorCapabilities module
             * @property {string} [name] SensorCapabilities name
             * @property {number} [frequency] SensorCapabilities frequency
             * @property {string} [unitOfMeasure] SensorCapabilities unitOfMeasure
             * @property {string} [uncalibratedUnitOfMeasure] SensorCapabilities uncalibratedUnitOfMeasure
             * @property {string} [path] SensorCapabilities path
             * @property {number} [flags] SensorCapabilities flags
             * @property {fk_app.ILiveValue} [value] SensorCapabilities value
             */
    
            /**
             * Constructs a new SensorCapabilities.
             * @memberof fk_app
             * @classdesc Represents a SensorCapabilities.
             * @constructor
             * @param {fk_app.ISensorCapabilities=} [properties] Properties to set
             */
            function SensorCapabilities(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * SensorCapabilities number.
             * @member {number}number
             * @memberof fk_app.SensorCapabilities
             * @instance
             */
            SensorCapabilities.prototype.number = 0;
    
            /**
             * SensorCapabilities module.
             * @member {number}module
             * @memberof fk_app.SensorCapabilities
             * @instance
             */
            SensorCapabilities.prototype.module = 0;
    
            /**
             * SensorCapabilities name.
             * @member {string}name
             * @memberof fk_app.SensorCapabilities
             * @instance
             */
            SensorCapabilities.prototype.name = "";
    
            /**
             * SensorCapabilities frequency.
             * @member {number}frequency
             * @memberof fk_app.SensorCapabilities
             * @instance
             */
            SensorCapabilities.prototype.frequency = 0;
    
            /**
             * SensorCapabilities unitOfMeasure.
             * @member {string}unitOfMeasure
             * @memberof fk_app.SensorCapabilities
             * @instance
             */
            SensorCapabilities.prototype.unitOfMeasure = "";
    
            /**
             * SensorCapabilities uncalibratedUnitOfMeasure.
             * @member {string}uncalibratedUnitOfMeasure
             * @memberof fk_app.SensorCapabilities
             * @instance
             */
            SensorCapabilities.prototype.uncalibratedUnitOfMeasure = "";
    
            /**
             * SensorCapabilities path.
             * @member {string}path
             * @memberof fk_app.SensorCapabilities
             * @instance
             */
            SensorCapabilities.prototype.path = "";
    
            /**
             * SensorCapabilities flags.
             * @member {number}flags
             * @memberof fk_app.SensorCapabilities
             * @instance
             */
            SensorCapabilities.prototype.flags = 0;
    
            /**
             * SensorCapabilities value.
             * @member {(fk_app.ILiveValue|null|undefined)}value
             * @memberof fk_app.SensorCapabilities
             * @instance
             */
            SensorCapabilities.prototype.value = null;
    
            /**
             * Creates a new SensorCapabilities instance using the specified properties.
             * @function create
             * @memberof fk_app.SensorCapabilities
             * @static
             * @param {fk_app.ISensorCapabilities=} [properties] Properties to set
             * @returns {fk_app.SensorCapabilities} SensorCapabilities instance
             */
            SensorCapabilities.create = function create(properties) {
                return new SensorCapabilities(properties);
            };
    
            /**
             * Encodes the specified SensorCapabilities message. Does not implicitly {@link fk_app.SensorCapabilities.verify|verify} messages.
             * @function encode
             * @memberof fk_app.SensorCapabilities
             * @static
             * @param {fk_app.ISensorCapabilities} message SensorCapabilities message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            SensorCapabilities.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.number != null && message.hasOwnProperty("number"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.number);
                if (message.module != null && message.hasOwnProperty("module"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint32(message.module);
                if (message.name != null && message.hasOwnProperty("name"))
                    writer.uint32(/* id 3, wireType 2 =*/26).string(message.name);
                if (message.frequency != null && message.hasOwnProperty("frequency"))
                    writer.uint32(/* id 4, wireType 0 =*/32).uint32(message.frequency);
                if (message.unitOfMeasure != null && message.hasOwnProperty("unitOfMeasure"))
                    writer.uint32(/* id 5, wireType 2 =*/42).string(message.unitOfMeasure);
                if (message.path != null && message.hasOwnProperty("path"))
                    writer.uint32(/* id 6, wireType 2 =*/50).string(message.path);
                if (message.flags != null && message.hasOwnProperty("flags"))
                    writer.uint32(/* id 7, wireType 0 =*/56).uint32(message.flags);
                if (message.value != null && message.hasOwnProperty("value"))
                    $root.fk_app.LiveValue.encode(message.value, writer.uint32(/* id 8, wireType 2 =*/66).fork()).ldelim();
                if (message.uncalibratedUnitOfMeasure != null && message.hasOwnProperty("uncalibratedUnitOfMeasure"))
                    writer.uint32(/* id 9, wireType 2 =*/74).string(message.uncalibratedUnitOfMeasure);
                return writer;
            };
    
            /**
             * Encodes the specified SensorCapabilities message, length delimited. Does not implicitly {@link fk_app.SensorCapabilities.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.SensorCapabilities
             * @static
             * @param {fk_app.ISensorCapabilities} message SensorCapabilities message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            SensorCapabilities.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a SensorCapabilities message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.SensorCapabilities
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.SensorCapabilities} SensorCapabilities
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            SensorCapabilities.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.SensorCapabilities();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.number = reader.uint32();
                        break;
                    case 2:
                        message.module = reader.uint32();
                        break;
                    case 3:
                        message.name = reader.string();
                        break;
                    case 4:
                        message.frequency = reader.uint32();
                        break;
                    case 5:
                        message.unitOfMeasure = reader.string();
                        break;
                    case 9:
                        message.uncalibratedUnitOfMeasure = reader.string();
                        break;
                    case 6:
                        message.path = reader.string();
                        break;
                    case 7:
                        message.flags = reader.uint32();
                        break;
                    case 8:
                        message.value = $root.fk_app.LiveValue.decode(reader, reader.uint32());
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a SensorCapabilities message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.SensorCapabilities
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.SensorCapabilities} SensorCapabilities
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            SensorCapabilities.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a SensorCapabilities message.
             * @function verify
             * @memberof fk_app.SensorCapabilities
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            SensorCapabilities.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.number != null && message.hasOwnProperty("number"))
                    if (!$util.isInteger(message.number))
                        return "number: integer expected";
                if (message.module != null && message.hasOwnProperty("module"))
                    if (!$util.isInteger(message.module))
                        return "module: integer expected";
                if (message.name != null && message.hasOwnProperty("name"))
                    if (!$util.isString(message.name))
                        return "name: string expected";
                if (message.frequency != null && message.hasOwnProperty("frequency"))
                    if (!$util.isInteger(message.frequency))
                        return "frequency: integer expected";
                if (message.unitOfMeasure != null && message.hasOwnProperty("unitOfMeasure"))
                    if (!$util.isString(message.unitOfMeasure))
                        return "unitOfMeasure: string expected";
                if (message.uncalibratedUnitOfMeasure != null && message.hasOwnProperty("uncalibratedUnitOfMeasure"))
                    if (!$util.isString(message.uncalibratedUnitOfMeasure))
                        return "uncalibratedUnitOfMeasure: string expected";
                if (message.path != null && message.hasOwnProperty("path"))
                    if (!$util.isString(message.path))
                        return "path: string expected";
                if (message.flags != null && message.hasOwnProperty("flags"))
                    if (!$util.isInteger(message.flags))
                        return "flags: integer expected";
                if (message.value != null && message.hasOwnProperty("value")) {
                    var error = $root.fk_app.LiveValue.verify(message.value);
                    if (error)
                        return "value." + error;
                }
                return null;
            };
    
            /**
             * Creates a SensorCapabilities message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.SensorCapabilities
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.SensorCapabilities} SensorCapabilities
             */
            SensorCapabilities.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.SensorCapabilities)
                    return object;
                var message = new $root.fk_app.SensorCapabilities();
                if (object.number != null)
                    message.number = object.number >>> 0;
                if (object.module != null)
                    message.module = object.module >>> 0;
                if (object.name != null)
                    message.name = String(object.name);
                if (object.frequency != null)
                    message.frequency = object.frequency >>> 0;
                if (object.unitOfMeasure != null)
                    message.unitOfMeasure = String(object.unitOfMeasure);
                if (object.uncalibratedUnitOfMeasure != null)
                    message.uncalibratedUnitOfMeasure = String(object.uncalibratedUnitOfMeasure);
                if (object.path != null)
                    message.path = String(object.path);
                if (object.flags != null)
                    message.flags = object.flags >>> 0;
                if (object.value != null) {
                    if (typeof object.value !== "object")
                        throw TypeError(".fk_app.SensorCapabilities.value: object expected");
                    message.value = $root.fk_app.LiveValue.fromObject(object.value);
                }
                return message;
            };
    
            /**
             * Creates a plain object from a SensorCapabilities message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.SensorCapabilities
             * @static
             * @param {fk_app.SensorCapabilities} message SensorCapabilities
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            SensorCapabilities.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.number = 0;
                    object.module = 0;
                    object.name = "";
                    object.frequency = 0;
                    object.unitOfMeasure = "";
                    object.path = "";
                    object.flags = 0;
                    object.value = null;
                    object.uncalibratedUnitOfMeasure = "";
                }
                if (message.number != null && message.hasOwnProperty("number"))
                    object.number = message.number;
                if (message.module != null && message.hasOwnProperty("module"))
                    object.module = message.module;
                if (message.name != null && message.hasOwnProperty("name"))
                    object.name = message.name;
                if (message.frequency != null && message.hasOwnProperty("frequency"))
                    object.frequency = message.frequency;
                if (message.unitOfMeasure != null && message.hasOwnProperty("unitOfMeasure"))
                    object.unitOfMeasure = message.unitOfMeasure;
                if (message.path != null && message.hasOwnProperty("path"))
                    object.path = message.path;
                if (message.flags != null && message.hasOwnProperty("flags"))
                    object.flags = message.flags;
                if (message.value != null && message.hasOwnProperty("value"))
                    object.value = $root.fk_app.LiveValue.toObject(message.value, options);
                if (message.uncalibratedUnitOfMeasure != null && message.hasOwnProperty("uncalibratedUnitOfMeasure"))
                    object.uncalibratedUnitOfMeasure = message.uncalibratedUnitOfMeasure;
                return object;
            };
    
            /**
             * Converts this SensorCapabilities to JSON.
             * @function toJSON
             * @memberof fk_app.SensorCapabilities
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            SensorCapabilities.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return SensorCapabilities;
        })();
    
        fk_app.ModuleHeader = (function() {
    
            /**
             * Properties of a ModuleHeader.
             * @memberof fk_app
             * @interface IModuleHeader
             * @property {number} [manufacturer] ModuleHeader manufacturer
             * @property {number} [kind] ModuleHeader kind
             * @property {number} [version] ModuleHeader version
             */
    
            /**
             * Constructs a new ModuleHeader.
             * @memberof fk_app
             * @classdesc Represents a ModuleHeader.
             * @constructor
             * @param {fk_app.IModuleHeader=} [properties] Properties to set
             */
            function ModuleHeader(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * ModuleHeader manufacturer.
             * @member {number}manufacturer
             * @memberof fk_app.ModuleHeader
             * @instance
             */
            ModuleHeader.prototype.manufacturer = 0;
    
            /**
             * ModuleHeader kind.
             * @member {number}kind
             * @memberof fk_app.ModuleHeader
             * @instance
             */
            ModuleHeader.prototype.kind = 0;
    
            /**
             * ModuleHeader version.
             * @member {number}version
             * @memberof fk_app.ModuleHeader
             * @instance
             */
            ModuleHeader.prototype.version = 0;
    
            /**
             * Creates a new ModuleHeader instance using the specified properties.
             * @function create
             * @memberof fk_app.ModuleHeader
             * @static
             * @param {fk_app.IModuleHeader=} [properties] Properties to set
             * @returns {fk_app.ModuleHeader} ModuleHeader instance
             */
            ModuleHeader.create = function create(properties) {
                return new ModuleHeader(properties);
            };
    
            /**
             * Encodes the specified ModuleHeader message. Does not implicitly {@link fk_app.ModuleHeader.verify|verify} messages.
             * @function encode
             * @memberof fk_app.ModuleHeader
             * @static
             * @param {fk_app.IModuleHeader} message ModuleHeader message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            ModuleHeader.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.manufacturer != null && message.hasOwnProperty("manufacturer"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.manufacturer);
                if (message.kind != null && message.hasOwnProperty("kind"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint32(message.kind);
                if (message.version != null && message.hasOwnProperty("version"))
                    writer.uint32(/* id 3, wireType 0 =*/24).uint32(message.version);
                return writer;
            };
    
            /**
             * Encodes the specified ModuleHeader message, length delimited. Does not implicitly {@link fk_app.ModuleHeader.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.ModuleHeader
             * @static
             * @param {fk_app.IModuleHeader} message ModuleHeader message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            ModuleHeader.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a ModuleHeader message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.ModuleHeader
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.ModuleHeader} ModuleHeader
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            ModuleHeader.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.ModuleHeader();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.manufacturer = reader.uint32();
                        break;
                    case 2:
                        message.kind = reader.uint32();
                        break;
                    case 3:
                        message.version = reader.uint32();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a ModuleHeader message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.ModuleHeader
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.ModuleHeader} ModuleHeader
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            ModuleHeader.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a ModuleHeader message.
             * @function verify
             * @memberof fk_app.ModuleHeader
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            ModuleHeader.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.manufacturer != null && message.hasOwnProperty("manufacturer"))
                    if (!$util.isInteger(message.manufacturer))
                        return "manufacturer: integer expected";
                if (message.kind != null && message.hasOwnProperty("kind"))
                    if (!$util.isInteger(message.kind))
                        return "kind: integer expected";
                if (message.version != null && message.hasOwnProperty("version"))
                    if (!$util.isInteger(message.version))
                        return "version: integer expected";
                return null;
            };
    
            /**
             * Creates a ModuleHeader message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.ModuleHeader
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.ModuleHeader} ModuleHeader
             */
            ModuleHeader.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.ModuleHeader)
                    return object;
                var message = new $root.fk_app.ModuleHeader();
                if (object.manufacturer != null)
                    message.manufacturer = object.manufacturer >>> 0;
                if (object.kind != null)
                    message.kind = object.kind >>> 0;
                if (object.version != null)
                    message.version = object.version >>> 0;
                return message;
            };
    
            /**
             * Creates a plain object from a ModuleHeader message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.ModuleHeader
             * @static
             * @param {fk_app.ModuleHeader} message ModuleHeader
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            ModuleHeader.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.manufacturer = 0;
                    object.kind = 0;
                    object.version = 0;
                }
                if (message.manufacturer != null && message.hasOwnProperty("manufacturer"))
                    object.manufacturer = message.manufacturer;
                if (message.kind != null && message.hasOwnProperty("kind"))
                    object.kind = message.kind;
                if (message.version != null && message.hasOwnProperty("version"))
                    object.version = message.version;
                return object;
            };
    
            /**
             * Converts this ModuleHeader to JSON.
             * @function toJSON
             * @memberof fk_app.ModuleHeader
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            ModuleHeader.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return ModuleHeader;
        })();
    
        fk_app.ModuleCapabilities = (function() {
    
            /**
             * Properties of a ModuleCapabilities.
             * @memberof fk_app
             * @interface IModuleCapabilities
             * @property {number} [position] ModuleCapabilities position
             * @property {string} [name] ModuleCapabilities name
             * @property {Array.<fk_app.ISensorCapabilities>} [sensors] ModuleCapabilities sensors
             * @property {string} [path] ModuleCapabilities path
             * @property {number} [flags] ModuleCapabilities flags
             * @property {Uint8Array} [id] ModuleCapabilities id
             * @property {fk_app.IModuleHeader} [header] ModuleCapabilities header
             * @property {Uint8Array} [configuration] ModuleCapabilities configuration
             */
    
            /**
             * Constructs a new ModuleCapabilities.
             * @memberof fk_app
             * @classdesc Represents a ModuleCapabilities.
             * @constructor
             * @param {fk_app.IModuleCapabilities=} [properties] Properties to set
             */
            function ModuleCapabilities(properties) {
                this.sensors = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * ModuleCapabilities position.
             * @member {number}position
             * @memberof fk_app.ModuleCapabilities
             * @instance
             */
            ModuleCapabilities.prototype.position = 0;
    
            /**
             * ModuleCapabilities name.
             * @member {string}name
             * @memberof fk_app.ModuleCapabilities
             * @instance
             */
            ModuleCapabilities.prototype.name = "";
    
            /**
             * ModuleCapabilities sensors.
             * @member {Array.<fk_app.ISensorCapabilities>}sensors
             * @memberof fk_app.ModuleCapabilities
             * @instance
             */
            ModuleCapabilities.prototype.sensors = $util.emptyArray;
    
            /**
             * ModuleCapabilities path.
             * @member {string}path
             * @memberof fk_app.ModuleCapabilities
             * @instance
             */
            ModuleCapabilities.prototype.path = "";
    
            /**
             * ModuleCapabilities flags.
             * @member {number}flags
             * @memberof fk_app.ModuleCapabilities
             * @instance
             */
            ModuleCapabilities.prototype.flags = 0;
    
            /**
             * ModuleCapabilities id.
             * @member {Uint8Array}id
             * @memberof fk_app.ModuleCapabilities
             * @instance
             */
            ModuleCapabilities.prototype.id = $util.newBuffer([]);
    
            /**
             * ModuleCapabilities header.
             * @member {(fk_app.IModuleHeader|null|undefined)}header
             * @memberof fk_app.ModuleCapabilities
             * @instance
             */
            ModuleCapabilities.prototype.header = null;
    
            /**
             * ModuleCapabilities configuration.
             * @member {Uint8Array}configuration
             * @memberof fk_app.ModuleCapabilities
             * @instance
             */
            ModuleCapabilities.prototype.configuration = $util.newBuffer([]);
    
            /**
             * Creates a new ModuleCapabilities instance using the specified properties.
             * @function create
             * @memberof fk_app.ModuleCapabilities
             * @static
             * @param {fk_app.IModuleCapabilities=} [properties] Properties to set
             * @returns {fk_app.ModuleCapabilities} ModuleCapabilities instance
             */
            ModuleCapabilities.create = function create(properties) {
                return new ModuleCapabilities(properties);
            };
    
            /**
             * Encodes the specified ModuleCapabilities message. Does not implicitly {@link fk_app.ModuleCapabilities.verify|verify} messages.
             * @function encode
             * @memberof fk_app.ModuleCapabilities
             * @static
             * @param {fk_app.IModuleCapabilities} message ModuleCapabilities message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            ModuleCapabilities.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.position != null && message.hasOwnProperty("position"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.position);
                if (message.name != null && message.hasOwnProperty("name"))
                    writer.uint32(/* id 2, wireType 2 =*/18).string(message.name);
                if (message.sensors != null && message.sensors.length)
                    for (var i = 0; i < message.sensors.length; ++i)
                        $root.fk_app.SensorCapabilities.encode(message.sensors[i], writer.uint32(/* id 3, wireType 2 =*/26).fork()).ldelim();
                if (message.path != null && message.hasOwnProperty("path"))
                    writer.uint32(/* id 4, wireType 2 =*/34).string(message.path);
                if (message.flags != null && message.hasOwnProperty("flags"))
                    writer.uint32(/* id 5, wireType 0 =*/40).uint32(message.flags);
                if (message.id != null && message.hasOwnProperty("id"))
                    writer.uint32(/* id 6, wireType 2 =*/50).bytes(message.id);
                if (message.header != null && message.hasOwnProperty("header"))
                    $root.fk_app.ModuleHeader.encode(message.header, writer.uint32(/* id 7, wireType 2 =*/58).fork()).ldelim();
                if (message.configuration != null && message.hasOwnProperty("configuration"))
                    writer.uint32(/* id 8, wireType 2 =*/66).bytes(message.configuration);
                return writer;
            };
    
            /**
             * Encodes the specified ModuleCapabilities message, length delimited. Does not implicitly {@link fk_app.ModuleCapabilities.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.ModuleCapabilities
             * @static
             * @param {fk_app.IModuleCapabilities} message ModuleCapabilities message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            ModuleCapabilities.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a ModuleCapabilities message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.ModuleCapabilities
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.ModuleCapabilities} ModuleCapabilities
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            ModuleCapabilities.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.ModuleCapabilities();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.position = reader.uint32();
                        break;
                    case 2:
                        message.name = reader.string();
                        break;
                    case 3:
                        if (!(message.sensors && message.sensors.length))
                            message.sensors = [];
                        message.sensors.push($root.fk_app.SensorCapabilities.decode(reader, reader.uint32()));
                        break;
                    case 4:
                        message.path = reader.string();
                        break;
                    case 5:
                        message.flags = reader.uint32();
                        break;
                    case 6:
                        message.id = reader.bytes();
                        break;
                    case 7:
                        message.header = $root.fk_app.ModuleHeader.decode(reader, reader.uint32());
                        break;
                    case 8:
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
             * Decodes a ModuleCapabilities message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.ModuleCapabilities
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.ModuleCapabilities} ModuleCapabilities
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            ModuleCapabilities.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a ModuleCapabilities message.
             * @function verify
             * @memberof fk_app.ModuleCapabilities
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            ModuleCapabilities.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.position != null && message.hasOwnProperty("position"))
                    if (!$util.isInteger(message.position))
                        return "position: integer expected";
                if (message.name != null && message.hasOwnProperty("name"))
                    if (!$util.isString(message.name))
                        return "name: string expected";
                if (message.sensors != null && message.hasOwnProperty("sensors")) {
                    if (!Array.isArray(message.sensors))
                        return "sensors: array expected";
                    for (var i = 0; i < message.sensors.length; ++i) {
                        var error = $root.fk_app.SensorCapabilities.verify(message.sensors[i]);
                        if (error)
                            return "sensors." + error;
                    }
                }
                if (message.path != null && message.hasOwnProperty("path"))
                    if (!$util.isString(message.path))
                        return "path: string expected";
                if (message.flags != null && message.hasOwnProperty("flags"))
                    if (!$util.isInteger(message.flags))
                        return "flags: integer expected";
                if (message.id != null && message.hasOwnProperty("id"))
                    if (!(message.id && typeof message.id.length === "number" || $util.isString(message.id)))
                        return "id: buffer expected";
                if (message.header != null && message.hasOwnProperty("header")) {
                    error = $root.fk_app.ModuleHeader.verify(message.header);
                    if (error)
                        return "header." + error;
                }
                if (message.configuration != null && message.hasOwnProperty("configuration"))
                    if (!(message.configuration && typeof message.configuration.length === "number" || $util.isString(message.configuration)))
                        return "configuration: buffer expected";
                return null;
            };
    
            /**
             * Creates a ModuleCapabilities message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.ModuleCapabilities
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.ModuleCapabilities} ModuleCapabilities
             */
            ModuleCapabilities.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.ModuleCapabilities)
                    return object;
                var message = new $root.fk_app.ModuleCapabilities();
                if (object.position != null)
                    message.position = object.position >>> 0;
                if (object.name != null)
                    message.name = String(object.name);
                if (object.sensors) {
                    if (!Array.isArray(object.sensors))
                        throw TypeError(".fk_app.ModuleCapabilities.sensors: array expected");
                    message.sensors = [];
                    for (var i = 0; i < object.sensors.length; ++i) {
                        if (typeof object.sensors[i] !== "object")
                            throw TypeError(".fk_app.ModuleCapabilities.sensors: object expected");
                        message.sensors[i] = $root.fk_app.SensorCapabilities.fromObject(object.sensors[i]);
                    }
                }
                if (object.path != null)
                    message.path = String(object.path);
                if (object.flags != null)
                    message.flags = object.flags >>> 0;
                if (object.id != null)
                    if (typeof object.id === "string")
                        $util.base64.decode(object.id, message.id = $util.newBuffer($util.base64.length(object.id)), 0);
                    else if (object.id.length)
                        message.id = object.id;
                if (object.header != null) {
                    if (typeof object.header !== "object")
                        throw TypeError(".fk_app.ModuleCapabilities.header: object expected");
                    message.header = $root.fk_app.ModuleHeader.fromObject(object.header);
                }
                if (object.configuration != null)
                    if (typeof object.configuration === "string")
                        $util.base64.decode(object.configuration, message.configuration = $util.newBuffer($util.base64.length(object.configuration)), 0);
                    else if (object.configuration.length)
                        message.configuration = object.configuration;
                return message;
            };
    
            /**
             * Creates a plain object from a ModuleCapabilities message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.ModuleCapabilities
             * @static
             * @param {fk_app.ModuleCapabilities} message ModuleCapabilities
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            ModuleCapabilities.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults)
                    object.sensors = [];
                if (options.defaults) {
                    object.position = 0;
                    object.name = "";
                    object.path = "";
                    object.flags = 0;
                    object.id = options.bytes === String ? "" : [];
                    object.header = null;
                    object.configuration = options.bytes === String ? "" : [];
                }
                if (message.position != null && message.hasOwnProperty("position"))
                    object.position = message.position;
                if (message.name != null && message.hasOwnProperty("name"))
                    object.name = message.name;
                if (message.sensors && message.sensors.length) {
                    object.sensors = [];
                    for (var j = 0; j < message.sensors.length; ++j)
                        object.sensors[j] = $root.fk_app.SensorCapabilities.toObject(message.sensors[j], options);
                }
                if (message.path != null && message.hasOwnProperty("path"))
                    object.path = message.path;
                if (message.flags != null && message.hasOwnProperty("flags"))
                    object.flags = message.flags;
                if (message.id != null && message.hasOwnProperty("id"))
                    object.id = options.bytes === String ? $util.base64.encode(message.id, 0, message.id.length) : options.bytes === Array ? Array.prototype.slice.call(message.id) : message.id;
                if (message.header != null && message.hasOwnProperty("header"))
                    object.header = $root.fk_app.ModuleHeader.toObject(message.header, options);
                if (message.configuration != null && message.hasOwnProperty("configuration"))
                    object.configuration = options.bytes === String ? $util.base64.encode(message.configuration, 0, message.configuration.length) : options.bytes === Array ? Array.prototype.slice.call(message.configuration) : message.configuration;
                return object;
            };
    
            /**
             * Converts this ModuleCapabilities to JSON.
             * @function toJSON
             * @memberof fk_app.ModuleCapabilities
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            ModuleCapabilities.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return ModuleCapabilities;
        })();
    
        fk_app.Capabilities = (function() {
    
            /**
             * Properties of a Capabilities.
             * @memberof fk_app
             * @interface ICapabilities
             * @property {number} [version] Capabilities version
             * @property {Uint8Array} [deviceId] Capabilities deviceId
             * @property {string} [name] Capabilities name
             * @property {Array.<fk_app.IModuleCapabilities>} [modules] Capabilities modules
             * @property {Array.<fk_app.ISensorCapabilities>} [sensors] Capabilities sensors
             */
    
            /**
             * Constructs a new Capabilities.
             * @memberof fk_app
             * @classdesc Represents a Capabilities.
             * @constructor
             * @param {fk_app.ICapabilities=} [properties] Properties to set
             */
            function Capabilities(properties) {
                this.modules = [];
                this.sensors = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Capabilities version.
             * @member {number}version
             * @memberof fk_app.Capabilities
             * @instance
             */
            Capabilities.prototype.version = 0;
    
            /**
             * Capabilities deviceId.
             * @member {Uint8Array}deviceId
             * @memberof fk_app.Capabilities
             * @instance
             */
            Capabilities.prototype.deviceId = $util.newBuffer([]);
    
            /**
             * Capabilities name.
             * @member {string}name
             * @memberof fk_app.Capabilities
             * @instance
             */
            Capabilities.prototype.name = "";
    
            /**
             * Capabilities modules.
             * @member {Array.<fk_app.IModuleCapabilities>}modules
             * @memberof fk_app.Capabilities
             * @instance
             */
            Capabilities.prototype.modules = $util.emptyArray;
    
            /**
             * Capabilities sensors.
             * @member {Array.<fk_app.ISensorCapabilities>}sensors
             * @memberof fk_app.Capabilities
             * @instance
             */
            Capabilities.prototype.sensors = $util.emptyArray;
    
            /**
             * Creates a new Capabilities instance using the specified properties.
             * @function create
             * @memberof fk_app.Capabilities
             * @static
             * @param {fk_app.ICapabilities=} [properties] Properties to set
             * @returns {fk_app.Capabilities} Capabilities instance
             */
            Capabilities.create = function create(properties) {
                return new Capabilities(properties);
            };
    
            /**
             * Encodes the specified Capabilities message. Does not implicitly {@link fk_app.Capabilities.verify|verify} messages.
             * @function encode
             * @memberof fk_app.Capabilities
             * @static
             * @param {fk_app.ICapabilities} message Capabilities message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Capabilities.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.version != null && message.hasOwnProperty("version"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.version);
                if (message.deviceId != null && message.hasOwnProperty("deviceId"))
                    writer.uint32(/* id 2, wireType 2 =*/18).bytes(message.deviceId);
                if (message.name != null && message.hasOwnProperty("name"))
                    writer.uint32(/* id 3, wireType 2 =*/26).string(message.name);
                if (message.modules != null && message.modules.length)
                    for (var i = 0; i < message.modules.length; ++i)
                        $root.fk_app.ModuleCapabilities.encode(message.modules[i], writer.uint32(/* id 4, wireType 2 =*/34).fork()).ldelim();
                if (message.sensors != null && message.sensors.length)
                    for (var i = 0; i < message.sensors.length; ++i)
                        $root.fk_app.SensorCapabilities.encode(message.sensors[i], writer.uint32(/* id 5, wireType 2 =*/42).fork()).ldelim();
                return writer;
            };
    
            /**
             * Encodes the specified Capabilities message, length delimited. Does not implicitly {@link fk_app.Capabilities.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.Capabilities
             * @static
             * @param {fk_app.ICapabilities} message Capabilities message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Capabilities.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a Capabilities message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.Capabilities
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.Capabilities} Capabilities
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Capabilities.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.Capabilities();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.version = reader.uint32();
                        break;
                    case 2:
                        message.deviceId = reader.bytes();
                        break;
                    case 3:
                        message.name = reader.string();
                        break;
                    case 4:
                        if (!(message.modules && message.modules.length))
                            message.modules = [];
                        message.modules.push($root.fk_app.ModuleCapabilities.decode(reader, reader.uint32()));
                        break;
                    case 5:
                        if (!(message.sensors && message.sensors.length))
                            message.sensors = [];
                        message.sensors.push($root.fk_app.SensorCapabilities.decode(reader, reader.uint32()));
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a Capabilities message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.Capabilities
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.Capabilities} Capabilities
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Capabilities.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a Capabilities message.
             * @function verify
             * @memberof fk_app.Capabilities
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            Capabilities.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.version != null && message.hasOwnProperty("version"))
                    if (!$util.isInteger(message.version))
                        return "version: integer expected";
                if (message.deviceId != null && message.hasOwnProperty("deviceId"))
                    if (!(message.deviceId && typeof message.deviceId.length === "number" || $util.isString(message.deviceId)))
                        return "deviceId: buffer expected";
                if (message.name != null && message.hasOwnProperty("name"))
                    if (!$util.isString(message.name))
                        return "name: string expected";
                if (message.modules != null && message.hasOwnProperty("modules")) {
                    if (!Array.isArray(message.modules))
                        return "modules: array expected";
                    for (var i = 0; i < message.modules.length; ++i) {
                        var error = $root.fk_app.ModuleCapabilities.verify(message.modules[i]);
                        if (error)
                            return "modules." + error;
                    }
                }
                if (message.sensors != null && message.hasOwnProperty("sensors")) {
                    if (!Array.isArray(message.sensors))
                        return "sensors: array expected";
                    for (var i = 0; i < message.sensors.length; ++i) {
                        error = $root.fk_app.SensorCapabilities.verify(message.sensors[i]);
                        if (error)
                            return "sensors." + error;
                    }
                }
                return null;
            };
    
            /**
             * Creates a Capabilities message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.Capabilities
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.Capabilities} Capabilities
             */
            Capabilities.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.Capabilities)
                    return object;
                var message = new $root.fk_app.Capabilities();
                if (object.version != null)
                    message.version = object.version >>> 0;
                if (object.deviceId != null)
                    if (typeof object.deviceId === "string")
                        $util.base64.decode(object.deviceId, message.deviceId = $util.newBuffer($util.base64.length(object.deviceId)), 0);
                    else if (object.deviceId.length)
                        message.deviceId = object.deviceId;
                if (object.name != null)
                    message.name = String(object.name);
                if (object.modules) {
                    if (!Array.isArray(object.modules))
                        throw TypeError(".fk_app.Capabilities.modules: array expected");
                    message.modules = [];
                    for (var i = 0; i < object.modules.length; ++i) {
                        if (typeof object.modules[i] !== "object")
                            throw TypeError(".fk_app.Capabilities.modules: object expected");
                        message.modules[i] = $root.fk_app.ModuleCapabilities.fromObject(object.modules[i]);
                    }
                }
                if (object.sensors) {
                    if (!Array.isArray(object.sensors))
                        throw TypeError(".fk_app.Capabilities.sensors: array expected");
                    message.sensors = [];
                    for (var i = 0; i < object.sensors.length; ++i) {
                        if (typeof object.sensors[i] !== "object")
                            throw TypeError(".fk_app.Capabilities.sensors: object expected");
                        message.sensors[i] = $root.fk_app.SensorCapabilities.fromObject(object.sensors[i]);
                    }
                }
                return message;
            };
    
            /**
             * Creates a plain object from a Capabilities message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.Capabilities
             * @static
             * @param {fk_app.Capabilities} message Capabilities
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            Capabilities.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults) {
                    object.modules = [];
                    object.sensors = [];
                }
                if (options.defaults) {
                    object.version = 0;
                    object.deviceId = options.bytes === String ? "" : [];
                    object.name = "";
                }
                if (message.version != null && message.hasOwnProperty("version"))
                    object.version = message.version;
                if (message.deviceId != null && message.hasOwnProperty("deviceId"))
                    object.deviceId = options.bytes === String ? $util.base64.encode(message.deviceId, 0, message.deviceId.length) : options.bytes === Array ? Array.prototype.slice.call(message.deviceId) : message.deviceId;
                if (message.name != null && message.hasOwnProperty("name"))
                    object.name = message.name;
                if (message.modules && message.modules.length) {
                    object.modules = [];
                    for (var j = 0; j < message.modules.length; ++j)
                        object.modules[j] = $root.fk_app.ModuleCapabilities.toObject(message.modules[j], options);
                }
                if (message.sensors && message.sensors.length) {
                    object.sensors = [];
                    for (var j = 0; j < message.sensors.length; ++j)
                        object.sensors[j] = $root.fk_app.SensorCapabilities.toObject(message.sensors[j], options);
                }
                return object;
            };
    
            /**
             * Converts this Capabilities to JSON.
             * @function toJSON
             * @memberof fk_app.Capabilities
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Capabilities.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Capabilities;
        })();
    
        fk_app.NetworkInfo = (function() {
    
            /**
             * Properties of a NetworkInfo.
             * @memberof fk_app
             * @interface INetworkInfo
             * @property {string} [ssid] NetworkInfo ssid
             * @property {string} [password] NetworkInfo password
             * @property {boolean} [create] NetworkInfo create
             * @property {boolean} [preferred] NetworkInfo preferred
             * @property {boolean} [keeping] NetworkInfo keeping
             */
    
            /**
             * Constructs a new NetworkInfo.
             * @memberof fk_app
             * @classdesc Represents a NetworkInfo.
             * @constructor
             * @param {fk_app.INetworkInfo=} [properties] Properties to set
             */
            function NetworkInfo(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * NetworkInfo ssid.
             * @member {string}ssid
             * @memberof fk_app.NetworkInfo
             * @instance
             */
            NetworkInfo.prototype.ssid = "";
    
            /**
             * NetworkInfo password.
             * @member {string}password
             * @memberof fk_app.NetworkInfo
             * @instance
             */
            NetworkInfo.prototype.password = "";
    
            /**
             * NetworkInfo create.
             * @member {boolean}create
             * @memberof fk_app.NetworkInfo
             * @instance
             */
            NetworkInfo.prototype.create = false;
    
            /**
             * NetworkInfo preferred.
             * @member {boolean}preferred
             * @memberof fk_app.NetworkInfo
             * @instance
             */
            NetworkInfo.prototype.preferred = false;
    
            /**
             * NetworkInfo keeping.
             * @member {boolean}keeping
             * @memberof fk_app.NetworkInfo
             * @instance
             */
            NetworkInfo.prototype.keeping = false;
    
            /**
             * Creates a new NetworkInfo instance using the specified properties.
             * @function create
             * @memberof fk_app.NetworkInfo
             * @static
             * @param {fk_app.INetworkInfo=} [properties] Properties to set
             * @returns {fk_app.NetworkInfo} NetworkInfo instance
             */
            NetworkInfo.create = function create(properties) {
                return new NetworkInfo(properties);
            };
    
            /**
             * Encodes the specified NetworkInfo message. Does not implicitly {@link fk_app.NetworkInfo.verify|verify} messages.
             * @function encode
             * @memberof fk_app.NetworkInfo
             * @static
             * @param {fk_app.INetworkInfo} message NetworkInfo message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            NetworkInfo.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.ssid != null && message.hasOwnProperty("ssid"))
                    writer.uint32(/* id 1, wireType 2 =*/10).string(message.ssid);
                if (message.password != null && message.hasOwnProperty("password"))
                    writer.uint32(/* id 2, wireType 2 =*/18).string(message.password);
                if (message.create != null && message.hasOwnProperty("create"))
                    writer.uint32(/* id 3, wireType 0 =*/24).bool(message.create);
                if (message.preferred != null && message.hasOwnProperty("preferred"))
                    writer.uint32(/* id 4, wireType 0 =*/32).bool(message.preferred);
                if (message.keeping != null && message.hasOwnProperty("keeping"))
                    writer.uint32(/* id 5, wireType 0 =*/40).bool(message.keeping);
                return writer;
            };
    
            /**
             * Encodes the specified NetworkInfo message, length delimited. Does not implicitly {@link fk_app.NetworkInfo.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.NetworkInfo
             * @static
             * @param {fk_app.INetworkInfo} message NetworkInfo message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            NetworkInfo.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a NetworkInfo message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.NetworkInfo
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.NetworkInfo} NetworkInfo
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            NetworkInfo.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.NetworkInfo();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.ssid = reader.string();
                        break;
                    case 2:
                        message.password = reader.string();
                        break;
                    case 3:
                        message.create = reader.bool();
                        break;
                    case 4:
                        message.preferred = reader.bool();
                        break;
                    case 5:
                        message.keeping = reader.bool();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a NetworkInfo message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.NetworkInfo
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.NetworkInfo} NetworkInfo
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            NetworkInfo.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a NetworkInfo message.
             * @function verify
             * @memberof fk_app.NetworkInfo
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            NetworkInfo.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.ssid != null && message.hasOwnProperty("ssid"))
                    if (!$util.isString(message.ssid))
                        return "ssid: string expected";
                if (message.password != null && message.hasOwnProperty("password"))
                    if (!$util.isString(message.password))
                        return "password: string expected";
                if (message.create != null && message.hasOwnProperty("create"))
                    if (typeof message.create !== "boolean")
                        return "create: boolean expected";
                if (message.preferred != null && message.hasOwnProperty("preferred"))
                    if (typeof message.preferred !== "boolean")
                        return "preferred: boolean expected";
                if (message.keeping != null && message.hasOwnProperty("keeping"))
                    if (typeof message.keeping !== "boolean")
                        return "keeping: boolean expected";
                return null;
            };
    
            /**
             * Creates a NetworkInfo message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.NetworkInfo
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.NetworkInfo} NetworkInfo
             */
            NetworkInfo.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.NetworkInfo)
                    return object;
                var message = new $root.fk_app.NetworkInfo();
                if (object.ssid != null)
                    message.ssid = String(object.ssid);
                if (object.password != null)
                    message.password = String(object.password);
                if (object.create != null)
                    message.create = Boolean(object.create);
                if (object.preferred != null)
                    message.preferred = Boolean(object.preferred);
                if (object.keeping != null)
                    message.keeping = Boolean(object.keeping);
                return message;
            };
    
            /**
             * Creates a plain object from a NetworkInfo message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.NetworkInfo
             * @static
             * @param {fk_app.NetworkInfo} message NetworkInfo
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            NetworkInfo.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.ssid = "";
                    object.password = "";
                    object.create = false;
                    object.preferred = false;
                    object.keeping = false;
                }
                if (message.ssid != null && message.hasOwnProperty("ssid"))
                    object.ssid = message.ssid;
                if (message.password != null && message.hasOwnProperty("password"))
                    object.password = message.password;
                if (message.create != null && message.hasOwnProperty("create"))
                    object.create = message.create;
                if (message.preferred != null && message.hasOwnProperty("preferred"))
                    object.preferred = message.preferred;
                if (message.keeping != null && message.hasOwnProperty("keeping"))
                    object.keeping = message.keeping;
                return object;
            };
    
            /**
             * Converts this NetworkInfo to JSON.
             * @function toJSON
             * @memberof fk_app.NetworkInfo
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            NetworkInfo.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return NetworkInfo;
        })();
    
        fk_app.NetworkSettings = (function() {
    
            /**
             * Properties of a NetworkSettings.
             * @memberof fk_app
             * @interface INetworkSettings
             * @property {number} [createAccessPoint] NetworkSettings createAccessPoint
             * @property {fk_app.INetworkInfo} [connected] NetworkSettings connected
             * @property {string} [macAddress] NetworkSettings macAddress
             * @property {boolean} [modifying] NetworkSettings modifying
             * @property {Array.<fk_app.INetworkInfo>} [networks] NetworkSettings networks
             */
    
            /**
             * Constructs a new NetworkSettings.
             * @memberof fk_app
             * @classdesc Represents a NetworkSettings.
             * @constructor
             * @param {fk_app.INetworkSettings=} [properties] Properties to set
             */
            function NetworkSettings(properties) {
                this.networks = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * NetworkSettings createAccessPoint.
             * @member {number}createAccessPoint
             * @memberof fk_app.NetworkSettings
             * @instance
             */
            NetworkSettings.prototype.createAccessPoint = 0;
    
            /**
             * NetworkSettings connected.
             * @member {(fk_app.INetworkInfo|null|undefined)}connected
             * @memberof fk_app.NetworkSettings
             * @instance
             */
            NetworkSettings.prototype.connected = null;
    
            /**
             * NetworkSettings macAddress.
             * @member {string}macAddress
             * @memberof fk_app.NetworkSettings
             * @instance
             */
            NetworkSettings.prototype.macAddress = "";
    
            /**
             * NetworkSettings modifying.
             * @member {boolean}modifying
             * @memberof fk_app.NetworkSettings
             * @instance
             */
            NetworkSettings.prototype.modifying = false;
    
            /**
             * NetworkSettings networks.
             * @member {Array.<fk_app.INetworkInfo>}networks
             * @memberof fk_app.NetworkSettings
             * @instance
             */
            NetworkSettings.prototype.networks = $util.emptyArray;
    
            /**
             * Creates a new NetworkSettings instance using the specified properties.
             * @function create
             * @memberof fk_app.NetworkSettings
             * @static
             * @param {fk_app.INetworkSettings=} [properties] Properties to set
             * @returns {fk_app.NetworkSettings} NetworkSettings instance
             */
            NetworkSettings.create = function create(properties) {
                return new NetworkSettings(properties);
            };
    
            /**
             * Encodes the specified NetworkSettings message. Does not implicitly {@link fk_app.NetworkSettings.verify|verify} messages.
             * @function encode
             * @memberof fk_app.NetworkSettings
             * @static
             * @param {fk_app.INetworkSettings} message NetworkSettings message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            NetworkSettings.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.createAccessPoint != null && message.hasOwnProperty("createAccessPoint"))
                    writer.uint32(/* id 1, wireType 0 =*/8).int32(message.createAccessPoint);
                if (message.networks != null && message.networks.length)
                    for (var i = 0; i < message.networks.length; ++i)
                        $root.fk_app.NetworkInfo.encode(message.networks[i], writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
                if (message.connected != null && message.hasOwnProperty("connected"))
                    $root.fk_app.NetworkInfo.encode(message.connected, writer.uint32(/* id 3, wireType 2 =*/26).fork()).ldelim();
                if (message.macAddress != null && message.hasOwnProperty("macAddress"))
                    writer.uint32(/* id 4, wireType 2 =*/34).string(message.macAddress);
                if (message.modifying != null && message.hasOwnProperty("modifying"))
                    writer.uint32(/* id 5, wireType 0 =*/40).bool(message.modifying);
                return writer;
            };
    
            /**
             * Encodes the specified NetworkSettings message, length delimited. Does not implicitly {@link fk_app.NetworkSettings.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.NetworkSettings
             * @static
             * @param {fk_app.INetworkSettings} message NetworkSettings message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            NetworkSettings.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a NetworkSettings message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.NetworkSettings
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.NetworkSettings} NetworkSettings
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            NetworkSettings.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.NetworkSettings();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.createAccessPoint = reader.int32();
                        break;
                    case 3:
                        message.connected = $root.fk_app.NetworkInfo.decode(reader, reader.uint32());
                        break;
                    case 4:
                        message.macAddress = reader.string();
                        break;
                    case 5:
                        message.modifying = reader.bool();
                        break;
                    case 2:
                        if (!(message.networks && message.networks.length))
                            message.networks = [];
                        message.networks.push($root.fk_app.NetworkInfo.decode(reader, reader.uint32()));
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a NetworkSettings message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.NetworkSettings
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.NetworkSettings} NetworkSettings
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            NetworkSettings.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a NetworkSettings message.
             * @function verify
             * @memberof fk_app.NetworkSettings
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            NetworkSettings.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.createAccessPoint != null && message.hasOwnProperty("createAccessPoint"))
                    if (!$util.isInteger(message.createAccessPoint))
                        return "createAccessPoint: integer expected";
                if (message.connected != null && message.hasOwnProperty("connected")) {
                    var error = $root.fk_app.NetworkInfo.verify(message.connected);
                    if (error)
                        return "connected." + error;
                }
                if (message.macAddress != null && message.hasOwnProperty("macAddress"))
                    if (!$util.isString(message.macAddress))
                        return "macAddress: string expected";
                if (message.modifying != null && message.hasOwnProperty("modifying"))
                    if (typeof message.modifying !== "boolean")
                        return "modifying: boolean expected";
                if (message.networks != null && message.hasOwnProperty("networks")) {
                    if (!Array.isArray(message.networks))
                        return "networks: array expected";
                    for (var i = 0; i < message.networks.length; ++i) {
                        error = $root.fk_app.NetworkInfo.verify(message.networks[i]);
                        if (error)
                            return "networks." + error;
                    }
                }
                return null;
            };
    
            /**
             * Creates a NetworkSettings message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.NetworkSettings
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.NetworkSettings} NetworkSettings
             */
            NetworkSettings.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.NetworkSettings)
                    return object;
                var message = new $root.fk_app.NetworkSettings();
                if (object.createAccessPoint != null)
                    message.createAccessPoint = object.createAccessPoint | 0;
                if (object.connected != null) {
                    if (typeof object.connected !== "object")
                        throw TypeError(".fk_app.NetworkSettings.connected: object expected");
                    message.connected = $root.fk_app.NetworkInfo.fromObject(object.connected);
                }
                if (object.macAddress != null)
                    message.macAddress = String(object.macAddress);
                if (object.modifying != null)
                    message.modifying = Boolean(object.modifying);
                if (object.networks) {
                    if (!Array.isArray(object.networks))
                        throw TypeError(".fk_app.NetworkSettings.networks: array expected");
                    message.networks = [];
                    for (var i = 0; i < object.networks.length; ++i) {
                        if (typeof object.networks[i] !== "object")
                            throw TypeError(".fk_app.NetworkSettings.networks: object expected");
                        message.networks[i] = $root.fk_app.NetworkInfo.fromObject(object.networks[i]);
                    }
                }
                return message;
            };
    
            /**
             * Creates a plain object from a NetworkSettings message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.NetworkSettings
             * @static
             * @param {fk_app.NetworkSettings} message NetworkSettings
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            NetworkSettings.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults)
                    object.networks = [];
                if (options.defaults) {
                    object.createAccessPoint = 0;
                    object.connected = null;
                    object.macAddress = "";
                    object.modifying = false;
                }
                if (message.createAccessPoint != null && message.hasOwnProperty("createAccessPoint"))
                    object.createAccessPoint = message.createAccessPoint;
                if (message.networks && message.networks.length) {
                    object.networks = [];
                    for (var j = 0; j < message.networks.length; ++j)
                        object.networks[j] = $root.fk_app.NetworkInfo.toObject(message.networks[j], options);
                }
                if (message.connected != null && message.hasOwnProperty("connected"))
                    object.connected = $root.fk_app.NetworkInfo.toObject(message.connected, options);
                if (message.macAddress != null && message.hasOwnProperty("macAddress"))
                    object.macAddress = message.macAddress;
                if (message.modifying != null && message.hasOwnProperty("modifying"))
                    object.modifying = message.modifying;
                return object;
            };
    
            /**
             * Converts this NetworkSettings to JSON.
             * @function toJSON
             * @memberof fk_app.NetworkSettings
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            NetworkSettings.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return NetworkSettings;
        })();
    
        fk_app.Firmware = (function() {
    
            /**
             * Properties of a Firmware.
             * @memberof fk_app
             * @interface IFirmware
             * @property {string} [version] Firmware version
             * @property {string} [build] Firmware build
             * @property {string} [number] Firmware number
             * @property {number|Long} [timestamp] Firmware timestamp
             * @property {string} [hash] Firmware hash
             * @property {number|Long} [logicalAddress] Firmware logicalAddress
             * @property {string} [name] Firmware name
             */
    
            /**
             * Constructs a new Firmware.
             * @memberof fk_app
             * @classdesc Represents a Firmware.
             * @constructor
             * @param {fk_app.IFirmware=} [properties] Properties to set
             */
            function Firmware(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Firmware version.
             * @member {string}version
             * @memberof fk_app.Firmware
             * @instance
             */
            Firmware.prototype.version = "";
    
            /**
             * Firmware build.
             * @member {string}build
             * @memberof fk_app.Firmware
             * @instance
             */
            Firmware.prototype.build = "";
    
            /**
             * Firmware number.
             * @member {string}number
             * @memberof fk_app.Firmware
             * @instance
             */
            Firmware.prototype.number = "";
    
            /**
             * Firmware timestamp.
             * @member {number|Long}timestamp
             * @memberof fk_app.Firmware
             * @instance
             */
            Firmware.prototype.timestamp = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * Firmware hash.
             * @member {string}hash
             * @memberof fk_app.Firmware
             * @instance
             */
            Firmware.prototype.hash = "";
    
            /**
             * Firmware logicalAddress.
             * @member {number|Long}logicalAddress
             * @memberof fk_app.Firmware
             * @instance
             */
            Firmware.prototype.logicalAddress = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * Firmware name.
             * @member {string}name
             * @memberof fk_app.Firmware
             * @instance
             */
            Firmware.prototype.name = "";
    
            /**
             * Creates a new Firmware instance using the specified properties.
             * @function create
             * @memberof fk_app.Firmware
             * @static
             * @param {fk_app.IFirmware=} [properties] Properties to set
             * @returns {fk_app.Firmware} Firmware instance
             */
            Firmware.create = function create(properties) {
                return new Firmware(properties);
            };
    
            /**
             * Encodes the specified Firmware message. Does not implicitly {@link fk_app.Firmware.verify|verify} messages.
             * @function encode
             * @memberof fk_app.Firmware
             * @static
             * @param {fk_app.IFirmware} message Firmware message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Firmware.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.version != null && message.hasOwnProperty("version"))
                    writer.uint32(/* id 1, wireType 2 =*/10).string(message.version);
                if (message.build != null && message.hasOwnProperty("build"))
                    writer.uint32(/* id 2, wireType 2 =*/18).string(message.build);
                if (message.number != null && message.hasOwnProperty("number"))
                    writer.uint32(/* id 3, wireType 2 =*/26).string(message.number);
                if (message.timestamp != null && message.hasOwnProperty("timestamp"))
                    writer.uint32(/* id 4, wireType 0 =*/32).uint64(message.timestamp);
                if (message.hash != null && message.hasOwnProperty("hash"))
                    writer.uint32(/* id 5, wireType 2 =*/42).string(message.hash);
                if (message.logicalAddress != null && message.hasOwnProperty("logicalAddress"))
                    writer.uint32(/* id 6, wireType 0 =*/48).uint64(message.logicalAddress);
                if (message.name != null && message.hasOwnProperty("name"))
                    writer.uint32(/* id 7, wireType 2 =*/58).string(message.name);
                return writer;
            };
    
            /**
             * Encodes the specified Firmware message, length delimited. Does not implicitly {@link fk_app.Firmware.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.Firmware
             * @static
             * @param {fk_app.IFirmware} message Firmware message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Firmware.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a Firmware message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.Firmware
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.Firmware} Firmware
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Firmware.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.Firmware();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.version = reader.string();
                        break;
                    case 2:
                        message.build = reader.string();
                        break;
                    case 3:
                        message.number = reader.string();
                        break;
                    case 4:
                        message.timestamp = reader.uint64();
                        break;
                    case 5:
                        message.hash = reader.string();
                        break;
                    case 6:
                        message.logicalAddress = reader.uint64();
                        break;
                    case 7:
                        message.name = reader.string();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a Firmware message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.Firmware
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.Firmware} Firmware
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Firmware.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a Firmware message.
             * @function verify
             * @memberof fk_app.Firmware
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            Firmware.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.version != null && message.hasOwnProperty("version"))
                    if (!$util.isString(message.version))
                        return "version: string expected";
                if (message.build != null && message.hasOwnProperty("build"))
                    if (!$util.isString(message.build))
                        return "build: string expected";
                if (message.number != null && message.hasOwnProperty("number"))
                    if (!$util.isString(message.number))
                        return "number: string expected";
                if (message.timestamp != null && message.hasOwnProperty("timestamp"))
                    if (!$util.isInteger(message.timestamp) && !(message.timestamp && $util.isInteger(message.timestamp.low) && $util.isInteger(message.timestamp.high)))
                        return "timestamp: integer|Long expected";
                if (message.hash != null && message.hasOwnProperty("hash"))
                    if (!$util.isString(message.hash))
                        return "hash: string expected";
                if (message.logicalAddress != null && message.hasOwnProperty("logicalAddress"))
                    if (!$util.isInteger(message.logicalAddress) && !(message.logicalAddress && $util.isInteger(message.logicalAddress.low) && $util.isInteger(message.logicalAddress.high)))
                        return "logicalAddress: integer|Long expected";
                if (message.name != null && message.hasOwnProperty("name"))
                    if (!$util.isString(message.name))
                        return "name: string expected";
                return null;
            };
    
            /**
             * Creates a Firmware message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.Firmware
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.Firmware} Firmware
             */
            Firmware.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.Firmware)
                    return object;
                var message = new $root.fk_app.Firmware();
                if (object.version != null)
                    message.version = String(object.version);
                if (object.build != null)
                    message.build = String(object.build);
                if (object.number != null)
                    message.number = String(object.number);
                if (object.timestamp != null)
                    if ($util.Long)
                        (message.timestamp = $util.Long.fromValue(object.timestamp)).unsigned = true;
                    else if (typeof object.timestamp === "string")
                        message.timestamp = parseInt(object.timestamp, 10);
                    else if (typeof object.timestamp === "number")
                        message.timestamp = object.timestamp;
                    else if (typeof object.timestamp === "object")
                        message.timestamp = new $util.LongBits(object.timestamp.low >>> 0, object.timestamp.high >>> 0).toNumber(true);
                if (object.hash != null)
                    message.hash = String(object.hash);
                if (object.logicalAddress != null)
                    if ($util.Long)
                        (message.logicalAddress = $util.Long.fromValue(object.logicalAddress)).unsigned = true;
                    else if (typeof object.logicalAddress === "string")
                        message.logicalAddress = parseInt(object.logicalAddress, 10);
                    else if (typeof object.logicalAddress === "number")
                        message.logicalAddress = object.logicalAddress;
                    else if (typeof object.logicalAddress === "object")
                        message.logicalAddress = new $util.LongBits(object.logicalAddress.low >>> 0, object.logicalAddress.high >>> 0).toNumber(true);
                if (object.name != null)
                    message.name = String(object.name);
                return message;
            };
    
            /**
             * Creates a plain object from a Firmware message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.Firmware
             * @static
             * @param {fk_app.Firmware} message Firmware
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            Firmware.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.version = "";
                    object.build = "";
                    object.number = "";
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.timestamp = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.timestamp = options.longs === String ? "0" : 0;
                    object.hash = "";
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.logicalAddress = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.logicalAddress = options.longs === String ? "0" : 0;
                    object.name = "";
                }
                if (message.version != null && message.hasOwnProperty("version"))
                    object.version = message.version;
                if (message.build != null && message.hasOwnProperty("build"))
                    object.build = message.build;
                if (message.number != null && message.hasOwnProperty("number"))
                    object.number = message.number;
                if (message.timestamp != null && message.hasOwnProperty("timestamp"))
                    if (typeof message.timestamp === "number")
                        object.timestamp = options.longs === String ? String(message.timestamp) : message.timestamp;
                    else
                        object.timestamp = options.longs === String ? $util.Long.prototype.toString.call(message.timestamp) : options.longs === Number ? new $util.LongBits(message.timestamp.low >>> 0, message.timestamp.high >>> 0).toNumber(true) : message.timestamp;
                if (message.hash != null && message.hasOwnProperty("hash"))
                    object.hash = message.hash;
                if (message.logicalAddress != null && message.hasOwnProperty("logicalAddress"))
                    if (typeof message.logicalAddress === "number")
                        object.logicalAddress = options.longs === String ? String(message.logicalAddress) : message.logicalAddress;
                    else
                        object.logicalAddress = options.longs === String ? $util.Long.prototype.toString.call(message.logicalAddress) : options.longs === Number ? new $util.LongBits(message.logicalAddress.low >>> 0, message.logicalAddress.high >>> 0).toNumber(true) : message.logicalAddress;
                if (message.name != null && message.hasOwnProperty("name"))
                    object.name = message.name;
                return object;
            };
    
            /**
             * Converts this Firmware to JSON.
             * @function toJSON
             * @memberof fk_app.Firmware
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Firmware.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Firmware;
        })();
    
        fk_app.Identity = (function() {
    
            /**
             * Properties of an Identity.
             * @memberof fk_app
             * @interface IIdentity
             * @property {string} [device] Identity device
             * @property {string} [stream] Identity stream
             * @property {Uint8Array} [deviceId] Identity deviceId
             * @property {string} [firmware] Identity firmware
             * @property {string} [build] Identity build
             * @property {string} [number] Identity number
             * @property {string} [name] Identity name
             * @property {Uint8Array} [generationId] Identity generationId
             */
    
            /**
             * Constructs a new Identity.
             * @memberof fk_app
             * @classdesc Represents an Identity.
             * @constructor
             * @param {fk_app.IIdentity=} [properties] Properties to set
             */
            function Identity(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Identity device.
             * @member {string}device
             * @memberof fk_app.Identity
             * @instance
             */
            Identity.prototype.device = "";
    
            /**
             * Identity stream.
             * @member {string}stream
             * @memberof fk_app.Identity
             * @instance
             */
            Identity.prototype.stream = "";
    
            /**
             * Identity deviceId.
             * @member {Uint8Array}deviceId
             * @memberof fk_app.Identity
             * @instance
             */
            Identity.prototype.deviceId = $util.newBuffer([]);
    
            /**
             * Identity firmware.
             * @member {string}firmware
             * @memberof fk_app.Identity
             * @instance
             */
            Identity.prototype.firmware = "";
    
            /**
             * Identity build.
             * @member {string}build
             * @memberof fk_app.Identity
             * @instance
             */
            Identity.prototype.build = "";
    
            /**
             * Identity number.
             * @member {string}number
             * @memberof fk_app.Identity
             * @instance
             */
            Identity.prototype.number = "";
    
            /**
             * Identity name.
             * @member {string}name
             * @memberof fk_app.Identity
             * @instance
             */
            Identity.prototype.name = "";
    
            /**
             * Identity generationId.
             * @member {Uint8Array}generationId
             * @memberof fk_app.Identity
             * @instance
             */
            Identity.prototype.generationId = $util.newBuffer([]);
    
            /**
             * Creates a new Identity instance using the specified properties.
             * @function create
             * @memberof fk_app.Identity
             * @static
             * @param {fk_app.IIdentity=} [properties] Properties to set
             * @returns {fk_app.Identity} Identity instance
             */
            Identity.create = function create(properties) {
                return new Identity(properties);
            };
    
            /**
             * Encodes the specified Identity message. Does not implicitly {@link fk_app.Identity.verify|verify} messages.
             * @function encode
             * @memberof fk_app.Identity
             * @static
             * @param {fk_app.IIdentity} message Identity message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Identity.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.device != null && message.hasOwnProperty("device"))
                    writer.uint32(/* id 1, wireType 2 =*/10).string(message.device);
                if (message.stream != null && message.hasOwnProperty("stream"))
                    writer.uint32(/* id 2, wireType 2 =*/18).string(message.stream);
                if (message.deviceId != null && message.hasOwnProperty("deviceId"))
                    writer.uint32(/* id 3, wireType 2 =*/26).bytes(message.deviceId);
                if (message.firmware != null && message.hasOwnProperty("firmware"))
                    writer.uint32(/* id 4, wireType 2 =*/34).string(message.firmware);
                if (message.build != null && message.hasOwnProperty("build"))
                    writer.uint32(/* id 5, wireType 2 =*/42).string(message.build);
                if (message.name != null && message.hasOwnProperty("name"))
                    writer.uint32(/* id 6, wireType 2 =*/50).string(message.name);
                if (message.generationId != null && message.hasOwnProperty("generationId"))
                    writer.uint32(/* id 7, wireType 2 =*/58).bytes(message.generationId);
                if (message.number != null && message.hasOwnProperty("number"))
                    writer.uint32(/* id 8, wireType 2 =*/66).string(message.number);
                return writer;
            };
    
            /**
             * Encodes the specified Identity message, length delimited. Does not implicitly {@link fk_app.Identity.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.Identity
             * @static
             * @param {fk_app.IIdentity} message Identity message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Identity.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes an Identity message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.Identity
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.Identity} Identity
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Identity.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.Identity();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.device = reader.string();
                        break;
                    case 2:
                        message.stream = reader.string();
                        break;
                    case 3:
                        message.deviceId = reader.bytes();
                        break;
                    case 4:
                        message.firmware = reader.string();
                        break;
                    case 5:
                        message.build = reader.string();
                        break;
                    case 8:
                        message.number = reader.string();
                        break;
                    case 6:
                        message.name = reader.string();
                        break;
                    case 7:
                        message.generationId = reader.bytes();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes an Identity message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.Identity
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.Identity} Identity
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Identity.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies an Identity message.
             * @function verify
             * @memberof fk_app.Identity
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            Identity.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.device != null && message.hasOwnProperty("device"))
                    if (!$util.isString(message.device))
                        return "device: string expected";
                if (message.stream != null && message.hasOwnProperty("stream"))
                    if (!$util.isString(message.stream))
                        return "stream: string expected";
                if (message.deviceId != null && message.hasOwnProperty("deviceId"))
                    if (!(message.deviceId && typeof message.deviceId.length === "number" || $util.isString(message.deviceId)))
                        return "deviceId: buffer expected";
                if (message.firmware != null && message.hasOwnProperty("firmware"))
                    if (!$util.isString(message.firmware))
                        return "firmware: string expected";
                if (message.build != null && message.hasOwnProperty("build"))
                    if (!$util.isString(message.build))
                        return "build: string expected";
                if (message.number != null && message.hasOwnProperty("number"))
                    if (!$util.isString(message.number))
                        return "number: string expected";
                if (message.name != null && message.hasOwnProperty("name"))
                    if (!$util.isString(message.name))
                        return "name: string expected";
                if (message.generationId != null && message.hasOwnProperty("generationId"))
                    if (!(message.generationId && typeof message.generationId.length === "number" || $util.isString(message.generationId)))
                        return "generationId: buffer expected";
                return null;
            };
    
            /**
             * Creates an Identity message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.Identity
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.Identity} Identity
             */
            Identity.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.Identity)
                    return object;
                var message = new $root.fk_app.Identity();
                if (object.device != null)
                    message.device = String(object.device);
                if (object.stream != null)
                    message.stream = String(object.stream);
                if (object.deviceId != null)
                    if (typeof object.deviceId === "string")
                        $util.base64.decode(object.deviceId, message.deviceId = $util.newBuffer($util.base64.length(object.deviceId)), 0);
                    else if (object.deviceId.length)
                        message.deviceId = object.deviceId;
                if (object.firmware != null)
                    message.firmware = String(object.firmware);
                if (object.build != null)
                    message.build = String(object.build);
                if (object.number != null)
                    message.number = String(object.number);
                if (object.name != null)
                    message.name = String(object.name);
                if (object.generationId != null)
                    if (typeof object.generationId === "string")
                        $util.base64.decode(object.generationId, message.generationId = $util.newBuffer($util.base64.length(object.generationId)), 0);
                    else if (object.generationId.length)
                        message.generationId = object.generationId;
                return message;
            };
    
            /**
             * Creates a plain object from an Identity message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.Identity
             * @static
             * @param {fk_app.Identity} message Identity
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            Identity.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.device = "";
                    object.stream = "";
                    object.deviceId = options.bytes === String ? "" : [];
                    object.firmware = "";
                    object.build = "";
                    object.name = "";
                    object.generationId = options.bytes === String ? "" : [];
                    object.number = "";
                }
                if (message.device != null && message.hasOwnProperty("device"))
                    object.device = message.device;
                if (message.stream != null && message.hasOwnProperty("stream"))
                    object.stream = message.stream;
                if (message.deviceId != null && message.hasOwnProperty("deviceId"))
                    object.deviceId = options.bytes === String ? $util.base64.encode(message.deviceId, 0, message.deviceId.length) : options.bytes === Array ? Array.prototype.slice.call(message.deviceId) : message.deviceId;
                if (message.firmware != null && message.hasOwnProperty("firmware"))
                    object.firmware = message.firmware;
                if (message.build != null && message.hasOwnProperty("build"))
                    object.build = message.build;
                if (message.name != null && message.hasOwnProperty("name"))
                    object.name = message.name;
                if (message.generationId != null && message.hasOwnProperty("generationId"))
                    object.generationId = options.bytes === String ? $util.base64.encode(message.generationId, 0, message.generationId.length) : options.bytes === Array ? Array.prototype.slice.call(message.generationId) : message.generationId;
                if (message.number != null && message.hasOwnProperty("number"))
                    object.number = message.number;
                return object;
            };
    
            /**
             * Converts this Identity to JSON.
             * @function toJSON
             * @memberof fk_app.Identity
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Identity.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Identity;
        })();
    
        fk_app.ConfigureSensorQuery = (function() {
    
            /**
             * Properties of a ConfigureSensorQuery.
             * @memberof fk_app
             * @interface IConfigureSensorQuery
             * @property {number} [id] ConfigureSensorQuery id
             * @property {number} [frequency] ConfigureSensorQuery frequency
             */
    
            /**
             * Constructs a new ConfigureSensorQuery.
             * @memberof fk_app
             * @classdesc Represents a ConfigureSensorQuery.
             * @constructor
             * @param {fk_app.IConfigureSensorQuery=} [properties] Properties to set
             */
            function ConfigureSensorQuery(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * ConfigureSensorQuery id.
             * @member {number}id
             * @memberof fk_app.ConfigureSensorQuery
             * @instance
             */
            ConfigureSensorQuery.prototype.id = 0;
    
            /**
             * ConfigureSensorQuery frequency.
             * @member {number}frequency
             * @memberof fk_app.ConfigureSensorQuery
             * @instance
             */
            ConfigureSensorQuery.prototype.frequency = 0;
    
            /**
             * Creates a new ConfigureSensorQuery instance using the specified properties.
             * @function create
             * @memberof fk_app.ConfigureSensorQuery
             * @static
             * @param {fk_app.IConfigureSensorQuery=} [properties] Properties to set
             * @returns {fk_app.ConfigureSensorQuery} ConfigureSensorQuery instance
             */
            ConfigureSensorQuery.create = function create(properties) {
                return new ConfigureSensorQuery(properties);
            };
    
            /**
             * Encodes the specified ConfigureSensorQuery message. Does not implicitly {@link fk_app.ConfigureSensorQuery.verify|verify} messages.
             * @function encode
             * @memberof fk_app.ConfigureSensorQuery
             * @static
             * @param {fk_app.IConfigureSensorQuery} message ConfigureSensorQuery message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            ConfigureSensorQuery.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.id != null && message.hasOwnProperty("id"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.id);
                if (message.frequency != null && message.hasOwnProperty("frequency"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint32(message.frequency);
                return writer;
            };
    
            /**
             * Encodes the specified ConfigureSensorQuery message, length delimited. Does not implicitly {@link fk_app.ConfigureSensorQuery.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.ConfigureSensorQuery
             * @static
             * @param {fk_app.IConfigureSensorQuery} message ConfigureSensorQuery message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            ConfigureSensorQuery.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a ConfigureSensorQuery message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.ConfigureSensorQuery
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.ConfigureSensorQuery} ConfigureSensorQuery
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            ConfigureSensorQuery.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.ConfigureSensorQuery();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.id = reader.uint32();
                        break;
                    case 2:
                        message.frequency = reader.uint32();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a ConfigureSensorQuery message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.ConfigureSensorQuery
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.ConfigureSensorQuery} ConfigureSensorQuery
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            ConfigureSensorQuery.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a ConfigureSensorQuery message.
             * @function verify
             * @memberof fk_app.ConfigureSensorQuery
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            ConfigureSensorQuery.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.id != null && message.hasOwnProperty("id"))
                    if (!$util.isInteger(message.id))
                        return "id: integer expected";
                if (message.frequency != null && message.hasOwnProperty("frequency"))
                    if (!$util.isInteger(message.frequency))
                        return "frequency: integer expected";
                return null;
            };
    
            /**
             * Creates a ConfigureSensorQuery message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.ConfigureSensorQuery
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.ConfigureSensorQuery} ConfigureSensorQuery
             */
            ConfigureSensorQuery.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.ConfigureSensorQuery)
                    return object;
                var message = new $root.fk_app.ConfigureSensorQuery();
                if (object.id != null)
                    message.id = object.id >>> 0;
                if (object.frequency != null)
                    message.frequency = object.frequency >>> 0;
                return message;
            };
    
            /**
             * Creates a plain object from a ConfigureSensorQuery message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.ConfigureSensorQuery
             * @static
             * @param {fk_app.ConfigureSensorQuery} message ConfigureSensorQuery
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            ConfigureSensorQuery.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.id = 0;
                    object.frequency = 0;
                }
                if (message.id != null && message.hasOwnProperty("id"))
                    object.id = message.id;
                if (message.frequency != null && message.hasOwnProperty("frequency"))
                    object.frequency = message.frequency;
                return object;
            };
    
            /**
             * Converts this ConfigureSensorQuery to JSON.
             * @function toJSON
             * @memberof fk_app.ConfigureSensorQuery
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            ConfigureSensorQuery.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return ConfigureSensorQuery;
        })();
    
        fk_app.LiveDataPoll = (function() {
    
            /**
             * Properties of a LiveDataPoll.
             * @memberof fk_app
             * @interface ILiveDataPoll
             * @property {number} [interval] LiveDataPoll interval
             */
    
            /**
             * Constructs a new LiveDataPoll.
             * @memberof fk_app
             * @classdesc Represents a LiveDataPoll.
             * @constructor
             * @param {fk_app.ILiveDataPoll=} [properties] Properties to set
             */
            function LiveDataPoll(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * LiveDataPoll interval.
             * @member {number}interval
             * @memberof fk_app.LiveDataPoll
             * @instance
             */
            LiveDataPoll.prototype.interval = 0;
    
            /**
             * Creates a new LiveDataPoll instance using the specified properties.
             * @function create
             * @memberof fk_app.LiveDataPoll
             * @static
             * @param {fk_app.ILiveDataPoll=} [properties] Properties to set
             * @returns {fk_app.LiveDataPoll} LiveDataPoll instance
             */
            LiveDataPoll.create = function create(properties) {
                return new LiveDataPoll(properties);
            };
    
            /**
             * Encodes the specified LiveDataPoll message. Does not implicitly {@link fk_app.LiveDataPoll.verify|verify} messages.
             * @function encode
             * @memberof fk_app.LiveDataPoll
             * @static
             * @param {fk_app.ILiveDataPoll} message LiveDataPoll message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            LiveDataPoll.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.interval != null && message.hasOwnProperty("interval"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.interval);
                return writer;
            };
    
            /**
             * Encodes the specified LiveDataPoll message, length delimited. Does not implicitly {@link fk_app.LiveDataPoll.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.LiveDataPoll
             * @static
             * @param {fk_app.ILiveDataPoll} message LiveDataPoll message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            LiveDataPoll.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a LiveDataPoll message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.LiveDataPoll
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.LiveDataPoll} LiveDataPoll
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            LiveDataPoll.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.LiveDataPoll();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.interval = reader.uint32();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a LiveDataPoll message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.LiveDataPoll
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.LiveDataPoll} LiveDataPoll
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            LiveDataPoll.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a LiveDataPoll message.
             * @function verify
             * @memberof fk_app.LiveDataPoll
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            LiveDataPoll.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.interval != null && message.hasOwnProperty("interval"))
                    if (!$util.isInteger(message.interval))
                        return "interval: integer expected";
                return null;
            };
    
            /**
             * Creates a LiveDataPoll message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.LiveDataPoll
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.LiveDataPoll} LiveDataPoll
             */
            LiveDataPoll.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.LiveDataPoll)
                    return object;
                var message = new $root.fk_app.LiveDataPoll();
                if (object.interval != null)
                    message.interval = object.interval >>> 0;
                return message;
            };
    
            /**
             * Creates a plain object from a LiveDataPoll message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.LiveDataPoll
             * @static
             * @param {fk_app.LiveDataPoll} message LiveDataPoll
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            LiveDataPoll.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults)
                    object.interval = 0;
                if (message.interval != null && message.hasOwnProperty("interval"))
                    object.interval = message.interval;
                return object;
            };
    
            /**
             * Converts this LiveDataPoll to JSON.
             * @function toJSON
             * @memberof fk_app.LiveDataPoll
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            LiveDataPoll.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return LiveDataPoll;
        })();
    
        fk_app.LiveDataSample = (function() {
    
            /**
             * Properties of a LiveDataSample.
             * @memberof fk_app
             * @interface ILiveDataSample
             * @property {number} [sensor] LiveDataSample sensor
             * @property {number|Long} [time] LiveDataSample time
             * @property {number} [value] LiveDataSample value
             */
    
            /**
             * Constructs a new LiveDataSample.
             * @memberof fk_app
             * @classdesc Represents a LiveDataSample.
             * @constructor
             * @param {fk_app.ILiveDataSample=} [properties] Properties to set
             */
            function LiveDataSample(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * LiveDataSample sensor.
             * @member {number}sensor
             * @memberof fk_app.LiveDataSample
             * @instance
             */
            LiveDataSample.prototype.sensor = 0;
    
            /**
             * LiveDataSample time.
             * @member {number|Long}time
             * @memberof fk_app.LiveDataSample
             * @instance
             */
            LiveDataSample.prototype.time = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * LiveDataSample value.
             * @member {number}value
             * @memberof fk_app.LiveDataSample
             * @instance
             */
            LiveDataSample.prototype.value = 0;
    
            /**
             * Creates a new LiveDataSample instance using the specified properties.
             * @function create
             * @memberof fk_app.LiveDataSample
             * @static
             * @param {fk_app.ILiveDataSample=} [properties] Properties to set
             * @returns {fk_app.LiveDataSample} LiveDataSample instance
             */
            LiveDataSample.create = function create(properties) {
                return new LiveDataSample(properties);
            };
    
            /**
             * Encodes the specified LiveDataSample message. Does not implicitly {@link fk_app.LiveDataSample.verify|verify} messages.
             * @function encode
             * @memberof fk_app.LiveDataSample
             * @static
             * @param {fk_app.ILiveDataSample} message LiveDataSample message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            LiveDataSample.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.sensor != null && message.hasOwnProperty("sensor"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.sensor);
                if (message.time != null && message.hasOwnProperty("time"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint64(message.time);
                if (message.value != null && message.hasOwnProperty("value"))
                    writer.uint32(/* id 3, wireType 5 =*/29).float(message.value);
                return writer;
            };
    
            /**
             * Encodes the specified LiveDataSample message, length delimited. Does not implicitly {@link fk_app.LiveDataSample.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.LiveDataSample
             * @static
             * @param {fk_app.ILiveDataSample} message LiveDataSample message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            LiveDataSample.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a LiveDataSample message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.LiveDataSample
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.LiveDataSample} LiveDataSample
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            LiveDataSample.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.LiveDataSample();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.sensor = reader.uint32();
                        break;
                    case 2:
                        message.time = reader.uint64();
                        break;
                    case 3:
                        message.value = reader.float();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a LiveDataSample message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.LiveDataSample
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.LiveDataSample} LiveDataSample
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            LiveDataSample.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a LiveDataSample message.
             * @function verify
             * @memberof fk_app.LiveDataSample
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            LiveDataSample.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.sensor != null && message.hasOwnProperty("sensor"))
                    if (!$util.isInteger(message.sensor))
                        return "sensor: integer expected";
                if (message.time != null && message.hasOwnProperty("time"))
                    if (!$util.isInteger(message.time) && !(message.time && $util.isInteger(message.time.low) && $util.isInteger(message.time.high)))
                        return "time: integer|Long expected";
                if (message.value != null && message.hasOwnProperty("value"))
                    if (typeof message.value !== "number")
                        return "value: number expected";
                return null;
            };
    
            /**
             * Creates a LiveDataSample message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.LiveDataSample
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.LiveDataSample} LiveDataSample
             */
            LiveDataSample.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.LiveDataSample)
                    return object;
                var message = new $root.fk_app.LiveDataSample();
                if (object.sensor != null)
                    message.sensor = object.sensor >>> 0;
                if (object.time != null)
                    if ($util.Long)
                        (message.time = $util.Long.fromValue(object.time)).unsigned = true;
                    else if (typeof object.time === "string")
                        message.time = parseInt(object.time, 10);
                    else if (typeof object.time === "number")
                        message.time = object.time;
                    else if (typeof object.time === "object")
                        message.time = new $util.LongBits(object.time.low >>> 0, object.time.high >>> 0).toNumber(true);
                if (object.value != null)
                    message.value = Number(object.value);
                return message;
            };
    
            /**
             * Creates a plain object from a LiveDataSample message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.LiveDataSample
             * @static
             * @param {fk_app.LiveDataSample} message LiveDataSample
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            LiveDataSample.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.sensor = 0;
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.time = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.time = options.longs === String ? "0" : 0;
                    object.value = 0;
                }
                if (message.sensor != null && message.hasOwnProperty("sensor"))
                    object.sensor = message.sensor;
                if (message.time != null && message.hasOwnProperty("time"))
                    if (typeof message.time === "number")
                        object.time = options.longs === String ? String(message.time) : message.time;
                    else
                        object.time = options.longs === String ? $util.Long.prototype.toString.call(message.time) : options.longs === Number ? new $util.LongBits(message.time.low >>> 0, message.time.high >>> 0).toNumber(true) : message.time;
                if (message.value != null && message.hasOwnProperty("value"))
                    object.value = options.json && !isFinite(message.value) ? String(message.value) : message.value;
                return object;
            };
    
            /**
             * Converts this LiveDataSample to JSON.
             * @function toJSON
             * @memberof fk_app.LiveDataSample
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            LiveDataSample.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return LiveDataSample;
        })();
    
        fk_app.LiveData = (function() {
    
            /**
             * Properties of a LiveData.
             * @memberof fk_app
             * @interface ILiveData
             * @property {Array.<fk_app.ILiveDataSample>} [samples] LiveData samples
             */
    
            /**
             * Constructs a new LiveData.
             * @memberof fk_app
             * @classdesc Represents a LiveData.
             * @constructor
             * @param {fk_app.ILiveData=} [properties] Properties to set
             */
            function LiveData(properties) {
                this.samples = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * LiveData samples.
             * @member {Array.<fk_app.ILiveDataSample>}samples
             * @memberof fk_app.LiveData
             * @instance
             */
            LiveData.prototype.samples = $util.emptyArray;
    
            /**
             * Creates a new LiveData instance using the specified properties.
             * @function create
             * @memberof fk_app.LiveData
             * @static
             * @param {fk_app.ILiveData=} [properties] Properties to set
             * @returns {fk_app.LiveData} LiveData instance
             */
            LiveData.create = function create(properties) {
                return new LiveData(properties);
            };
    
            /**
             * Encodes the specified LiveData message. Does not implicitly {@link fk_app.LiveData.verify|verify} messages.
             * @function encode
             * @memberof fk_app.LiveData
             * @static
             * @param {fk_app.ILiveData} message LiveData message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            LiveData.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.samples != null && message.samples.length)
                    for (var i = 0; i < message.samples.length; ++i)
                        $root.fk_app.LiveDataSample.encode(message.samples[i], writer.uint32(/* id 1, wireType 2 =*/10).fork()).ldelim();
                return writer;
            };
    
            /**
             * Encodes the specified LiveData message, length delimited. Does not implicitly {@link fk_app.LiveData.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.LiveData
             * @static
             * @param {fk_app.ILiveData} message LiveData message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            LiveData.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a LiveData message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.LiveData
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.LiveData} LiveData
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            LiveData.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.LiveData();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        if (!(message.samples && message.samples.length))
                            message.samples = [];
                        message.samples.push($root.fk_app.LiveDataSample.decode(reader, reader.uint32()));
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a LiveData message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.LiveData
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.LiveData} LiveData
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            LiveData.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a LiveData message.
             * @function verify
             * @memberof fk_app.LiveData
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            LiveData.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.samples != null && message.hasOwnProperty("samples")) {
                    if (!Array.isArray(message.samples))
                        return "samples: array expected";
                    for (var i = 0; i < message.samples.length; ++i) {
                        var error = $root.fk_app.LiveDataSample.verify(message.samples[i]);
                        if (error)
                            return "samples." + error;
                    }
                }
                return null;
            };
    
            /**
             * Creates a LiveData message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.LiveData
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.LiveData} LiveData
             */
            LiveData.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.LiveData)
                    return object;
                var message = new $root.fk_app.LiveData();
                if (object.samples) {
                    if (!Array.isArray(object.samples))
                        throw TypeError(".fk_app.LiveData.samples: array expected");
                    message.samples = [];
                    for (var i = 0; i < object.samples.length; ++i) {
                        if (typeof object.samples[i] !== "object")
                            throw TypeError(".fk_app.LiveData.samples: object expected");
                        message.samples[i] = $root.fk_app.LiveDataSample.fromObject(object.samples[i]);
                    }
                }
                return message;
            };
    
            /**
             * Creates a plain object from a LiveData message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.LiveData
             * @static
             * @param {fk_app.LiveData} message LiveData
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            LiveData.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults)
                    object.samples = [];
                if (message.samples && message.samples.length) {
                    object.samples = [];
                    for (var j = 0; j < message.samples.length; ++j)
                        object.samples[j] = $root.fk_app.LiveDataSample.toObject(message.samples[j], options);
                }
                return object;
            };
    
            /**
             * Converts this LiveData to JSON.
             * @function toJSON
             * @memberof fk_app.LiveData
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            LiveData.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return LiveData;
        })();
    
        fk_app.File = (function() {
    
            /**
             * Properties of a File.
             * @memberof fk_app
             * @interface IFile
             * @property {number} [id] File id
             * @property {number|Long} [time] File time
             * @property {number|Long} [size] File size
             * @property {number} [version] File version
             * @property {string} [name] File name
             * @property {number|Long} [maximum] File maximum
             */
    
            /**
             * Constructs a new File.
             * @memberof fk_app
             * @classdesc Represents a File.
             * @constructor
             * @param {fk_app.IFile=} [properties] Properties to set
             */
            function File(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * File id.
             * @member {number}id
             * @memberof fk_app.File
             * @instance
             */
            File.prototype.id = 0;
    
            /**
             * File time.
             * @member {number|Long}time
             * @memberof fk_app.File
             * @instance
             */
            File.prototype.time = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * File size.
             * @member {number|Long}size
             * @memberof fk_app.File
             * @instance
             */
            File.prototype.size = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * File version.
             * @member {number}version
             * @memberof fk_app.File
             * @instance
             */
            File.prototype.version = 0;
    
            /**
             * File name.
             * @member {string}name
             * @memberof fk_app.File
             * @instance
             */
            File.prototype.name = "";
    
            /**
             * File maximum.
             * @member {number|Long}maximum
             * @memberof fk_app.File
             * @instance
             */
            File.prototype.maximum = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * Creates a new File instance using the specified properties.
             * @function create
             * @memberof fk_app.File
             * @static
             * @param {fk_app.IFile=} [properties] Properties to set
             * @returns {fk_app.File} File instance
             */
            File.create = function create(properties) {
                return new File(properties);
            };
    
            /**
             * Encodes the specified File message. Does not implicitly {@link fk_app.File.verify|verify} messages.
             * @function encode
             * @memberof fk_app.File
             * @static
             * @param {fk_app.IFile} message File message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            File.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.id != null && message.hasOwnProperty("id"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.id);
                if (message.time != null && message.hasOwnProperty("time"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint64(message.time);
                if (message.size != null && message.hasOwnProperty("size"))
                    writer.uint32(/* id 3, wireType 0 =*/24).uint64(message.size);
                if (message.version != null && message.hasOwnProperty("version"))
                    writer.uint32(/* id 4, wireType 0 =*/32).uint32(message.version);
                if (message.name != null && message.hasOwnProperty("name"))
                    writer.uint32(/* id 5, wireType 2 =*/42).string(message.name);
                if (message.maximum != null && message.hasOwnProperty("maximum"))
                    writer.uint32(/* id 6, wireType 0 =*/48).uint64(message.maximum);
                return writer;
            };
    
            /**
             * Encodes the specified File message, length delimited. Does not implicitly {@link fk_app.File.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.File
             * @static
             * @param {fk_app.IFile} message File message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            File.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a File message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.File
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.File} File
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            File.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.File();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.id = reader.uint32();
                        break;
                    case 2:
                        message.time = reader.uint64();
                        break;
                    case 3:
                        message.size = reader.uint64();
                        break;
                    case 4:
                        message.version = reader.uint32();
                        break;
                    case 5:
                        message.name = reader.string();
                        break;
                    case 6:
                        message.maximum = reader.uint64();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a File message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.File
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.File} File
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            File.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a File message.
             * @function verify
             * @memberof fk_app.File
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            File.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.id != null && message.hasOwnProperty("id"))
                    if (!$util.isInteger(message.id))
                        return "id: integer expected";
                if (message.time != null && message.hasOwnProperty("time"))
                    if (!$util.isInteger(message.time) && !(message.time && $util.isInteger(message.time.low) && $util.isInteger(message.time.high)))
                        return "time: integer|Long expected";
                if (message.size != null && message.hasOwnProperty("size"))
                    if (!$util.isInteger(message.size) && !(message.size && $util.isInteger(message.size.low) && $util.isInteger(message.size.high)))
                        return "size: integer|Long expected";
                if (message.version != null && message.hasOwnProperty("version"))
                    if (!$util.isInteger(message.version))
                        return "version: integer expected";
                if (message.name != null && message.hasOwnProperty("name"))
                    if (!$util.isString(message.name))
                        return "name: string expected";
                if (message.maximum != null && message.hasOwnProperty("maximum"))
                    if (!$util.isInteger(message.maximum) && !(message.maximum && $util.isInteger(message.maximum.low) && $util.isInteger(message.maximum.high)))
                        return "maximum: integer|Long expected";
                return null;
            };
    
            /**
             * Creates a File message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.File
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.File} File
             */
            File.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.File)
                    return object;
                var message = new $root.fk_app.File();
                if (object.id != null)
                    message.id = object.id >>> 0;
                if (object.time != null)
                    if ($util.Long)
                        (message.time = $util.Long.fromValue(object.time)).unsigned = true;
                    else if (typeof object.time === "string")
                        message.time = parseInt(object.time, 10);
                    else if (typeof object.time === "number")
                        message.time = object.time;
                    else if (typeof object.time === "object")
                        message.time = new $util.LongBits(object.time.low >>> 0, object.time.high >>> 0).toNumber(true);
                if (object.size != null)
                    if ($util.Long)
                        (message.size = $util.Long.fromValue(object.size)).unsigned = true;
                    else if (typeof object.size === "string")
                        message.size = parseInt(object.size, 10);
                    else if (typeof object.size === "number")
                        message.size = object.size;
                    else if (typeof object.size === "object")
                        message.size = new $util.LongBits(object.size.low >>> 0, object.size.high >>> 0).toNumber(true);
                if (object.version != null)
                    message.version = object.version >>> 0;
                if (object.name != null)
                    message.name = String(object.name);
                if (object.maximum != null)
                    if ($util.Long)
                        (message.maximum = $util.Long.fromValue(object.maximum)).unsigned = true;
                    else if (typeof object.maximum === "string")
                        message.maximum = parseInt(object.maximum, 10);
                    else if (typeof object.maximum === "number")
                        message.maximum = object.maximum;
                    else if (typeof object.maximum === "object")
                        message.maximum = new $util.LongBits(object.maximum.low >>> 0, object.maximum.high >>> 0).toNumber(true);
                return message;
            };
    
            /**
             * Creates a plain object from a File message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.File
             * @static
             * @param {fk_app.File} message File
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            File.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.id = 0;
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.time = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.time = options.longs === String ? "0" : 0;
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.size = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.size = options.longs === String ? "0" : 0;
                    object.version = 0;
                    object.name = "";
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.maximum = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.maximum = options.longs === String ? "0" : 0;
                }
                if (message.id != null && message.hasOwnProperty("id"))
                    object.id = message.id;
                if (message.time != null && message.hasOwnProperty("time"))
                    if (typeof message.time === "number")
                        object.time = options.longs === String ? String(message.time) : message.time;
                    else
                        object.time = options.longs === String ? $util.Long.prototype.toString.call(message.time) : options.longs === Number ? new $util.LongBits(message.time.low >>> 0, message.time.high >>> 0).toNumber(true) : message.time;
                if (message.size != null && message.hasOwnProperty("size"))
                    if (typeof message.size === "number")
                        object.size = options.longs === String ? String(message.size) : message.size;
                    else
                        object.size = options.longs === String ? $util.Long.prototype.toString.call(message.size) : options.longs === Number ? new $util.LongBits(message.size.low >>> 0, message.size.high >>> 0).toNumber(true) : message.size;
                if (message.version != null && message.hasOwnProperty("version"))
                    object.version = message.version;
                if (message.name != null && message.hasOwnProperty("name"))
                    object.name = message.name;
                if (message.maximum != null && message.hasOwnProperty("maximum"))
                    if (typeof message.maximum === "number")
                        object.maximum = options.longs === String ? String(message.maximum) : message.maximum;
                    else
                        object.maximum = options.longs === String ? $util.Long.prototype.toString.call(message.maximum) : options.longs === Number ? new $util.LongBits(message.maximum.low >>> 0, message.maximum.high >>> 0).toNumber(true) : message.maximum;
                return object;
            };
    
            /**
             * Converts this File to JSON.
             * @function toJSON
             * @memberof fk_app.File
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            File.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return File;
        })();
    
        fk_app.Files = (function() {
    
            /**
             * Properties of a Files.
             * @memberof fk_app
             * @interface IFiles
             * @property {Array.<fk_app.IFile>} [files] Files files
             */
    
            /**
             * Constructs a new Files.
             * @memberof fk_app
             * @classdesc Represents a Files.
             * @constructor
             * @param {fk_app.IFiles=} [properties] Properties to set
             */
            function Files(properties) {
                this.files = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Files files.
             * @member {Array.<fk_app.IFile>}files
             * @memberof fk_app.Files
             * @instance
             */
            Files.prototype.files = $util.emptyArray;
    
            /**
             * Creates a new Files instance using the specified properties.
             * @function create
             * @memberof fk_app.Files
             * @static
             * @param {fk_app.IFiles=} [properties] Properties to set
             * @returns {fk_app.Files} Files instance
             */
            Files.create = function create(properties) {
                return new Files(properties);
            };
    
            /**
             * Encodes the specified Files message. Does not implicitly {@link fk_app.Files.verify|verify} messages.
             * @function encode
             * @memberof fk_app.Files
             * @static
             * @param {fk_app.IFiles} message Files message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Files.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.files != null && message.files.length)
                    for (var i = 0; i < message.files.length; ++i)
                        $root.fk_app.File.encode(message.files[i], writer.uint32(/* id 1, wireType 2 =*/10).fork()).ldelim();
                return writer;
            };
    
            /**
             * Encodes the specified Files message, length delimited. Does not implicitly {@link fk_app.Files.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.Files
             * @static
             * @param {fk_app.IFiles} message Files message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Files.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a Files message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.Files
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.Files} Files
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Files.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.Files();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        if (!(message.files && message.files.length))
                            message.files = [];
                        message.files.push($root.fk_app.File.decode(reader, reader.uint32()));
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a Files message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.Files
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.Files} Files
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Files.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a Files message.
             * @function verify
             * @memberof fk_app.Files
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            Files.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.files != null && message.hasOwnProperty("files")) {
                    if (!Array.isArray(message.files))
                        return "files: array expected";
                    for (var i = 0; i < message.files.length; ++i) {
                        var error = $root.fk_app.File.verify(message.files[i]);
                        if (error)
                            return "files." + error;
                    }
                }
                return null;
            };
    
            /**
             * Creates a Files message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.Files
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.Files} Files
             */
            Files.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.Files)
                    return object;
                var message = new $root.fk_app.Files();
                if (object.files) {
                    if (!Array.isArray(object.files))
                        throw TypeError(".fk_app.Files.files: array expected");
                    message.files = [];
                    for (var i = 0; i < object.files.length; ++i) {
                        if (typeof object.files[i] !== "object")
                            throw TypeError(".fk_app.Files.files: object expected");
                        message.files[i] = $root.fk_app.File.fromObject(object.files[i]);
                    }
                }
                return message;
            };
    
            /**
             * Creates a plain object from a Files message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.Files
             * @static
             * @param {fk_app.Files} message Files
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            Files.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults)
                    object.files = [];
                if (message.files && message.files.length) {
                    object.files = [];
                    for (var j = 0; j < message.files.length; ++j)
                        object.files[j] = $root.fk_app.File.toObject(message.files[j], options);
                }
                return object;
            };
    
            /**
             * Converts this Files to JSON.
             * @function toJSON
             * @memberof fk_app.Files
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Files.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Files;
        })();
    
        fk_app.DownloadFile = (function() {
    
            /**
             * Properties of a DownloadFile.
             * @memberof fk_app
             * @interface IDownloadFile
             * @property {number} [id] DownloadFile id
             * @property {number} [offset] DownloadFile offset
             * @property {number} [length] DownloadFile length
             * @property {number} [flags] DownloadFile flags
             */
    
            /**
             * Constructs a new DownloadFile.
             * @memberof fk_app
             * @classdesc Represents a DownloadFile.
             * @constructor
             * @param {fk_app.IDownloadFile=} [properties] Properties to set
             */
            function DownloadFile(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * DownloadFile id.
             * @member {number}id
             * @memberof fk_app.DownloadFile
             * @instance
             */
            DownloadFile.prototype.id = 0;
    
            /**
             * DownloadFile offset.
             * @member {number}offset
             * @memberof fk_app.DownloadFile
             * @instance
             */
            DownloadFile.prototype.offset = 0;
    
            /**
             * DownloadFile length.
             * @member {number}length
             * @memberof fk_app.DownloadFile
             * @instance
             */
            DownloadFile.prototype.length = 0;
    
            /**
             * DownloadFile flags.
             * @member {number}flags
             * @memberof fk_app.DownloadFile
             * @instance
             */
            DownloadFile.prototype.flags = 0;
    
            /**
             * Creates a new DownloadFile instance using the specified properties.
             * @function create
             * @memberof fk_app.DownloadFile
             * @static
             * @param {fk_app.IDownloadFile=} [properties] Properties to set
             * @returns {fk_app.DownloadFile} DownloadFile instance
             */
            DownloadFile.create = function create(properties) {
                return new DownloadFile(properties);
            };
    
            /**
             * Encodes the specified DownloadFile message. Does not implicitly {@link fk_app.DownloadFile.verify|verify} messages.
             * @function encode
             * @memberof fk_app.DownloadFile
             * @static
             * @param {fk_app.IDownloadFile} message DownloadFile message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            DownloadFile.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.id != null && message.hasOwnProperty("id"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.id);
                if (message.offset != null && message.hasOwnProperty("offset"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint32(message.offset);
                if (message.length != null && message.hasOwnProperty("length"))
                    writer.uint32(/* id 3, wireType 0 =*/24).uint32(message.length);
                if (message.flags != null && message.hasOwnProperty("flags"))
                    writer.uint32(/* id 4, wireType 0 =*/32).uint32(message.flags);
                return writer;
            };
    
            /**
             * Encodes the specified DownloadFile message, length delimited. Does not implicitly {@link fk_app.DownloadFile.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.DownloadFile
             * @static
             * @param {fk_app.IDownloadFile} message DownloadFile message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            DownloadFile.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a DownloadFile message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.DownloadFile
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.DownloadFile} DownloadFile
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            DownloadFile.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.DownloadFile();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.id = reader.uint32();
                        break;
                    case 2:
                        message.offset = reader.uint32();
                        break;
                    case 3:
                        message.length = reader.uint32();
                        break;
                    case 4:
                        message.flags = reader.uint32();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a DownloadFile message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.DownloadFile
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.DownloadFile} DownloadFile
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            DownloadFile.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a DownloadFile message.
             * @function verify
             * @memberof fk_app.DownloadFile
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            DownloadFile.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.id != null && message.hasOwnProperty("id"))
                    if (!$util.isInteger(message.id))
                        return "id: integer expected";
                if (message.offset != null && message.hasOwnProperty("offset"))
                    if (!$util.isInteger(message.offset))
                        return "offset: integer expected";
                if (message.length != null && message.hasOwnProperty("length"))
                    if (!$util.isInteger(message.length))
                        return "length: integer expected";
                if (message.flags != null && message.hasOwnProperty("flags"))
                    if (!$util.isInteger(message.flags))
                        return "flags: integer expected";
                return null;
            };
    
            /**
             * Creates a DownloadFile message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.DownloadFile
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.DownloadFile} DownloadFile
             */
            DownloadFile.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.DownloadFile)
                    return object;
                var message = new $root.fk_app.DownloadFile();
                if (object.id != null)
                    message.id = object.id >>> 0;
                if (object.offset != null)
                    message.offset = object.offset >>> 0;
                if (object.length != null)
                    message.length = object.length >>> 0;
                if (object.flags != null)
                    message.flags = object.flags >>> 0;
                return message;
            };
    
            /**
             * Creates a plain object from a DownloadFile message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.DownloadFile
             * @static
             * @param {fk_app.DownloadFile} message DownloadFile
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            DownloadFile.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.id = 0;
                    object.offset = 0;
                    object.length = 0;
                    object.flags = 0;
                }
                if (message.id != null && message.hasOwnProperty("id"))
                    object.id = message.id;
                if (message.offset != null && message.hasOwnProperty("offset"))
                    object.offset = message.offset;
                if (message.length != null && message.hasOwnProperty("length"))
                    object.length = message.length;
                if (message.flags != null && message.hasOwnProperty("flags"))
                    object.flags = message.flags;
                return object;
            };
    
            /**
             * Converts this DownloadFile to JSON.
             * @function toJSON
             * @memberof fk_app.DownloadFile
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            DownloadFile.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return DownloadFile;
        })();
    
        fk_app.EraseFile = (function() {
    
            /**
             * Properties of an EraseFile.
             * @memberof fk_app
             * @interface IEraseFile
             * @property {number} [id] EraseFile id
             */
    
            /**
             * Constructs a new EraseFile.
             * @memberof fk_app
             * @classdesc Represents an EraseFile.
             * @constructor
             * @param {fk_app.IEraseFile=} [properties] Properties to set
             */
            function EraseFile(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * EraseFile id.
             * @member {number}id
             * @memberof fk_app.EraseFile
             * @instance
             */
            EraseFile.prototype.id = 0;
    
            /**
             * Creates a new EraseFile instance using the specified properties.
             * @function create
             * @memberof fk_app.EraseFile
             * @static
             * @param {fk_app.IEraseFile=} [properties] Properties to set
             * @returns {fk_app.EraseFile} EraseFile instance
             */
            EraseFile.create = function create(properties) {
                return new EraseFile(properties);
            };
    
            /**
             * Encodes the specified EraseFile message. Does not implicitly {@link fk_app.EraseFile.verify|verify} messages.
             * @function encode
             * @memberof fk_app.EraseFile
             * @static
             * @param {fk_app.IEraseFile} message EraseFile message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            EraseFile.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.id != null && message.hasOwnProperty("id"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.id);
                return writer;
            };
    
            /**
             * Encodes the specified EraseFile message, length delimited. Does not implicitly {@link fk_app.EraseFile.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.EraseFile
             * @static
             * @param {fk_app.IEraseFile} message EraseFile message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            EraseFile.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes an EraseFile message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.EraseFile
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.EraseFile} EraseFile
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            EraseFile.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.EraseFile();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.id = reader.uint32();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes an EraseFile message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.EraseFile
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.EraseFile} EraseFile
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            EraseFile.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies an EraseFile message.
             * @function verify
             * @memberof fk_app.EraseFile
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            EraseFile.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.id != null && message.hasOwnProperty("id"))
                    if (!$util.isInteger(message.id))
                        return "id: integer expected";
                return null;
            };
    
            /**
             * Creates an EraseFile message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.EraseFile
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.EraseFile} EraseFile
             */
            EraseFile.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.EraseFile)
                    return object;
                var message = new $root.fk_app.EraseFile();
                if (object.id != null)
                    message.id = object.id >>> 0;
                return message;
            };
    
            /**
             * Creates a plain object from an EraseFile message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.EraseFile
             * @static
             * @param {fk_app.EraseFile} message EraseFile
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            EraseFile.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults)
                    object.id = 0;
                if (message.id != null && message.hasOwnProperty("id"))
                    object.id = message.id;
                return object;
            };
    
            /**
             * Converts this EraseFile to JSON.
             * @function toJSON
             * @memberof fk_app.EraseFile
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            EraseFile.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return EraseFile;
        })();
    
        fk_app.FileData = (function() {
    
            /**
             * Properties of a FileData.
             * @memberof fk_app
             * @interface IFileData
             * @property {number} [offset] FileData offset
             * @property {Uint8Array} [data] FileData data
             * @property {number} [size] FileData size
             * @property {number} [hash] FileData hash
             * @property {number} [version] FileData version
             * @property {number} [id] FileData id
             */
    
            /**
             * Constructs a new FileData.
             * @memberof fk_app
             * @classdesc Represents a FileData.
             * @constructor
             * @param {fk_app.IFileData=} [properties] Properties to set
             */
            function FileData(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * FileData offset.
             * @member {number}offset
             * @memberof fk_app.FileData
             * @instance
             */
            FileData.prototype.offset = 0;
    
            /**
             * FileData data.
             * @member {Uint8Array}data
             * @memberof fk_app.FileData
             * @instance
             */
            FileData.prototype.data = $util.newBuffer([]);
    
            /**
             * FileData size.
             * @member {number}size
             * @memberof fk_app.FileData
             * @instance
             */
            FileData.prototype.size = 0;
    
            /**
             * FileData hash.
             * @member {number}hash
             * @memberof fk_app.FileData
             * @instance
             */
            FileData.prototype.hash = 0;
    
            /**
             * FileData version.
             * @member {number}version
             * @memberof fk_app.FileData
             * @instance
             */
            FileData.prototype.version = 0;
    
            /**
             * FileData id.
             * @member {number}id
             * @memberof fk_app.FileData
             * @instance
             */
            FileData.prototype.id = 0;
    
            /**
             * Creates a new FileData instance using the specified properties.
             * @function create
             * @memberof fk_app.FileData
             * @static
             * @param {fk_app.IFileData=} [properties] Properties to set
             * @returns {fk_app.FileData} FileData instance
             */
            FileData.create = function create(properties) {
                return new FileData(properties);
            };
    
            /**
             * Encodes the specified FileData message. Does not implicitly {@link fk_app.FileData.verify|verify} messages.
             * @function encode
             * @memberof fk_app.FileData
             * @static
             * @param {fk_app.IFileData} message FileData message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            FileData.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.offset != null && message.hasOwnProperty("offset"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.offset);
                if (message.data != null && message.hasOwnProperty("data"))
                    writer.uint32(/* id 2, wireType 2 =*/18).bytes(message.data);
                if (message.size != null && message.hasOwnProperty("size"))
                    writer.uint32(/* id 3, wireType 0 =*/24).uint32(message.size);
                if (message.hash != null && message.hasOwnProperty("hash"))
                    writer.uint32(/* id 4, wireType 0 =*/32).uint32(message.hash);
                if (message.version != null && message.hasOwnProperty("version"))
                    writer.uint32(/* id 5, wireType 0 =*/40).uint32(message.version);
                if (message.id != null && message.hasOwnProperty("id"))
                    writer.uint32(/* id 6, wireType 0 =*/48).uint32(message.id);
                return writer;
            };
    
            /**
             * Encodes the specified FileData message, length delimited. Does not implicitly {@link fk_app.FileData.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.FileData
             * @static
             * @param {fk_app.IFileData} message FileData message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            FileData.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a FileData message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.FileData
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.FileData} FileData
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            FileData.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.FileData();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.offset = reader.uint32();
                        break;
                    case 2:
                        message.data = reader.bytes();
                        break;
                    case 3:
                        message.size = reader.uint32();
                        break;
                    case 4:
                        message.hash = reader.uint32();
                        break;
                    case 5:
                        message.version = reader.uint32();
                        break;
                    case 6:
                        message.id = reader.uint32();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a FileData message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.FileData
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.FileData} FileData
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            FileData.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a FileData message.
             * @function verify
             * @memberof fk_app.FileData
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            FileData.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.offset != null && message.hasOwnProperty("offset"))
                    if (!$util.isInteger(message.offset))
                        return "offset: integer expected";
                if (message.data != null && message.hasOwnProperty("data"))
                    if (!(message.data && typeof message.data.length === "number" || $util.isString(message.data)))
                        return "data: buffer expected";
                if (message.size != null && message.hasOwnProperty("size"))
                    if (!$util.isInteger(message.size))
                        return "size: integer expected";
                if (message.hash != null && message.hasOwnProperty("hash"))
                    if (!$util.isInteger(message.hash))
                        return "hash: integer expected";
                if (message.version != null && message.hasOwnProperty("version"))
                    if (!$util.isInteger(message.version))
                        return "version: integer expected";
                if (message.id != null && message.hasOwnProperty("id"))
                    if (!$util.isInteger(message.id))
                        return "id: integer expected";
                return null;
            };
    
            /**
             * Creates a FileData message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.FileData
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.FileData} FileData
             */
            FileData.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.FileData)
                    return object;
                var message = new $root.fk_app.FileData();
                if (object.offset != null)
                    message.offset = object.offset >>> 0;
                if (object.data != null)
                    if (typeof object.data === "string")
                        $util.base64.decode(object.data, message.data = $util.newBuffer($util.base64.length(object.data)), 0);
                    else if (object.data.length)
                        message.data = object.data;
                if (object.size != null)
                    message.size = object.size >>> 0;
                if (object.hash != null)
                    message.hash = object.hash >>> 0;
                if (object.version != null)
                    message.version = object.version >>> 0;
                if (object.id != null)
                    message.id = object.id >>> 0;
                return message;
            };
    
            /**
             * Creates a plain object from a FileData message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.FileData
             * @static
             * @param {fk_app.FileData} message FileData
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            FileData.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.offset = 0;
                    object.data = options.bytes === String ? "" : [];
                    object.size = 0;
                    object.hash = 0;
                    object.version = 0;
                    object.id = 0;
                }
                if (message.offset != null && message.hasOwnProperty("offset"))
                    object.offset = message.offset;
                if (message.data != null && message.hasOwnProperty("data"))
                    object.data = options.bytes === String ? $util.base64.encode(message.data, 0, message.data.length) : options.bytes === Array ? Array.prototype.slice.call(message.data) : message.data;
                if (message.size != null && message.hasOwnProperty("size"))
                    object.size = message.size;
                if (message.hash != null && message.hasOwnProperty("hash"))
                    object.hash = message.hash;
                if (message.version != null && message.hasOwnProperty("version"))
                    object.version = message.version;
                if (message.id != null && message.hasOwnProperty("id"))
                    object.id = message.id;
                return object;
            };
    
            /**
             * Converts this FileData to JSON.
             * @function toJSON
             * @memberof fk_app.FileData
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            FileData.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return FileData;
        })();
    
        fk_app.DeviceStatus = (function() {
    
            /**
             * Properties of a DeviceStatus.
             * @memberof fk_app
             * @interface IDeviceStatus
             * @property {number} [uptime] DeviceStatus uptime
             * @property {number} [batteryPercentage] DeviceStatus batteryPercentage
             * @property {number} [batteryVoltage] DeviceStatus batteryVoltage
             * @property {number} [gpsHasFix] DeviceStatus gpsHasFix
             * @property {number} [gpsSatellites] DeviceStatus gpsSatellites
             */
    
            /**
             * Constructs a new DeviceStatus.
             * @memberof fk_app
             * @classdesc Represents a DeviceStatus.
             * @constructor
             * @param {fk_app.IDeviceStatus=} [properties] Properties to set
             */
            function DeviceStatus(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * DeviceStatus uptime.
             * @member {number}uptime
             * @memberof fk_app.DeviceStatus
             * @instance
             */
            DeviceStatus.prototype.uptime = 0;
    
            /**
             * DeviceStatus batteryPercentage.
             * @member {number}batteryPercentage
             * @memberof fk_app.DeviceStatus
             * @instance
             */
            DeviceStatus.prototype.batteryPercentage = 0;
    
            /**
             * DeviceStatus batteryVoltage.
             * @member {number}batteryVoltage
             * @memberof fk_app.DeviceStatus
             * @instance
             */
            DeviceStatus.prototype.batteryVoltage = 0;
    
            /**
             * DeviceStatus gpsHasFix.
             * @member {number}gpsHasFix
             * @memberof fk_app.DeviceStatus
             * @instance
             */
            DeviceStatus.prototype.gpsHasFix = 0;
    
            /**
             * DeviceStatus gpsSatellites.
             * @member {number}gpsSatellites
             * @memberof fk_app.DeviceStatus
             * @instance
             */
            DeviceStatus.prototype.gpsSatellites = 0;
    
            /**
             * Creates a new DeviceStatus instance using the specified properties.
             * @function create
             * @memberof fk_app.DeviceStatus
             * @static
             * @param {fk_app.IDeviceStatus=} [properties] Properties to set
             * @returns {fk_app.DeviceStatus} DeviceStatus instance
             */
            DeviceStatus.create = function create(properties) {
                return new DeviceStatus(properties);
            };
    
            /**
             * Encodes the specified DeviceStatus message. Does not implicitly {@link fk_app.DeviceStatus.verify|verify} messages.
             * @function encode
             * @memberof fk_app.DeviceStatus
             * @static
             * @param {fk_app.IDeviceStatus} message DeviceStatus message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            DeviceStatus.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.uptime != null && message.hasOwnProperty("uptime"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.uptime);
                if (message.batteryPercentage != null && message.hasOwnProperty("batteryPercentage"))
                    writer.uint32(/* id 2, wireType 5 =*/21).float(message.batteryPercentage);
                if (message.batteryVoltage != null && message.hasOwnProperty("batteryVoltage"))
                    writer.uint32(/* id 3, wireType 5 =*/29).float(message.batteryVoltage);
                if (message.gpsHasFix != null && message.hasOwnProperty("gpsHasFix"))
                    writer.uint32(/* id 4, wireType 0 =*/32).uint32(message.gpsHasFix);
                if (message.gpsSatellites != null && message.hasOwnProperty("gpsSatellites"))
                    writer.uint32(/* id 5, wireType 0 =*/40).uint32(message.gpsSatellites);
                return writer;
            };
    
            /**
             * Encodes the specified DeviceStatus message, length delimited. Does not implicitly {@link fk_app.DeviceStatus.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.DeviceStatus
             * @static
             * @param {fk_app.IDeviceStatus} message DeviceStatus message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            DeviceStatus.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a DeviceStatus message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.DeviceStatus
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.DeviceStatus} DeviceStatus
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            DeviceStatus.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.DeviceStatus();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.uptime = reader.uint32();
                        break;
                    case 2:
                        message.batteryPercentage = reader.float();
                        break;
                    case 3:
                        message.batteryVoltage = reader.float();
                        break;
                    case 4:
                        message.gpsHasFix = reader.uint32();
                        break;
                    case 5:
                        message.gpsSatellites = reader.uint32();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a DeviceStatus message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.DeviceStatus
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.DeviceStatus} DeviceStatus
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            DeviceStatus.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a DeviceStatus message.
             * @function verify
             * @memberof fk_app.DeviceStatus
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            DeviceStatus.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.uptime != null && message.hasOwnProperty("uptime"))
                    if (!$util.isInteger(message.uptime))
                        return "uptime: integer expected";
                if (message.batteryPercentage != null && message.hasOwnProperty("batteryPercentage"))
                    if (typeof message.batteryPercentage !== "number")
                        return "batteryPercentage: number expected";
                if (message.batteryVoltage != null && message.hasOwnProperty("batteryVoltage"))
                    if (typeof message.batteryVoltage !== "number")
                        return "batteryVoltage: number expected";
                if (message.gpsHasFix != null && message.hasOwnProperty("gpsHasFix"))
                    if (!$util.isInteger(message.gpsHasFix))
                        return "gpsHasFix: integer expected";
                if (message.gpsSatellites != null && message.hasOwnProperty("gpsSatellites"))
                    if (!$util.isInteger(message.gpsSatellites))
                        return "gpsSatellites: integer expected";
                return null;
            };
    
            /**
             * Creates a DeviceStatus message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.DeviceStatus
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.DeviceStatus} DeviceStatus
             */
            DeviceStatus.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.DeviceStatus)
                    return object;
                var message = new $root.fk_app.DeviceStatus();
                if (object.uptime != null)
                    message.uptime = object.uptime >>> 0;
                if (object.batteryPercentage != null)
                    message.batteryPercentage = Number(object.batteryPercentage);
                if (object.batteryVoltage != null)
                    message.batteryVoltage = Number(object.batteryVoltage);
                if (object.gpsHasFix != null)
                    message.gpsHasFix = object.gpsHasFix >>> 0;
                if (object.gpsSatellites != null)
                    message.gpsSatellites = object.gpsSatellites >>> 0;
                return message;
            };
    
            /**
             * Creates a plain object from a DeviceStatus message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.DeviceStatus
             * @static
             * @param {fk_app.DeviceStatus} message DeviceStatus
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            DeviceStatus.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.uptime = 0;
                    object.batteryPercentage = 0;
                    object.batteryVoltage = 0;
                    object.gpsHasFix = 0;
                    object.gpsSatellites = 0;
                }
                if (message.uptime != null && message.hasOwnProperty("uptime"))
                    object.uptime = message.uptime;
                if (message.batteryPercentage != null && message.hasOwnProperty("batteryPercentage"))
                    object.batteryPercentage = options.json && !isFinite(message.batteryPercentage) ? String(message.batteryPercentage) : message.batteryPercentage;
                if (message.batteryVoltage != null && message.hasOwnProperty("batteryVoltage"))
                    object.batteryVoltage = options.json && !isFinite(message.batteryVoltage) ? String(message.batteryVoltage) : message.batteryVoltage;
                if (message.gpsHasFix != null && message.hasOwnProperty("gpsHasFix"))
                    object.gpsHasFix = message.gpsHasFix;
                if (message.gpsSatellites != null && message.hasOwnProperty("gpsSatellites"))
                    object.gpsSatellites = message.gpsSatellites;
                return object;
            };
    
            /**
             * Converts this DeviceStatus to JSON.
             * @function toJSON
             * @memberof fk_app.DeviceStatus
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            DeviceStatus.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return DeviceStatus;
        })();
    
        fk_app.QueryModule = (function() {
    
            /**
             * Properties of a QueryModule.
             * @memberof fk_app
             * @interface IQueryModule
             * @property {number} [id] QueryModule id
             * @property {number} [address] QueryModule address
             * @property {Uint8Array} [message] QueryModule message
             */
    
            /**
             * Constructs a new QueryModule.
             * @memberof fk_app
             * @classdesc Represents a QueryModule.
             * @constructor
             * @param {fk_app.IQueryModule=} [properties] Properties to set
             */
            function QueryModule(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * QueryModule id.
             * @member {number}id
             * @memberof fk_app.QueryModule
             * @instance
             */
            QueryModule.prototype.id = 0;
    
            /**
             * QueryModule address.
             * @member {number}address
             * @memberof fk_app.QueryModule
             * @instance
             */
            QueryModule.prototype.address = 0;
    
            /**
             * QueryModule message.
             * @member {Uint8Array}message
             * @memberof fk_app.QueryModule
             * @instance
             */
            QueryModule.prototype.message = $util.newBuffer([]);
    
            /**
             * Creates a new QueryModule instance using the specified properties.
             * @function create
             * @memberof fk_app.QueryModule
             * @static
             * @param {fk_app.IQueryModule=} [properties] Properties to set
             * @returns {fk_app.QueryModule} QueryModule instance
             */
            QueryModule.create = function create(properties) {
                return new QueryModule(properties);
            };
    
            /**
             * Encodes the specified QueryModule message. Does not implicitly {@link fk_app.QueryModule.verify|verify} messages.
             * @function encode
             * @memberof fk_app.QueryModule
             * @static
             * @param {fk_app.IQueryModule} message QueryModule message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            QueryModule.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.id != null && message.hasOwnProperty("id"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.id);
                if (message.address != null && message.hasOwnProperty("address"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint32(message.address);
                if (message.message != null && message.hasOwnProperty("message"))
                    writer.uint32(/* id 3, wireType 2 =*/26).bytes(message.message);
                return writer;
            };
    
            /**
             * Encodes the specified QueryModule message, length delimited. Does not implicitly {@link fk_app.QueryModule.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.QueryModule
             * @static
             * @param {fk_app.IQueryModule} message QueryModule message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            QueryModule.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a QueryModule message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.QueryModule
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.QueryModule} QueryModule
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            QueryModule.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.QueryModule();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.id = reader.uint32();
                        break;
                    case 2:
                        message.address = reader.uint32();
                        break;
                    case 3:
                        message.message = reader.bytes();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a QueryModule message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.QueryModule
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.QueryModule} QueryModule
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            QueryModule.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a QueryModule message.
             * @function verify
             * @memberof fk_app.QueryModule
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            QueryModule.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.id != null && message.hasOwnProperty("id"))
                    if (!$util.isInteger(message.id))
                        return "id: integer expected";
                if (message.address != null && message.hasOwnProperty("address"))
                    if (!$util.isInteger(message.address))
                        return "address: integer expected";
                if (message.message != null && message.hasOwnProperty("message"))
                    if (!(message.message && typeof message.message.length === "number" || $util.isString(message.message)))
                        return "message: buffer expected";
                return null;
            };
    
            /**
             * Creates a QueryModule message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.QueryModule
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.QueryModule} QueryModule
             */
            QueryModule.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.QueryModule)
                    return object;
                var message = new $root.fk_app.QueryModule();
                if (object.id != null)
                    message.id = object.id >>> 0;
                if (object.address != null)
                    message.address = object.address >>> 0;
                if (object.message != null)
                    if (typeof object.message === "string")
                        $util.base64.decode(object.message, message.message = $util.newBuffer($util.base64.length(object.message)), 0);
                    else if (object.message.length)
                        message.message = object.message;
                return message;
            };
    
            /**
             * Creates a plain object from a QueryModule message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.QueryModule
             * @static
             * @param {fk_app.QueryModule} message QueryModule
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            QueryModule.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.id = 0;
                    object.address = 0;
                    object.message = options.bytes === String ? "" : [];
                }
                if (message.id != null && message.hasOwnProperty("id"))
                    object.id = message.id;
                if (message.address != null && message.hasOwnProperty("address"))
                    object.address = message.address;
                if (message.message != null && message.hasOwnProperty("message"))
                    object.message = options.bytes === String ? $util.base64.encode(message.message, 0, message.message.length) : options.bytes === Array ? Array.prototype.slice.call(message.message) : message.message;
                return object;
            };
    
            /**
             * Converts this QueryModule to JSON.
             * @function toJSON
             * @memberof fk_app.QueryModule
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            QueryModule.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return QueryModule;
        })();
    
        fk_app.ModuleReply = (function() {
    
            /**
             * Properties of a ModuleReply.
             * @memberof fk_app
             * @interface IModuleReply
             * @property {number} [id] ModuleReply id
             * @property {number} [address] ModuleReply address
             * @property {Uint8Array} [message] ModuleReply message
             */
    
            /**
             * Constructs a new ModuleReply.
             * @memberof fk_app
             * @classdesc Represents a ModuleReply.
             * @constructor
             * @param {fk_app.IModuleReply=} [properties] Properties to set
             */
            function ModuleReply(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * ModuleReply id.
             * @member {number}id
             * @memberof fk_app.ModuleReply
             * @instance
             */
            ModuleReply.prototype.id = 0;
    
            /**
             * ModuleReply address.
             * @member {number}address
             * @memberof fk_app.ModuleReply
             * @instance
             */
            ModuleReply.prototype.address = 0;
    
            /**
             * ModuleReply message.
             * @member {Uint8Array}message
             * @memberof fk_app.ModuleReply
             * @instance
             */
            ModuleReply.prototype.message = $util.newBuffer([]);
    
            /**
             * Creates a new ModuleReply instance using the specified properties.
             * @function create
             * @memberof fk_app.ModuleReply
             * @static
             * @param {fk_app.IModuleReply=} [properties] Properties to set
             * @returns {fk_app.ModuleReply} ModuleReply instance
             */
            ModuleReply.create = function create(properties) {
                return new ModuleReply(properties);
            };
    
            /**
             * Encodes the specified ModuleReply message. Does not implicitly {@link fk_app.ModuleReply.verify|verify} messages.
             * @function encode
             * @memberof fk_app.ModuleReply
             * @static
             * @param {fk_app.IModuleReply} message ModuleReply message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            ModuleReply.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.id != null && message.hasOwnProperty("id"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.id);
                if (message.address != null && message.hasOwnProperty("address"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint32(message.address);
                if (message.message != null && message.hasOwnProperty("message"))
                    writer.uint32(/* id 3, wireType 2 =*/26).bytes(message.message);
                return writer;
            };
    
            /**
             * Encodes the specified ModuleReply message, length delimited. Does not implicitly {@link fk_app.ModuleReply.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.ModuleReply
             * @static
             * @param {fk_app.IModuleReply} message ModuleReply message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            ModuleReply.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a ModuleReply message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.ModuleReply
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.ModuleReply} ModuleReply
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            ModuleReply.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.ModuleReply();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.id = reader.uint32();
                        break;
                    case 2:
                        message.address = reader.uint32();
                        break;
                    case 3:
                        message.message = reader.bytes();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a ModuleReply message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.ModuleReply
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.ModuleReply} ModuleReply
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            ModuleReply.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a ModuleReply message.
             * @function verify
             * @memberof fk_app.ModuleReply
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            ModuleReply.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.id != null && message.hasOwnProperty("id"))
                    if (!$util.isInteger(message.id))
                        return "id: integer expected";
                if (message.address != null && message.hasOwnProperty("address"))
                    if (!$util.isInteger(message.address))
                        return "address: integer expected";
                if (message.message != null && message.hasOwnProperty("message"))
                    if (!(message.message && typeof message.message.length === "number" || $util.isString(message.message)))
                        return "message: buffer expected";
                return null;
            };
    
            /**
             * Creates a ModuleReply message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.ModuleReply
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.ModuleReply} ModuleReply
             */
            ModuleReply.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.ModuleReply)
                    return object;
                var message = new $root.fk_app.ModuleReply();
                if (object.id != null)
                    message.id = object.id >>> 0;
                if (object.address != null)
                    message.address = object.address >>> 0;
                if (object.message != null)
                    if (typeof object.message === "string")
                        $util.base64.decode(object.message, message.message = $util.newBuffer($util.base64.length(object.message)), 0);
                    else if (object.message.length)
                        message.message = object.message;
                return message;
            };
    
            /**
             * Creates a plain object from a ModuleReply message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.ModuleReply
             * @static
             * @param {fk_app.ModuleReply} message ModuleReply
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            ModuleReply.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.id = 0;
                    object.address = 0;
                    object.message = options.bytes === String ? "" : [];
                }
                if (message.id != null && message.hasOwnProperty("id"))
                    object.id = message.id;
                if (message.address != null && message.hasOwnProperty("address"))
                    object.address = message.address;
                if (message.message != null && message.hasOwnProperty("message"))
                    object.message = options.bytes === String ? $util.base64.encode(message.message, 0, message.message.length) : options.bytes === Array ? Array.prototype.slice.call(message.message) : message.message;
                return object;
            };
    
            /**
             * Converts this ModuleReply to JSON.
             * @function toJSON
             * @memberof fk_app.ModuleReply
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            ModuleReply.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return ModuleReply;
        })();
    
        fk_app.WireMessageQuery = (function() {
    
            /**
             * Properties of a WireMessageQuery.
             * @memberof fk_app
             * @interface IWireMessageQuery
             * @property {fk_app.QueryType} [type] WireMessageQuery type
             * @property {fk_app.IQueryCapabilities} [queryCapabilities] WireMessageQuery queryCapabilities
             * @property {fk_app.IConfigureSensorQuery} [configureSensor] WireMessageQuery configureSensor
             * @property {fk_app.ILiveDataPoll} [liveDataPoll] WireMessageQuery liveDataPoll
             * @property {fk_app.IDownloadFile} [downloadFile] WireMessageQuery downloadFile
             * @property {fk_app.IEraseFile} [eraseFile] WireMessageQuery eraseFile
             * @property {fk_app.INetworkSettings} [networkSettings] WireMessageQuery networkSettings
             * @property {fk_app.IIdentity} [identity] WireMessageQuery identity
             * @property {fk_app.IQueryModule} [module] WireMessageQuery module
             */
    
            /**
             * Constructs a new WireMessageQuery.
             * @memberof fk_app
             * @classdesc Represents a WireMessageQuery.
             * @constructor
             * @param {fk_app.IWireMessageQuery=} [properties] Properties to set
             */
            function WireMessageQuery(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * WireMessageQuery type.
             * @member {fk_app.QueryType}type
             * @memberof fk_app.WireMessageQuery
             * @instance
             */
            WireMessageQuery.prototype.type = 0;
    
            /**
             * WireMessageQuery queryCapabilities.
             * @member {(fk_app.IQueryCapabilities|null|undefined)}queryCapabilities
             * @memberof fk_app.WireMessageQuery
             * @instance
             */
            WireMessageQuery.prototype.queryCapabilities = null;
    
            /**
             * WireMessageQuery configureSensor.
             * @member {(fk_app.IConfigureSensorQuery|null|undefined)}configureSensor
             * @memberof fk_app.WireMessageQuery
             * @instance
             */
            WireMessageQuery.prototype.configureSensor = null;
    
            /**
             * WireMessageQuery liveDataPoll.
             * @member {(fk_app.ILiveDataPoll|null|undefined)}liveDataPoll
             * @memberof fk_app.WireMessageQuery
             * @instance
             */
            WireMessageQuery.prototype.liveDataPoll = null;
    
            /**
             * WireMessageQuery downloadFile.
             * @member {(fk_app.IDownloadFile|null|undefined)}downloadFile
             * @memberof fk_app.WireMessageQuery
             * @instance
             */
            WireMessageQuery.prototype.downloadFile = null;
    
            /**
             * WireMessageQuery eraseFile.
             * @member {(fk_app.IEraseFile|null|undefined)}eraseFile
             * @memberof fk_app.WireMessageQuery
             * @instance
             */
            WireMessageQuery.prototype.eraseFile = null;
    
            /**
             * WireMessageQuery networkSettings.
             * @member {(fk_app.INetworkSettings|null|undefined)}networkSettings
             * @memberof fk_app.WireMessageQuery
             * @instance
             */
            WireMessageQuery.prototype.networkSettings = null;
    
            /**
             * WireMessageQuery identity.
             * @member {(fk_app.IIdentity|null|undefined)}identity
             * @memberof fk_app.WireMessageQuery
             * @instance
             */
            WireMessageQuery.prototype.identity = null;
    
            /**
             * WireMessageQuery module.
             * @member {(fk_app.IQueryModule|null|undefined)}module
             * @memberof fk_app.WireMessageQuery
             * @instance
             */
            WireMessageQuery.prototype.module = null;
    
            /**
             * Creates a new WireMessageQuery instance using the specified properties.
             * @function create
             * @memberof fk_app.WireMessageQuery
             * @static
             * @param {fk_app.IWireMessageQuery=} [properties] Properties to set
             * @returns {fk_app.WireMessageQuery} WireMessageQuery instance
             */
            WireMessageQuery.create = function create(properties) {
                return new WireMessageQuery(properties);
            };
    
            /**
             * Encodes the specified WireMessageQuery message. Does not implicitly {@link fk_app.WireMessageQuery.verify|verify} messages.
             * @function encode
             * @memberof fk_app.WireMessageQuery
             * @static
             * @param {fk_app.IWireMessageQuery} message WireMessageQuery message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            WireMessageQuery.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.type != null && message.hasOwnProperty("type"))
                    writer.uint32(/* id 1, wireType 0 =*/8).int32(message.type);
                if (message.queryCapabilities != null && message.hasOwnProperty("queryCapabilities"))
                    $root.fk_app.QueryCapabilities.encode(message.queryCapabilities, writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
                if (message.configureSensor != null && message.hasOwnProperty("configureSensor"))
                    $root.fk_app.ConfigureSensorQuery.encode(message.configureSensor, writer.uint32(/* id 3, wireType 2 =*/26).fork()).ldelim();
                if (message.liveDataPoll != null && message.hasOwnProperty("liveDataPoll"))
                    $root.fk_app.LiveDataPoll.encode(message.liveDataPoll, writer.uint32(/* id 8, wireType 2 =*/66).fork()).ldelim();
                if (message.downloadFile != null && message.hasOwnProperty("downloadFile"))
                    $root.fk_app.DownloadFile.encode(message.downloadFile, writer.uint32(/* id 10, wireType 2 =*/82).fork()).ldelim();
                if (message.eraseFile != null && message.hasOwnProperty("eraseFile"))
                    $root.fk_app.EraseFile.encode(message.eraseFile, writer.uint32(/* id 11, wireType 2 =*/90).fork()).ldelim();
                if (message.networkSettings != null && message.hasOwnProperty("networkSettings"))
                    $root.fk_app.NetworkSettings.encode(message.networkSettings, writer.uint32(/* id 12, wireType 2 =*/98).fork()).ldelim();
                if (message.identity != null && message.hasOwnProperty("identity"))
                    $root.fk_app.Identity.encode(message.identity, writer.uint32(/* id 13, wireType 2 =*/106).fork()).ldelim();
                if (message.module != null && message.hasOwnProperty("module"))
                    $root.fk_app.QueryModule.encode(message.module, writer.uint32(/* id 14, wireType 2 =*/114).fork()).ldelim();
                return writer;
            };
    
            /**
             * Encodes the specified WireMessageQuery message, length delimited. Does not implicitly {@link fk_app.WireMessageQuery.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.WireMessageQuery
             * @static
             * @param {fk_app.IWireMessageQuery} message WireMessageQuery message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            WireMessageQuery.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a WireMessageQuery message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.WireMessageQuery
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.WireMessageQuery} WireMessageQuery
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            WireMessageQuery.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.WireMessageQuery();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.type = reader.int32();
                        break;
                    case 2:
                        message.queryCapabilities = $root.fk_app.QueryCapabilities.decode(reader, reader.uint32());
                        break;
                    case 3:
                        message.configureSensor = $root.fk_app.ConfigureSensorQuery.decode(reader, reader.uint32());
                        break;
                    case 8:
                        message.liveDataPoll = $root.fk_app.LiveDataPoll.decode(reader, reader.uint32());
                        break;
                    case 10:
                        message.downloadFile = $root.fk_app.DownloadFile.decode(reader, reader.uint32());
                        break;
                    case 11:
                        message.eraseFile = $root.fk_app.EraseFile.decode(reader, reader.uint32());
                        break;
                    case 12:
                        message.networkSettings = $root.fk_app.NetworkSettings.decode(reader, reader.uint32());
                        break;
                    case 13:
                        message.identity = $root.fk_app.Identity.decode(reader, reader.uint32());
                        break;
                    case 14:
                        message.module = $root.fk_app.QueryModule.decode(reader, reader.uint32());
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a WireMessageQuery message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.WireMessageQuery
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.WireMessageQuery} WireMessageQuery
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            WireMessageQuery.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a WireMessageQuery message.
             * @function verify
             * @memberof fk_app.WireMessageQuery
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            WireMessageQuery.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.type != null && message.hasOwnProperty("type"))
                    switch (message.type) {
                    default:
                        return "type: enum value expected";
                    case 0:
                    case 1:
                    case 2:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                        break;
                    }
                if (message.queryCapabilities != null && message.hasOwnProperty("queryCapabilities")) {
                    var error = $root.fk_app.QueryCapabilities.verify(message.queryCapabilities);
                    if (error)
                        return "queryCapabilities." + error;
                }
                if (message.configureSensor != null && message.hasOwnProperty("configureSensor")) {
                    error = $root.fk_app.ConfigureSensorQuery.verify(message.configureSensor);
                    if (error)
                        return "configureSensor." + error;
                }
                if (message.liveDataPoll != null && message.hasOwnProperty("liveDataPoll")) {
                    error = $root.fk_app.LiveDataPoll.verify(message.liveDataPoll);
                    if (error)
                        return "liveDataPoll." + error;
                }
                if (message.downloadFile != null && message.hasOwnProperty("downloadFile")) {
                    error = $root.fk_app.DownloadFile.verify(message.downloadFile);
                    if (error)
                        return "downloadFile." + error;
                }
                if (message.eraseFile != null && message.hasOwnProperty("eraseFile")) {
                    error = $root.fk_app.EraseFile.verify(message.eraseFile);
                    if (error)
                        return "eraseFile." + error;
                }
                if (message.networkSettings != null && message.hasOwnProperty("networkSettings")) {
                    error = $root.fk_app.NetworkSettings.verify(message.networkSettings);
                    if (error)
                        return "networkSettings." + error;
                }
                if (message.identity != null && message.hasOwnProperty("identity")) {
                    error = $root.fk_app.Identity.verify(message.identity);
                    if (error)
                        return "identity." + error;
                }
                if (message.module != null && message.hasOwnProperty("module")) {
                    error = $root.fk_app.QueryModule.verify(message.module);
                    if (error)
                        return "module." + error;
                }
                return null;
            };
    
            /**
             * Creates a WireMessageQuery message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.WireMessageQuery
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.WireMessageQuery} WireMessageQuery
             */
            WireMessageQuery.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.WireMessageQuery)
                    return object;
                var message = new $root.fk_app.WireMessageQuery();
                switch (object.type) {
                case "QUERY_NONE":
                case 0:
                    message.type = 0;
                    break;
                case "QUERY_CAPABILITIES":
                case 1:
                    message.type = 1;
                    break;
                case "QUERY_CONFIGURE_SENSOR":
                case 2:
                    message.type = 2;
                    break;
                case "QUERY_LIVE_DATA_POLL":
                case 7:
                    message.type = 7;
                    break;
                case "QUERY_SCHEDULES":
                case 8:
                    message.type = 8;
                    break;
                case "QUERY_CONFIGURE_SCHEDULES":
                case 9:
                    message.type = 9;
                    break;
                case "QUERY_FILES_SD":
                case 10:
                    message.type = 10;
                    break;
                case "QUERY_DOWNLOAD_FILE":
                case 11:
                    message.type = 11;
                    break;
                case "QUERY_ERASE_FILE":
                case 12:
                    message.type = 12;
                    break;
                case "QUERY_RESET":
                case 13:
                    message.type = 13;
                    break;
                case "QUERY_NETWORK_SETTINGS":
                case 14:
                    message.type = 14;
                    break;
                case "QUERY_CONFIGURE_NETWORK_SETTINGS":
                case 15:
                    message.type = 15;
                    break;
                case "QUERY_SCAN_MODULES":
                case 16:
                    message.type = 16;
                    break;
                case "QUERY_CONFIGURE_IDENTITY":
                case 17:
                    message.type = 17;
                    break;
                case "QUERY_STATUS":
                case 18:
                    message.type = 18;
                    break;
                case "QUERY_MODULE":
                case 19:
                    message.type = 19;
                    break;
                case "QUERY_METADATA":
                case 20:
                    message.type = 20;
                    break;
                case "QUERY_FORMAT":
                case 21:
                    message.type = 21;
                    break;
                case "QUERY_GET_READINGS":
                case 22:
                    message.type = 22;
                    break;
                case "QUERY_TAKE_READINGS":
                case 23:
                    message.type = 23;
                    break;
                case "QUERY_RECORDING_CONTROL":
                case 24:
                    message.type = 24;
                    break;
                case "QUERY_CONFIGURE":
                case 25:
                    message.type = 25;
                    break;
                case "QUERY_SCAN_NETWORKS":
                case 26:
                    message.type = 26;
                    break;
                case "QUERY_FILES_SPI":
                case 27:
                    message.type = 27;
                    break;
                case "QUERY_FILES_QSPI":
                case 28:
                    message.type = 28;
                    break;
                }
                if (object.queryCapabilities != null) {
                    if (typeof object.queryCapabilities !== "object")
                        throw TypeError(".fk_app.WireMessageQuery.queryCapabilities: object expected");
                    message.queryCapabilities = $root.fk_app.QueryCapabilities.fromObject(object.queryCapabilities);
                }
                if (object.configureSensor != null) {
                    if (typeof object.configureSensor !== "object")
                        throw TypeError(".fk_app.WireMessageQuery.configureSensor: object expected");
                    message.configureSensor = $root.fk_app.ConfigureSensorQuery.fromObject(object.configureSensor);
                }
                if (object.liveDataPoll != null) {
                    if (typeof object.liveDataPoll !== "object")
                        throw TypeError(".fk_app.WireMessageQuery.liveDataPoll: object expected");
                    message.liveDataPoll = $root.fk_app.LiveDataPoll.fromObject(object.liveDataPoll);
                }
                if (object.downloadFile != null) {
                    if (typeof object.downloadFile !== "object")
                        throw TypeError(".fk_app.WireMessageQuery.downloadFile: object expected");
                    message.downloadFile = $root.fk_app.DownloadFile.fromObject(object.downloadFile);
                }
                if (object.eraseFile != null) {
                    if (typeof object.eraseFile !== "object")
                        throw TypeError(".fk_app.WireMessageQuery.eraseFile: object expected");
                    message.eraseFile = $root.fk_app.EraseFile.fromObject(object.eraseFile);
                }
                if (object.networkSettings != null) {
                    if (typeof object.networkSettings !== "object")
                        throw TypeError(".fk_app.WireMessageQuery.networkSettings: object expected");
                    message.networkSettings = $root.fk_app.NetworkSettings.fromObject(object.networkSettings);
                }
                if (object.identity != null) {
                    if (typeof object.identity !== "object")
                        throw TypeError(".fk_app.WireMessageQuery.identity: object expected");
                    message.identity = $root.fk_app.Identity.fromObject(object.identity);
                }
                if (object.module != null) {
                    if (typeof object.module !== "object")
                        throw TypeError(".fk_app.WireMessageQuery.module: object expected");
                    message.module = $root.fk_app.QueryModule.fromObject(object.module);
                }
                return message;
            };
    
            /**
             * Creates a plain object from a WireMessageQuery message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.WireMessageQuery
             * @static
             * @param {fk_app.WireMessageQuery} message WireMessageQuery
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            WireMessageQuery.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.type = options.enums === String ? "QUERY_NONE" : 0;
                    object.queryCapabilities = null;
                    object.configureSensor = null;
                    object.liveDataPoll = null;
                    object.downloadFile = null;
                    object.eraseFile = null;
                    object.networkSettings = null;
                    object.identity = null;
                    object.module = null;
                }
                if (message.type != null && message.hasOwnProperty("type"))
                    object.type = options.enums === String ? $root.fk_app.QueryType[message.type] : message.type;
                if (message.queryCapabilities != null && message.hasOwnProperty("queryCapabilities"))
                    object.queryCapabilities = $root.fk_app.QueryCapabilities.toObject(message.queryCapabilities, options);
                if (message.configureSensor != null && message.hasOwnProperty("configureSensor"))
                    object.configureSensor = $root.fk_app.ConfigureSensorQuery.toObject(message.configureSensor, options);
                if (message.liveDataPoll != null && message.hasOwnProperty("liveDataPoll"))
                    object.liveDataPoll = $root.fk_app.LiveDataPoll.toObject(message.liveDataPoll, options);
                if (message.downloadFile != null && message.hasOwnProperty("downloadFile"))
                    object.downloadFile = $root.fk_app.DownloadFile.toObject(message.downloadFile, options);
                if (message.eraseFile != null && message.hasOwnProperty("eraseFile"))
                    object.eraseFile = $root.fk_app.EraseFile.toObject(message.eraseFile, options);
                if (message.networkSettings != null && message.hasOwnProperty("networkSettings"))
                    object.networkSettings = $root.fk_app.NetworkSettings.toObject(message.networkSettings, options);
                if (message.identity != null && message.hasOwnProperty("identity"))
                    object.identity = $root.fk_app.Identity.toObject(message.identity, options);
                if (message.module != null && message.hasOwnProperty("module"))
                    object.module = $root.fk_app.QueryModule.toObject(message.module, options);
                return object;
            };
    
            /**
             * Converts this WireMessageQuery to JSON.
             * @function toJSON
             * @memberof fk_app.WireMessageQuery
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            WireMessageQuery.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return WireMessageQuery;
        })();
    
        fk_app.Error = (function() {
    
            /**
             * Properties of an Error.
             * @memberof fk_app
             * @interface IError
             * @property {string} [message] Error message
             * @property {number} [delay] Error delay
             */
    
            /**
             * Constructs a new Error.
             * @memberof fk_app
             * @classdesc Represents an Error.
             * @constructor
             * @param {fk_app.IError=} [properties] Properties to set
             */
            function Error(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Error message.
             * @member {string}message
             * @memberof fk_app.Error
             * @instance
             */
            Error.prototype.message = "";
    
            /**
             * Error delay.
             * @member {number}delay
             * @memberof fk_app.Error
             * @instance
             */
            Error.prototype.delay = 0;
    
            /**
             * Creates a new Error instance using the specified properties.
             * @function create
             * @memberof fk_app.Error
             * @static
             * @param {fk_app.IError=} [properties] Properties to set
             * @returns {fk_app.Error} Error instance
             */
            Error.create = function create(properties) {
                return new Error(properties);
            };
    
            /**
             * Encodes the specified Error message. Does not implicitly {@link fk_app.Error.verify|verify} messages.
             * @function encode
             * @memberof fk_app.Error
             * @static
             * @param {fk_app.IError} message Error message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Error.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.message != null && message.hasOwnProperty("message"))
                    writer.uint32(/* id 1, wireType 2 =*/10).string(message.message);
                if (message.delay != null && message.hasOwnProperty("delay"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint32(message.delay);
                return writer;
            };
    
            /**
             * Encodes the specified Error message, length delimited. Does not implicitly {@link fk_app.Error.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.Error
             * @static
             * @param {fk_app.IError} message Error message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Error.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes an Error message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.Error
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.Error} Error
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Error.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.Error();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.message = reader.string();
                        break;
                    case 2:
                        message.delay = reader.uint32();
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
             * @memberof fk_app.Error
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.Error} Error
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
             * @memberof fk_app.Error
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            Error.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.message != null && message.hasOwnProperty("message"))
                    if (!$util.isString(message.message))
                        return "message: string expected";
                if (message.delay != null && message.hasOwnProperty("delay"))
                    if (!$util.isInteger(message.delay))
                        return "delay: integer expected";
                return null;
            };
    
            /**
             * Creates an Error message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.Error
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.Error} Error
             */
            Error.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.Error)
                    return object;
                var message = new $root.fk_app.Error();
                if (object.message != null)
                    message.message = String(object.message);
                if (object.delay != null)
                    message.delay = object.delay >>> 0;
                return message;
            };
    
            /**
             * Creates a plain object from an Error message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.Error
             * @static
             * @param {fk_app.Error} message Error
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            Error.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.message = "";
                    object.delay = 0;
                }
                if (message.message != null && message.hasOwnProperty("message"))
                    object.message = message.message;
                if (message.delay != null && message.hasOwnProperty("delay"))
                    object.delay = message.delay;
                return object;
            };
    
            /**
             * Converts this Error to JSON.
             * @function toJSON
             * @memberof fk_app.Error
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Error.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Error;
        })();
    
        fk_app.WireMessageReply = (function() {
    
            /**
             * Properties of a WireMessageReply.
             * @memberof fk_app
             * @interface IWireMessageReply
             * @property {fk_app.ReplyType} [type] WireMessageReply type
             * @property {Array.<fk_app.IError>} [errors] WireMessageReply errors
             * @property {fk_app.ICapabilities} [capabilities] WireMessageReply capabilities
             * @property {fk_app.ILiveData} [liveData] WireMessageReply liveData
             * @property {fk_app.IFiles} [files] WireMessageReply files
             * @property {fk_app.IFileData} [fileData] WireMessageReply fileData
             * @property {fk_app.INetworkSettings} [networkSettings] WireMessageReply networkSettings
             * @property {fk_app.IIdentity} [identity] WireMessageReply identity
             * @property {fk_app.IDeviceStatus} [status] WireMessageReply status
             * @property {fk_app.IModuleReply} [module] WireMessageReply module
             */
    
            /**
             * Constructs a new WireMessageReply.
             * @memberof fk_app
             * @classdesc Represents a WireMessageReply.
             * @constructor
             * @param {fk_app.IWireMessageReply=} [properties] Properties to set
             */
            function WireMessageReply(properties) {
                this.errors = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * WireMessageReply type.
             * @member {fk_app.ReplyType}type
             * @memberof fk_app.WireMessageReply
             * @instance
             */
            WireMessageReply.prototype.type = 0;
    
            /**
             * WireMessageReply errors.
             * @member {Array.<fk_app.IError>}errors
             * @memberof fk_app.WireMessageReply
             * @instance
             */
            WireMessageReply.prototype.errors = $util.emptyArray;
    
            /**
             * WireMessageReply capabilities.
             * @member {(fk_app.ICapabilities|null|undefined)}capabilities
             * @memberof fk_app.WireMessageReply
             * @instance
             */
            WireMessageReply.prototype.capabilities = null;
    
            /**
             * WireMessageReply liveData.
             * @member {(fk_app.ILiveData|null|undefined)}liveData
             * @memberof fk_app.WireMessageReply
             * @instance
             */
            WireMessageReply.prototype.liveData = null;
    
            /**
             * WireMessageReply files.
             * @member {(fk_app.IFiles|null|undefined)}files
             * @memberof fk_app.WireMessageReply
             * @instance
             */
            WireMessageReply.prototype.files = null;
    
            /**
             * WireMessageReply fileData.
             * @member {(fk_app.IFileData|null|undefined)}fileData
             * @memberof fk_app.WireMessageReply
             * @instance
             */
            WireMessageReply.prototype.fileData = null;
    
            /**
             * WireMessageReply networkSettings.
             * @member {(fk_app.INetworkSettings|null|undefined)}networkSettings
             * @memberof fk_app.WireMessageReply
             * @instance
             */
            WireMessageReply.prototype.networkSettings = null;
    
            /**
             * WireMessageReply identity.
             * @member {(fk_app.IIdentity|null|undefined)}identity
             * @memberof fk_app.WireMessageReply
             * @instance
             */
            WireMessageReply.prototype.identity = null;
    
            /**
             * WireMessageReply status.
             * @member {(fk_app.IDeviceStatus|null|undefined)}status
             * @memberof fk_app.WireMessageReply
             * @instance
             */
            WireMessageReply.prototype.status = null;
    
            /**
             * WireMessageReply module.
             * @member {(fk_app.IModuleReply|null|undefined)}module
             * @memberof fk_app.WireMessageReply
             * @instance
             */
            WireMessageReply.prototype.module = null;
    
            /**
             * Creates a new WireMessageReply instance using the specified properties.
             * @function create
             * @memberof fk_app.WireMessageReply
             * @static
             * @param {fk_app.IWireMessageReply=} [properties] Properties to set
             * @returns {fk_app.WireMessageReply} WireMessageReply instance
             */
            WireMessageReply.create = function create(properties) {
                return new WireMessageReply(properties);
            };
    
            /**
             * Encodes the specified WireMessageReply message. Does not implicitly {@link fk_app.WireMessageReply.verify|verify} messages.
             * @function encode
             * @memberof fk_app.WireMessageReply
             * @static
             * @param {fk_app.IWireMessageReply} message WireMessageReply message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            WireMessageReply.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.type != null && message.hasOwnProperty("type"))
                    writer.uint32(/* id 1, wireType 0 =*/8).int32(message.type);
                if (message.errors != null && message.errors.length)
                    for (var i = 0; i < message.errors.length; ++i)
                        $root.fk_app.Error.encode(message.errors[i], writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
                if (message.capabilities != null && message.hasOwnProperty("capabilities"))
                    $root.fk_app.Capabilities.encode(message.capabilities, writer.uint32(/* id 3, wireType 2 =*/26).fork()).ldelim();
                if (message.liveData != null && message.hasOwnProperty("liveData"))
                    $root.fk_app.LiveData.encode(message.liveData, writer.uint32(/* id 6, wireType 2 =*/50).fork()).ldelim();
                if (message.files != null && message.hasOwnProperty("files"))
                    $root.fk_app.Files.encode(message.files, writer.uint32(/* id 8, wireType 2 =*/66).fork()).ldelim();
                if (message.fileData != null && message.hasOwnProperty("fileData"))
                    $root.fk_app.FileData.encode(message.fileData, writer.uint32(/* id 9, wireType 2 =*/74).fork()).ldelim();
                if (message.networkSettings != null && message.hasOwnProperty("networkSettings"))
                    $root.fk_app.NetworkSettings.encode(message.networkSettings, writer.uint32(/* id 10, wireType 2 =*/82).fork()).ldelim();
                if (message.identity != null && message.hasOwnProperty("identity"))
                    $root.fk_app.Identity.encode(message.identity, writer.uint32(/* id 11, wireType 2 =*/90).fork()).ldelim();
                if (message.status != null && message.hasOwnProperty("status"))
                    $root.fk_app.DeviceStatus.encode(message.status, writer.uint32(/* id 12, wireType 2 =*/98).fork()).ldelim();
                if (message.module != null && message.hasOwnProperty("module"))
                    $root.fk_app.ModuleReply.encode(message.module, writer.uint32(/* id 13, wireType 2 =*/106).fork()).ldelim();
                return writer;
            };
    
            /**
             * Encodes the specified WireMessageReply message, length delimited. Does not implicitly {@link fk_app.WireMessageReply.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.WireMessageReply
             * @static
             * @param {fk_app.IWireMessageReply} message WireMessageReply message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            WireMessageReply.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a WireMessageReply message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.WireMessageReply
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.WireMessageReply} WireMessageReply
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            WireMessageReply.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.WireMessageReply();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.type = reader.int32();
                        break;
                    case 2:
                        if (!(message.errors && message.errors.length))
                            message.errors = [];
                        message.errors.push($root.fk_app.Error.decode(reader, reader.uint32()));
                        break;
                    case 3:
                        message.capabilities = $root.fk_app.Capabilities.decode(reader, reader.uint32());
                        break;
                    case 6:
                        message.liveData = $root.fk_app.LiveData.decode(reader, reader.uint32());
                        break;
                    case 8:
                        message.files = $root.fk_app.Files.decode(reader, reader.uint32());
                        break;
                    case 9:
                        message.fileData = $root.fk_app.FileData.decode(reader, reader.uint32());
                        break;
                    case 10:
                        message.networkSettings = $root.fk_app.NetworkSettings.decode(reader, reader.uint32());
                        break;
                    case 11:
                        message.identity = $root.fk_app.Identity.decode(reader, reader.uint32());
                        break;
                    case 12:
                        message.status = $root.fk_app.DeviceStatus.decode(reader, reader.uint32());
                        break;
                    case 13:
                        message.module = $root.fk_app.ModuleReply.decode(reader, reader.uint32());
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a WireMessageReply message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.WireMessageReply
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.WireMessageReply} WireMessageReply
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            WireMessageReply.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a WireMessageReply message.
             * @function verify
             * @memberof fk_app.WireMessageReply
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            WireMessageReply.verify = function verify(message) {
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
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                        break;
                    }
                if (message.errors != null && message.hasOwnProperty("errors")) {
                    if (!Array.isArray(message.errors))
                        return "errors: array expected";
                    for (var i = 0; i < message.errors.length; ++i) {
                        var error = $root.fk_app.Error.verify(message.errors[i]);
                        if (error)
                            return "errors." + error;
                    }
                }
                if (message.capabilities != null && message.hasOwnProperty("capabilities")) {
                    error = $root.fk_app.Capabilities.verify(message.capabilities);
                    if (error)
                        return "capabilities." + error;
                }
                if (message.liveData != null && message.hasOwnProperty("liveData")) {
                    error = $root.fk_app.LiveData.verify(message.liveData);
                    if (error)
                        return "liveData." + error;
                }
                if (message.files != null && message.hasOwnProperty("files")) {
                    error = $root.fk_app.Files.verify(message.files);
                    if (error)
                        return "files." + error;
                }
                if (message.fileData != null && message.hasOwnProperty("fileData")) {
                    error = $root.fk_app.FileData.verify(message.fileData);
                    if (error)
                        return "fileData." + error;
                }
                if (message.networkSettings != null && message.hasOwnProperty("networkSettings")) {
                    error = $root.fk_app.NetworkSettings.verify(message.networkSettings);
                    if (error)
                        return "networkSettings." + error;
                }
                if (message.identity != null && message.hasOwnProperty("identity")) {
                    error = $root.fk_app.Identity.verify(message.identity);
                    if (error)
                        return "identity." + error;
                }
                if (message.status != null && message.hasOwnProperty("status")) {
                    error = $root.fk_app.DeviceStatus.verify(message.status);
                    if (error)
                        return "status." + error;
                }
                if (message.module != null && message.hasOwnProperty("module")) {
                    error = $root.fk_app.ModuleReply.verify(message.module);
                    if (error)
                        return "module." + error;
                }
                return null;
            };
    
            /**
             * Creates a WireMessageReply message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.WireMessageReply
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.WireMessageReply} WireMessageReply
             */
            WireMessageReply.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.WireMessageReply)
                    return object;
                var message = new $root.fk_app.WireMessageReply();
                switch (object.type) {
                case "REPLY_NONE":
                case 0:
                    message.type = 0;
                    break;
                case "REPLY_SUCCESS":
                case 1:
                    message.type = 1;
                    break;
                case "REPLY_BUSY":
                case 2:
                    message.type = 2;
                    break;
                case "REPLY_ERROR":
                case 3:
                    message.type = 3;
                    break;
                case "REPLY_CAPABILITIES":
                case 4:
                    message.type = 4;
                    break;
                case "REPLY_LIVE_DATA_POLL":
                case 8:
                    message.type = 8;
                    break;
                case "REPLY_SCHEDULES":
                case 9:
                    message.type = 9;
                    break;
                case "REPLY_FILES":
                case 10:
                    message.type = 10;
                    break;
                case "REPLY_DOWNLOAD_FILE":
                case 11:
                    message.type = 11;
                    break;
                case "REPLY_RESET":
                case 12:
                    message.type = 12;
                    break;
                case "REPLY_NETWORK_SETTINGS":
                case 13:
                    message.type = 13;
                    break;
                case "REPLY_IDENTITY":
                case 14:
                    message.type = 14;
                    break;
                case "REPLY_STATUS":
                case 15:
                    message.type = 15;
                    break;
                case "REPLY_MODULE":
                case 16:
                    message.type = 16;
                    break;
                case "REPLY_METADATA":
                case 17:
                    message.type = 17;
                    break;
                case "REPLY_READINGS":
                case 18:
                    message.type = 18;
                    break;
                case "REPLY_NETWORKS":
                case 19:
                    message.type = 19;
                    break;
                }
                if (object.errors) {
                    if (!Array.isArray(object.errors))
                        throw TypeError(".fk_app.WireMessageReply.errors: array expected");
                    message.errors = [];
                    for (var i = 0; i < object.errors.length; ++i) {
                        if (typeof object.errors[i] !== "object")
                            throw TypeError(".fk_app.WireMessageReply.errors: object expected");
                        message.errors[i] = $root.fk_app.Error.fromObject(object.errors[i]);
                    }
                }
                if (object.capabilities != null) {
                    if (typeof object.capabilities !== "object")
                        throw TypeError(".fk_app.WireMessageReply.capabilities: object expected");
                    message.capabilities = $root.fk_app.Capabilities.fromObject(object.capabilities);
                }
                if (object.liveData != null) {
                    if (typeof object.liveData !== "object")
                        throw TypeError(".fk_app.WireMessageReply.liveData: object expected");
                    message.liveData = $root.fk_app.LiveData.fromObject(object.liveData);
                }
                if (object.files != null) {
                    if (typeof object.files !== "object")
                        throw TypeError(".fk_app.WireMessageReply.files: object expected");
                    message.files = $root.fk_app.Files.fromObject(object.files);
                }
                if (object.fileData != null) {
                    if (typeof object.fileData !== "object")
                        throw TypeError(".fk_app.WireMessageReply.fileData: object expected");
                    message.fileData = $root.fk_app.FileData.fromObject(object.fileData);
                }
                if (object.networkSettings != null) {
                    if (typeof object.networkSettings !== "object")
                        throw TypeError(".fk_app.WireMessageReply.networkSettings: object expected");
                    message.networkSettings = $root.fk_app.NetworkSettings.fromObject(object.networkSettings);
                }
                if (object.identity != null) {
                    if (typeof object.identity !== "object")
                        throw TypeError(".fk_app.WireMessageReply.identity: object expected");
                    message.identity = $root.fk_app.Identity.fromObject(object.identity);
                }
                if (object.status != null) {
                    if (typeof object.status !== "object")
                        throw TypeError(".fk_app.WireMessageReply.status: object expected");
                    message.status = $root.fk_app.DeviceStatus.fromObject(object.status);
                }
                if (object.module != null) {
                    if (typeof object.module !== "object")
                        throw TypeError(".fk_app.WireMessageReply.module: object expected");
                    message.module = $root.fk_app.ModuleReply.fromObject(object.module);
                }
                return message;
            };
    
            /**
             * Creates a plain object from a WireMessageReply message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.WireMessageReply
             * @static
             * @param {fk_app.WireMessageReply} message WireMessageReply
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            WireMessageReply.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults)
                    object.errors = [];
                if (options.defaults) {
                    object.type = options.enums === String ? "REPLY_NONE" : 0;
                    object.capabilities = null;
                    object.liveData = null;
                    object.files = null;
                    object.fileData = null;
                    object.networkSettings = null;
                    object.identity = null;
                    object.status = null;
                    object.module = null;
                }
                if (message.type != null && message.hasOwnProperty("type"))
                    object.type = options.enums === String ? $root.fk_app.ReplyType[message.type] : message.type;
                if (message.errors && message.errors.length) {
                    object.errors = [];
                    for (var j = 0; j < message.errors.length; ++j)
                        object.errors[j] = $root.fk_app.Error.toObject(message.errors[j], options);
                }
                if (message.capabilities != null && message.hasOwnProperty("capabilities"))
                    object.capabilities = $root.fk_app.Capabilities.toObject(message.capabilities, options);
                if (message.liveData != null && message.hasOwnProperty("liveData"))
                    object.liveData = $root.fk_app.LiveData.toObject(message.liveData, options);
                if (message.files != null && message.hasOwnProperty("files"))
                    object.files = $root.fk_app.Files.toObject(message.files, options);
                if (message.fileData != null && message.hasOwnProperty("fileData"))
                    object.fileData = $root.fk_app.FileData.toObject(message.fileData, options);
                if (message.networkSettings != null && message.hasOwnProperty("networkSettings"))
                    object.networkSettings = $root.fk_app.NetworkSettings.toObject(message.networkSettings, options);
                if (message.identity != null && message.hasOwnProperty("identity"))
                    object.identity = $root.fk_app.Identity.toObject(message.identity, options);
                if (message.status != null && message.hasOwnProperty("status"))
                    object.status = $root.fk_app.DeviceStatus.toObject(message.status, options);
                if (message.module != null && message.hasOwnProperty("module"))
                    object.module = $root.fk_app.ModuleReply.toObject(message.module, options);
                return object;
            };
    
            /**
             * Converts this WireMessageReply to JSON.
             * @function toJSON
             * @memberof fk_app.WireMessageReply
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            WireMessageReply.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return WireMessageReply;
        })();
    
        fk_app.Interval = (function() {
    
            /**
             * Properties of an Interval.
             * @memberof fk_app
             * @interface IInterval
             * @property {number|Long} [start] Interval start
             * @property {number|Long} [end] Interval end
             * @property {number} [interval] Interval interval
             */
    
            /**
             * Constructs a new Interval.
             * @memberof fk_app
             * @classdesc Represents an Interval.
             * @constructor
             * @param {fk_app.IInterval=} [properties] Properties to set
             */
            function Interval(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Interval start.
             * @member {number|Long}start
             * @memberof fk_app.Interval
             * @instance
             */
            Interval.prototype.start = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * Interval end.
             * @member {number|Long}end
             * @memberof fk_app.Interval
             * @instance
             */
            Interval.prototype.end = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * Interval interval.
             * @member {number}interval
             * @memberof fk_app.Interval
             * @instance
             */
            Interval.prototype.interval = 0;
    
            /**
             * Creates a new Interval instance using the specified properties.
             * @function create
             * @memberof fk_app.Interval
             * @static
             * @param {fk_app.IInterval=} [properties] Properties to set
             * @returns {fk_app.Interval} Interval instance
             */
            Interval.create = function create(properties) {
                return new Interval(properties);
            };
    
            /**
             * Encodes the specified Interval message. Does not implicitly {@link fk_app.Interval.verify|verify} messages.
             * @function encode
             * @memberof fk_app.Interval
             * @static
             * @param {fk_app.IInterval} message Interval message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Interval.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.start != null && message.hasOwnProperty("start"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint64(message.start);
                if (message.end != null && message.hasOwnProperty("end"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint64(message.end);
                if (message.interval != null && message.hasOwnProperty("interval"))
                    writer.uint32(/* id 3, wireType 0 =*/24).uint32(message.interval);
                return writer;
            };
    
            /**
             * Encodes the specified Interval message, length delimited. Does not implicitly {@link fk_app.Interval.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.Interval
             * @static
             * @param {fk_app.IInterval} message Interval message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Interval.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes an Interval message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.Interval
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.Interval} Interval
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Interval.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.Interval();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.start = reader.uint64();
                        break;
                    case 2:
                        message.end = reader.uint64();
                        break;
                    case 3:
                        message.interval = reader.uint32();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes an Interval message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.Interval
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.Interval} Interval
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Interval.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies an Interval message.
             * @function verify
             * @memberof fk_app.Interval
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            Interval.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.start != null && message.hasOwnProperty("start"))
                    if (!$util.isInteger(message.start) && !(message.start && $util.isInteger(message.start.low) && $util.isInteger(message.start.high)))
                        return "start: integer|Long expected";
                if (message.end != null && message.hasOwnProperty("end"))
                    if (!$util.isInteger(message.end) && !(message.end && $util.isInteger(message.end.low) && $util.isInteger(message.end.high)))
                        return "end: integer|Long expected";
                if (message.interval != null && message.hasOwnProperty("interval"))
                    if (!$util.isInteger(message.interval))
                        return "interval: integer expected";
                return null;
            };
    
            /**
             * Creates an Interval message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.Interval
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.Interval} Interval
             */
            Interval.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.Interval)
                    return object;
                var message = new $root.fk_app.Interval();
                if (object.start != null)
                    if ($util.Long)
                        (message.start = $util.Long.fromValue(object.start)).unsigned = true;
                    else if (typeof object.start === "string")
                        message.start = parseInt(object.start, 10);
                    else if (typeof object.start === "number")
                        message.start = object.start;
                    else if (typeof object.start === "object")
                        message.start = new $util.LongBits(object.start.low >>> 0, object.start.high >>> 0).toNumber(true);
                if (object.end != null)
                    if ($util.Long)
                        (message.end = $util.Long.fromValue(object.end)).unsigned = true;
                    else if (typeof object.end === "string")
                        message.end = parseInt(object.end, 10);
                    else if (typeof object.end === "number")
                        message.end = object.end;
                    else if (typeof object.end === "object")
                        message.end = new $util.LongBits(object.end.low >>> 0, object.end.high >>> 0).toNumber(true);
                if (object.interval != null)
                    message.interval = object.interval >>> 0;
                return message;
            };
    
            /**
             * Creates a plain object from an Interval message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.Interval
             * @static
             * @param {fk_app.Interval} message Interval
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            Interval.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.start = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.start = options.longs === String ? "0" : 0;
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.end = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.end = options.longs === String ? "0" : 0;
                    object.interval = 0;
                }
                if (message.start != null && message.hasOwnProperty("start"))
                    if (typeof message.start === "number")
                        object.start = options.longs === String ? String(message.start) : message.start;
                    else
                        object.start = options.longs === String ? $util.Long.prototype.toString.call(message.start) : options.longs === Number ? new $util.LongBits(message.start.low >>> 0, message.start.high >>> 0).toNumber(true) : message.start;
                if (message.end != null && message.hasOwnProperty("end"))
                    if (typeof message.end === "number")
                        object.end = options.longs === String ? String(message.end) : message.end;
                    else
                        object.end = options.longs === String ? $util.Long.prototype.toString.call(message.end) : options.longs === Number ? new $util.LongBits(message.end.low >>> 0, message.end.high >>> 0).toNumber(true) : message.end;
                if (message.interval != null && message.hasOwnProperty("interval"))
                    object.interval = message.interval;
                return object;
            };
    
            /**
             * Converts this Interval to JSON.
             * @function toJSON
             * @memberof fk_app.Interval
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Interval.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Interval;
        })();
    
        fk_app.Schedule = (function() {
    
            /**
             * Properties of a Schedule.
             * @memberof fk_app
             * @interface ISchedule
             * @property {Uint8Array} [cron] Schedule cron
             * @property {number} [interval] Schedule interval
             * @property {number} [repeated] Schedule repeated
             * @property {number} [duration] Schedule duration
             * @property {number} [jitter] Schedule jitter
             * @property {Array.<fk_app.IInterval>} [intervals] Schedule intervals
             */
    
            /**
             * Constructs a new Schedule.
             * @memberof fk_app
             * @classdesc Represents a Schedule.
             * @constructor
             * @param {fk_app.ISchedule=} [properties] Properties to set
             */
            function Schedule(properties) {
                this.intervals = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Schedule cron.
             * @member {Uint8Array}cron
             * @memberof fk_app.Schedule
             * @instance
             */
            Schedule.prototype.cron = $util.newBuffer([]);
    
            /**
             * Schedule interval.
             * @member {number}interval
             * @memberof fk_app.Schedule
             * @instance
             */
            Schedule.prototype.interval = 0;
    
            /**
             * Schedule repeated.
             * @member {number}repeated
             * @memberof fk_app.Schedule
             * @instance
             */
            Schedule.prototype.repeated = 0;
    
            /**
             * Schedule duration.
             * @member {number}duration
             * @memberof fk_app.Schedule
             * @instance
             */
            Schedule.prototype.duration = 0;
    
            /**
             * Schedule jitter.
             * @member {number}jitter
             * @memberof fk_app.Schedule
             * @instance
             */
            Schedule.prototype.jitter = 0;
    
            /**
             * Schedule intervals.
             * @member {Array.<fk_app.IInterval>}intervals
             * @memberof fk_app.Schedule
             * @instance
             */
            Schedule.prototype.intervals = $util.emptyArray;
    
            /**
             * Creates a new Schedule instance using the specified properties.
             * @function create
             * @memberof fk_app.Schedule
             * @static
             * @param {fk_app.ISchedule=} [properties] Properties to set
             * @returns {fk_app.Schedule} Schedule instance
             */
            Schedule.create = function create(properties) {
                return new Schedule(properties);
            };
    
            /**
             * Encodes the specified Schedule message. Does not implicitly {@link fk_app.Schedule.verify|verify} messages.
             * @function encode
             * @memberof fk_app.Schedule
             * @static
             * @param {fk_app.ISchedule} message Schedule message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Schedule.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.cron != null && message.hasOwnProperty("cron"))
                    writer.uint32(/* id 1, wireType 2 =*/10).bytes(message.cron);
                if (message.interval != null && message.hasOwnProperty("interval"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint32(message.interval);
                if (message.repeated != null && message.hasOwnProperty("repeated"))
                    writer.uint32(/* id 3, wireType 0 =*/24).uint32(message.repeated);
                if (message.duration != null && message.hasOwnProperty("duration"))
                    writer.uint32(/* id 4, wireType 0 =*/32).uint32(message.duration);
                if (message.jitter != null && message.hasOwnProperty("jitter"))
                    writer.uint32(/* id 5, wireType 0 =*/40).uint32(message.jitter);
                if (message.intervals != null && message.intervals.length)
                    for (var i = 0; i < message.intervals.length; ++i)
                        $root.fk_app.Interval.encode(message.intervals[i], writer.uint32(/* id 6, wireType 2 =*/50).fork()).ldelim();
                return writer;
            };
    
            /**
             * Encodes the specified Schedule message, length delimited. Does not implicitly {@link fk_app.Schedule.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.Schedule
             * @static
             * @param {fk_app.ISchedule} message Schedule message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Schedule.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a Schedule message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.Schedule
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.Schedule} Schedule
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Schedule.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.Schedule();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.cron = reader.bytes();
                        break;
                    case 2:
                        message.interval = reader.uint32();
                        break;
                    case 3:
                        message.repeated = reader.uint32();
                        break;
                    case 4:
                        message.duration = reader.uint32();
                        break;
                    case 5:
                        message.jitter = reader.uint32();
                        break;
                    case 6:
                        if (!(message.intervals && message.intervals.length))
                            message.intervals = [];
                        message.intervals.push($root.fk_app.Interval.decode(reader, reader.uint32()));
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a Schedule message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.Schedule
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.Schedule} Schedule
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Schedule.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a Schedule message.
             * @function verify
             * @memberof fk_app.Schedule
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            Schedule.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.cron != null && message.hasOwnProperty("cron"))
                    if (!(message.cron && typeof message.cron.length === "number" || $util.isString(message.cron)))
                        return "cron: buffer expected";
                if (message.interval != null && message.hasOwnProperty("interval"))
                    if (!$util.isInteger(message.interval))
                        return "interval: integer expected";
                if (message.repeated != null && message.hasOwnProperty("repeated"))
                    if (!$util.isInteger(message.repeated))
                        return "repeated: integer expected";
                if (message.duration != null && message.hasOwnProperty("duration"))
                    if (!$util.isInteger(message.duration))
                        return "duration: integer expected";
                if (message.jitter != null && message.hasOwnProperty("jitter"))
                    if (!$util.isInteger(message.jitter))
                        return "jitter: integer expected";
                if (message.intervals != null && message.hasOwnProperty("intervals")) {
                    if (!Array.isArray(message.intervals))
                        return "intervals: array expected";
                    for (var i = 0; i < message.intervals.length; ++i) {
                        var error = $root.fk_app.Interval.verify(message.intervals[i]);
                        if (error)
                            return "intervals." + error;
                    }
                }
                return null;
            };
    
            /**
             * Creates a Schedule message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.Schedule
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.Schedule} Schedule
             */
            Schedule.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.Schedule)
                    return object;
                var message = new $root.fk_app.Schedule();
                if (object.cron != null)
                    if (typeof object.cron === "string")
                        $util.base64.decode(object.cron, message.cron = $util.newBuffer($util.base64.length(object.cron)), 0);
                    else if (object.cron.length)
                        message.cron = object.cron;
                if (object.interval != null)
                    message.interval = object.interval >>> 0;
                if (object.repeated != null)
                    message.repeated = object.repeated >>> 0;
                if (object.duration != null)
                    message.duration = object.duration >>> 0;
                if (object.jitter != null)
                    message.jitter = object.jitter >>> 0;
                if (object.intervals) {
                    if (!Array.isArray(object.intervals))
                        throw TypeError(".fk_app.Schedule.intervals: array expected");
                    message.intervals = [];
                    for (var i = 0; i < object.intervals.length; ++i) {
                        if (typeof object.intervals[i] !== "object")
                            throw TypeError(".fk_app.Schedule.intervals: object expected");
                        message.intervals[i] = $root.fk_app.Interval.fromObject(object.intervals[i]);
                    }
                }
                return message;
            };
    
            /**
             * Creates a plain object from a Schedule message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.Schedule
             * @static
             * @param {fk_app.Schedule} message Schedule
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            Schedule.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults)
                    object.intervals = [];
                if (options.defaults) {
                    object.cron = options.bytes === String ? "" : [];
                    object.interval = 0;
                    object.repeated = 0;
                    object.duration = 0;
                    object.jitter = 0;
                }
                if (message.cron != null && message.hasOwnProperty("cron"))
                    object.cron = options.bytes === String ? $util.base64.encode(message.cron, 0, message.cron.length) : options.bytes === Array ? Array.prototype.slice.call(message.cron) : message.cron;
                if (message.interval != null && message.hasOwnProperty("interval"))
                    object.interval = message.interval;
                if (message.repeated != null && message.hasOwnProperty("repeated"))
                    object.repeated = message.repeated;
                if (message.duration != null && message.hasOwnProperty("duration"))
                    object.duration = message.duration;
                if (message.jitter != null && message.hasOwnProperty("jitter"))
                    object.jitter = message.jitter;
                if (message.intervals && message.intervals.length) {
                    object.intervals = [];
                    for (var j = 0; j < message.intervals.length; ++j)
                        object.intervals[j] = $root.fk_app.Interval.toObject(message.intervals[j], options);
                }
                return object;
            };
    
            /**
             * Converts this Schedule to JSON.
             * @function toJSON
             * @memberof fk_app.Schedule
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Schedule.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Schedule;
        })();
    
        fk_app.Schedules = (function() {
    
            /**
             * Properties of a Schedules.
             * @memberof fk_app
             * @interface ISchedules
             * @property {boolean} [modifying] Schedules modifying
             * @property {fk_app.ISchedule} [readings] Schedules readings
             * @property {fk_app.ISchedule} [lora] Schedules lora
             * @property {fk_app.ISchedule} [network] Schedules network
             * @property {fk_app.ISchedule} [gps] Schedules gps
             */
    
            /**
             * Constructs a new Schedules.
             * @memberof fk_app
             * @classdesc Represents a Schedules.
             * @constructor
             * @param {fk_app.ISchedules=} [properties] Properties to set
             */
            function Schedules(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Schedules modifying.
             * @member {boolean}modifying
             * @memberof fk_app.Schedules
             * @instance
             */
            Schedules.prototype.modifying = false;
    
            /**
             * Schedules readings.
             * @member {(fk_app.ISchedule|null|undefined)}readings
             * @memberof fk_app.Schedules
             * @instance
             */
            Schedules.prototype.readings = null;
    
            /**
             * Schedules lora.
             * @member {(fk_app.ISchedule|null|undefined)}lora
             * @memberof fk_app.Schedules
             * @instance
             */
            Schedules.prototype.lora = null;
    
            /**
             * Schedules network.
             * @member {(fk_app.ISchedule|null|undefined)}network
             * @memberof fk_app.Schedules
             * @instance
             */
            Schedules.prototype.network = null;
    
            /**
             * Schedules gps.
             * @member {(fk_app.ISchedule|null|undefined)}gps
             * @memberof fk_app.Schedules
             * @instance
             */
            Schedules.prototype.gps = null;
    
            /**
             * Creates a new Schedules instance using the specified properties.
             * @function create
             * @memberof fk_app.Schedules
             * @static
             * @param {fk_app.ISchedules=} [properties] Properties to set
             * @returns {fk_app.Schedules} Schedules instance
             */
            Schedules.create = function create(properties) {
                return new Schedules(properties);
            };
    
            /**
             * Encodes the specified Schedules message. Does not implicitly {@link fk_app.Schedules.verify|verify} messages.
             * @function encode
             * @memberof fk_app.Schedules
             * @static
             * @param {fk_app.ISchedules} message Schedules message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Schedules.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.modifying != null && message.hasOwnProperty("modifying"))
                    writer.uint32(/* id 1, wireType 0 =*/8).bool(message.modifying);
                if (message.readings != null && message.hasOwnProperty("readings"))
                    $root.fk_app.Schedule.encode(message.readings, writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
                if (message.lora != null && message.hasOwnProperty("lora"))
                    $root.fk_app.Schedule.encode(message.lora, writer.uint32(/* id 3, wireType 2 =*/26).fork()).ldelim();
                if (message.network != null && message.hasOwnProperty("network"))
                    $root.fk_app.Schedule.encode(message.network, writer.uint32(/* id 4, wireType 2 =*/34).fork()).ldelim();
                if (message.gps != null && message.hasOwnProperty("gps"))
                    $root.fk_app.Schedule.encode(message.gps, writer.uint32(/* id 5, wireType 2 =*/42).fork()).ldelim();
                return writer;
            };
    
            /**
             * Encodes the specified Schedules message, length delimited. Does not implicitly {@link fk_app.Schedules.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.Schedules
             * @static
             * @param {fk_app.ISchedules} message Schedules message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Schedules.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a Schedules message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.Schedules
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.Schedules} Schedules
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Schedules.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.Schedules();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.modifying = reader.bool();
                        break;
                    case 2:
                        message.readings = $root.fk_app.Schedule.decode(reader, reader.uint32());
                        break;
                    case 3:
                        message.lora = $root.fk_app.Schedule.decode(reader, reader.uint32());
                        break;
                    case 4:
                        message.network = $root.fk_app.Schedule.decode(reader, reader.uint32());
                        break;
                    case 5:
                        message.gps = $root.fk_app.Schedule.decode(reader, reader.uint32());
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a Schedules message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.Schedules
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.Schedules} Schedules
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Schedules.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a Schedules message.
             * @function verify
             * @memberof fk_app.Schedules
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            Schedules.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.modifying != null && message.hasOwnProperty("modifying"))
                    if (typeof message.modifying !== "boolean")
                        return "modifying: boolean expected";
                if (message.readings != null && message.hasOwnProperty("readings")) {
                    var error = $root.fk_app.Schedule.verify(message.readings);
                    if (error)
                        return "readings." + error;
                }
                if (message.lora != null && message.hasOwnProperty("lora")) {
                    error = $root.fk_app.Schedule.verify(message.lora);
                    if (error)
                        return "lora." + error;
                }
                if (message.network != null && message.hasOwnProperty("network")) {
                    error = $root.fk_app.Schedule.verify(message.network);
                    if (error)
                        return "network." + error;
                }
                if (message.gps != null && message.hasOwnProperty("gps")) {
                    error = $root.fk_app.Schedule.verify(message.gps);
                    if (error)
                        return "gps." + error;
                }
                return null;
            };
    
            /**
             * Creates a Schedules message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.Schedules
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.Schedules} Schedules
             */
            Schedules.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.Schedules)
                    return object;
                var message = new $root.fk_app.Schedules();
                if (object.modifying != null)
                    message.modifying = Boolean(object.modifying);
                if (object.readings != null) {
                    if (typeof object.readings !== "object")
                        throw TypeError(".fk_app.Schedules.readings: object expected");
                    message.readings = $root.fk_app.Schedule.fromObject(object.readings);
                }
                if (object.lora != null) {
                    if (typeof object.lora !== "object")
                        throw TypeError(".fk_app.Schedules.lora: object expected");
                    message.lora = $root.fk_app.Schedule.fromObject(object.lora);
                }
                if (object.network != null) {
                    if (typeof object.network !== "object")
                        throw TypeError(".fk_app.Schedules.network: object expected");
                    message.network = $root.fk_app.Schedule.fromObject(object.network);
                }
                if (object.gps != null) {
                    if (typeof object.gps !== "object")
                        throw TypeError(".fk_app.Schedules.gps: object expected");
                    message.gps = $root.fk_app.Schedule.fromObject(object.gps);
                }
                return message;
            };
    
            /**
             * Creates a plain object from a Schedules message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.Schedules
             * @static
             * @param {fk_app.Schedules} message Schedules
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            Schedules.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.modifying = false;
                    object.readings = null;
                    object.lora = null;
                    object.network = null;
                    object.gps = null;
                }
                if (message.modifying != null && message.hasOwnProperty("modifying"))
                    object.modifying = message.modifying;
                if (message.readings != null && message.hasOwnProperty("readings"))
                    object.readings = $root.fk_app.Schedule.toObject(message.readings, options);
                if (message.lora != null && message.hasOwnProperty("lora"))
                    object.lora = $root.fk_app.Schedule.toObject(message.lora, options);
                if (message.network != null && message.hasOwnProperty("network"))
                    object.network = $root.fk_app.Schedule.toObject(message.network, options);
                if (message.gps != null && message.hasOwnProperty("gps"))
                    object.gps = $root.fk_app.Schedule.toObject(message.gps, options);
                return object;
            };
    
            /**
             * Converts this Schedules to JSON.
             * @function toJSON
             * @memberof fk_app.Schedules
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Schedules.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Schedules;
        })();
    
        fk_app.HardwareStatus = (function() {
    
            /**
             * Properties of a HardwareStatus.
             * @memberof fk_app
             * @interface IHardwareStatus
             */
    
            /**
             * Constructs a new HardwareStatus.
             * @memberof fk_app
             * @classdesc Represents a HardwareStatus.
             * @constructor
             * @param {fk_app.IHardwareStatus=} [properties] Properties to set
             */
            function HardwareStatus(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Creates a new HardwareStatus instance using the specified properties.
             * @function create
             * @memberof fk_app.HardwareStatus
             * @static
             * @param {fk_app.IHardwareStatus=} [properties] Properties to set
             * @returns {fk_app.HardwareStatus} HardwareStatus instance
             */
            HardwareStatus.create = function create(properties) {
                return new HardwareStatus(properties);
            };
    
            /**
             * Encodes the specified HardwareStatus message. Does not implicitly {@link fk_app.HardwareStatus.verify|verify} messages.
             * @function encode
             * @memberof fk_app.HardwareStatus
             * @static
             * @param {fk_app.IHardwareStatus} message HardwareStatus message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            HardwareStatus.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                return writer;
            };
    
            /**
             * Encodes the specified HardwareStatus message, length delimited. Does not implicitly {@link fk_app.HardwareStatus.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.HardwareStatus
             * @static
             * @param {fk_app.IHardwareStatus} message HardwareStatus message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            HardwareStatus.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a HardwareStatus message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.HardwareStatus
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.HardwareStatus} HardwareStatus
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            HardwareStatus.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.HardwareStatus();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a HardwareStatus message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.HardwareStatus
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.HardwareStatus} HardwareStatus
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            HardwareStatus.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a HardwareStatus message.
             * @function verify
             * @memberof fk_app.HardwareStatus
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            HardwareStatus.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                return null;
            };
    
            /**
             * Creates a HardwareStatus message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.HardwareStatus
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.HardwareStatus} HardwareStatus
             */
            HardwareStatus.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.HardwareStatus)
                    return object;
                return new $root.fk_app.HardwareStatus();
            };
    
            /**
             * Creates a plain object from a HardwareStatus message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.HardwareStatus
             * @static
             * @param {fk_app.HardwareStatus} message HardwareStatus
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            HardwareStatus.toObject = function toObject() {
                return {};
            };
    
            /**
             * Converts this HardwareStatus to JSON.
             * @function toJSON
             * @memberof fk_app.HardwareStatus
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            HardwareStatus.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return HardwareStatus;
        })();
    
        fk_app.GpsStatus = (function() {
    
            /**
             * Properties of a GpsStatus.
             * @memberof fk_app
             * @interface IGpsStatus
             * @property {number} [enabled] GpsStatus enabled
             * @property {number} [fix] GpsStatus fix
             * @property {number|Long} [time] GpsStatus time
             * @property {number} [satellites] GpsStatus satellites
             * @property {number} [longitude] GpsStatus longitude
             * @property {number} [latitude] GpsStatus latitude
             * @property {number} [altitude] GpsStatus altitude
             */
    
            /**
             * Constructs a new GpsStatus.
             * @memberof fk_app
             * @classdesc Represents a GpsStatus.
             * @constructor
             * @param {fk_app.IGpsStatus=} [properties] Properties to set
             */
            function GpsStatus(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * GpsStatus enabled.
             * @member {number}enabled
             * @memberof fk_app.GpsStatus
             * @instance
             */
            GpsStatus.prototype.enabled = 0;
    
            /**
             * GpsStatus fix.
             * @member {number}fix
             * @memberof fk_app.GpsStatus
             * @instance
             */
            GpsStatus.prototype.fix = 0;
    
            /**
             * GpsStatus time.
             * @member {number|Long}time
             * @memberof fk_app.GpsStatus
             * @instance
             */
            GpsStatus.prototype.time = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * GpsStatus satellites.
             * @member {number}satellites
             * @memberof fk_app.GpsStatus
             * @instance
             */
            GpsStatus.prototype.satellites = 0;
    
            /**
             * GpsStatus longitude.
             * @member {number}longitude
             * @memberof fk_app.GpsStatus
             * @instance
             */
            GpsStatus.prototype.longitude = 0;
    
            /**
             * GpsStatus latitude.
             * @member {number}latitude
             * @memberof fk_app.GpsStatus
             * @instance
             */
            GpsStatus.prototype.latitude = 0;
    
            /**
             * GpsStatus altitude.
             * @member {number}altitude
             * @memberof fk_app.GpsStatus
             * @instance
             */
            GpsStatus.prototype.altitude = 0;
    
            /**
             * Creates a new GpsStatus instance using the specified properties.
             * @function create
             * @memberof fk_app.GpsStatus
             * @static
             * @param {fk_app.IGpsStatus=} [properties] Properties to set
             * @returns {fk_app.GpsStatus} GpsStatus instance
             */
            GpsStatus.create = function create(properties) {
                return new GpsStatus(properties);
            };
    
            /**
             * Encodes the specified GpsStatus message. Does not implicitly {@link fk_app.GpsStatus.verify|verify} messages.
             * @function encode
             * @memberof fk_app.GpsStatus
             * @static
             * @param {fk_app.IGpsStatus} message GpsStatus message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            GpsStatus.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.fix != null && message.hasOwnProperty("fix"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.fix);
                if (message.time != null && message.hasOwnProperty("time"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint64(message.time);
                if (message.satellites != null && message.hasOwnProperty("satellites"))
                    writer.uint32(/* id 3, wireType 0 =*/24).uint32(message.satellites);
                if (message.longitude != null && message.hasOwnProperty("longitude"))
                    writer.uint32(/* id 4, wireType 5 =*/37).float(message.longitude);
                if (message.latitude != null && message.hasOwnProperty("latitude"))
                    writer.uint32(/* id 5, wireType 5 =*/45).float(message.latitude);
                if (message.altitude != null && message.hasOwnProperty("altitude"))
                    writer.uint32(/* id 6, wireType 5 =*/53).float(message.altitude);
                if (message.enabled != null && message.hasOwnProperty("enabled"))
                    writer.uint32(/* id 7, wireType 0 =*/56).uint32(message.enabled);
                return writer;
            };
    
            /**
             * Encodes the specified GpsStatus message, length delimited. Does not implicitly {@link fk_app.GpsStatus.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.GpsStatus
             * @static
             * @param {fk_app.IGpsStatus} message GpsStatus message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            GpsStatus.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a GpsStatus message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.GpsStatus
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.GpsStatus} GpsStatus
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            GpsStatus.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.GpsStatus();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 7:
                        message.enabled = reader.uint32();
                        break;
                    case 1:
                        message.fix = reader.uint32();
                        break;
                    case 2:
                        message.time = reader.uint64();
                        break;
                    case 3:
                        message.satellites = reader.uint32();
                        break;
                    case 4:
                        message.longitude = reader.float();
                        break;
                    case 5:
                        message.latitude = reader.float();
                        break;
                    case 6:
                        message.altitude = reader.float();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a GpsStatus message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.GpsStatus
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.GpsStatus} GpsStatus
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            GpsStatus.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a GpsStatus message.
             * @function verify
             * @memberof fk_app.GpsStatus
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            GpsStatus.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.enabled != null && message.hasOwnProperty("enabled"))
                    if (!$util.isInteger(message.enabled))
                        return "enabled: integer expected";
                if (message.fix != null && message.hasOwnProperty("fix"))
                    if (!$util.isInteger(message.fix))
                        return "fix: integer expected";
                if (message.time != null && message.hasOwnProperty("time"))
                    if (!$util.isInteger(message.time) && !(message.time && $util.isInteger(message.time.low) && $util.isInteger(message.time.high)))
                        return "time: integer|Long expected";
                if (message.satellites != null && message.hasOwnProperty("satellites"))
                    if (!$util.isInteger(message.satellites))
                        return "satellites: integer expected";
                if (message.longitude != null && message.hasOwnProperty("longitude"))
                    if (typeof message.longitude !== "number")
                        return "longitude: number expected";
                if (message.latitude != null && message.hasOwnProperty("latitude"))
                    if (typeof message.latitude !== "number")
                        return "latitude: number expected";
                if (message.altitude != null && message.hasOwnProperty("altitude"))
                    if (typeof message.altitude !== "number")
                        return "altitude: number expected";
                return null;
            };
    
            /**
             * Creates a GpsStatus message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.GpsStatus
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.GpsStatus} GpsStatus
             */
            GpsStatus.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.GpsStatus)
                    return object;
                var message = new $root.fk_app.GpsStatus();
                if (object.enabled != null)
                    message.enabled = object.enabled >>> 0;
                if (object.fix != null)
                    message.fix = object.fix >>> 0;
                if (object.time != null)
                    if ($util.Long)
                        (message.time = $util.Long.fromValue(object.time)).unsigned = true;
                    else if (typeof object.time === "string")
                        message.time = parseInt(object.time, 10);
                    else if (typeof object.time === "number")
                        message.time = object.time;
                    else if (typeof object.time === "object")
                        message.time = new $util.LongBits(object.time.low >>> 0, object.time.high >>> 0).toNumber(true);
                if (object.satellites != null)
                    message.satellites = object.satellites >>> 0;
                if (object.longitude != null)
                    message.longitude = Number(object.longitude);
                if (object.latitude != null)
                    message.latitude = Number(object.latitude);
                if (object.altitude != null)
                    message.altitude = Number(object.altitude);
                return message;
            };
    
            /**
             * Creates a plain object from a GpsStatus message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.GpsStatus
             * @static
             * @param {fk_app.GpsStatus} message GpsStatus
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            GpsStatus.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.fix = 0;
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.time = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.time = options.longs === String ? "0" : 0;
                    object.satellites = 0;
                    object.longitude = 0;
                    object.latitude = 0;
                    object.altitude = 0;
                    object.enabled = 0;
                }
                if (message.fix != null && message.hasOwnProperty("fix"))
                    object.fix = message.fix;
                if (message.time != null && message.hasOwnProperty("time"))
                    if (typeof message.time === "number")
                        object.time = options.longs === String ? String(message.time) : message.time;
                    else
                        object.time = options.longs === String ? $util.Long.prototype.toString.call(message.time) : options.longs === Number ? new $util.LongBits(message.time.low >>> 0, message.time.high >>> 0).toNumber(true) : message.time;
                if (message.satellites != null && message.hasOwnProperty("satellites"))
                    object.satellites = message.satellites;
                if (message.longitude != null && message.hasOwnProperty("longitude"))
                    object.longitude = options.json && !isFinite(message.longitude) ? String(message.longitude) : message.longitude;
                if (message.latitude != null && message.hasOwnProperty("latitude"))
                    object.latitude = options.json && !isFinite(message.latitude) ? String(message.latitude) : message.latitude;
                if (message.altitude != null && message.hasOwnProperty("altitude"))
                    object.altitude = options.json && !isFinite(message.altitude) ? String(message.altitude) : message.altitude;
                if (message.enabled != null && message.hasOwnProperty("enabled"))
                    object.enabled = message.enabled;
                return object;
            };
    
            /**
             * Converts this GpsStatus to JSON.
             * @function toJSON
             * @memberof fk_app.GpsStatus
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            GpsStatus.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return GpsStatus;
        })();
    
        fk_app.MemoryStatus = (function() {
    
            /**
             * Properties of a MemoryStatus.
             * @memberof fk_app
             * @interface IMemoryStatus
             * @property {number} [sramAvailable] MemoryStatus sramAvailable
             * @property {number} [programFlashAvailable] MemoryStatus programFlashAvailable
             * @property {number} [extendedMemoryAvailable] MemoryStatus extendedMemoryAvailable
             * @property {number} [dataMemoryInstalled] MemoryStatus dataMemoryInstalled
             * @property {number} [dataMemoryUsed] MemoryStatus dataMemoryUsed
             * @property {number} [dataMemoryConsumption] MemoryStatus dataMemoryConsumption
             * @property {Array.<fk_app.IFirmware>} [firmware] MemoryStatus firmware
             */
    
            /**
             * Constructs a new MemoryStatus.
             * @memberof fk_app
             * @classdesc Represents a MemoryStatus.
             * @constructor
             * @param {fk_app.IMemoryStatus=} [properties] Properties to set
             */
            function MemoryStatus(properties) {
                this.firmware = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * MemoryStatus sramAvailable.
             * @member {number}sramAvailable
             * @memberof fk_app.MemoryStatus
             * @instance
             */
            MemoryStatus.prototype.sramAvailable = 0;
    
            /**
             * MemoryStatus programFlashAvailable.
             * @member {number}programFlashAvailable
             * @memberof fk_app.MemoryStatus
             * @instance
             */
            MemoryStatus.prototype.programFlashAvailable = 0;
    
            /**
             * MemoryStatus extendedMemoryAvailable.
             * @member {number}extendedMemoryAvailable
             * @memberof fk_app.MemoryStatus
             * @instance
             */
            MemoryStatus.prototype.extendedMemoryAvailable = 0;
    
            /**
             * MemoryStatus dataMemoryInstalled.
             * @member {number}dataMemoryInstalled
             * @memberof fk_app.MemoryStatus
             * @instance
             */
            MemoryStatus.prototype.dataMemoryInstalled = 0;
    
            /**
             * MemoryStatus dataMemoryUsed.
             * @member {number}dataMemoryUsed
             * @memberof fk_app.MemoryStatus
             * @instance
             */
            MemoryStatus.prototype.dataMemoryUsed = 0;
    
            /**
             * MemoryStatus dataMemoryConsumption.
             * @member {number}dataMemoryConsumption
             * @memberof fk_app.MemoryStatus
             * @instance
             */
            MemoryStatus.prototype.dataMemoryConsumption = 0;
    
            /**
             * MemoryStatus firmware.
             * @member {Array.<fk_app.IFirmware>}firmware
             * @memberof fk_app.MemoryStatus
             * @instance
             */
            MemoryStatus.prototype.firmware = $util.emptyArray;
    
            /**
             * Creates a new MemoryStatus instance using the specified properties.
             * @function create
             * @memberof fk_app.MemoryStatus
             * @static
             * @param {fk_app.IMemoryStatus=} [properties] Properties to set
             * @returns {fk_app.MemoryStatus} MemoryStatus instance
             */
            MemoryStatus.create = function create(properties) {
                return new MemoryStatus(properties);
            };
    
            /**
             * Encodes the specified MemoryStatus message. Does not implicitly {@link fk_app.MemoryStatus.verify|verify} messages.
             * @function encode
             * @memberof fk_app.MemoryStatus
             * @static
             * @param {fk_app.IMemoryStatus} message MemoryStatus message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            MemoryStatus.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.sramAvailable != null && message.hasOwnProperty("sramAvailable"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.sramAvailable);
                if (message.programFlashAvailable != null && message.hasOwnProperty("programFlashAvailable"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint32(message.programFlashAvailable);
                if (message.extendedMemoryAvailable != null && message.hasOwnProperty("extendedMemoryAvailable"))
                    writer.uint32(/* id 3, wireType 0 =*/24).uint32(message.extendedMemoryAvailable);
                if (message.dataMemoryInstalled != null && message.hasOwnProperty("dataMemoryInstalled"))
                    writer.uint32(/* id 4, wireType 0 =*/32).uint32(message.dataMemoryInstalled);
                if (message.dataMemoryUsed != null && message.hasOwnProperty("dataMemoryUsed"))
                    writer.uint32(/* id 5, wireType 0 =*/40).uint32(message.dataMemoryUsed);
                if (message.dataMemoryConsumption != null && message.hasOwnProperty("dataMemoryConsumption"))
                    writer.uint32(/* id 6, wireType 5 =*/53).float(message.dataMemoryConsumption);
                if (message.firmware != null && message.firmware.length)
                    for (var i = 0; i < message.firmware.length; ++i)
                        $root.fk_app.Firmware.encode(message.firmware[i], writer.uint32(/* id 7, wireType 2 =*/58).fork()).ldelim();
                return writer;
            };
    
            /**
             * Encodes the specified MemoryStatus message, length delimited. Does not implicitly {@link fk_app.MemoryStatus.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.MemoryStatus
             * @static
             * @param {fk_app.IMemoryStatus} message MemoryStatus message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            MemoryStatus.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a MemoryStatus message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.MemoryStatus
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.MemoryStatus} MemoryStatus
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            MemoryStatus.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.MemoryStatus();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.sramAvailable = reader.uint32();
                        break;
                    case 2:
                        message.programFlashAvailable = reader.uint32();
                        break;
                    case 3:
                        message.extendedMemoryAvailable = reader.uint32();
                        break;
                    case 4:
                        message.dataMemoryInstalled = reader.uint32();
                        break;
                    case 5:
                        message.dataMemoryUsed = reader.uint32();
                        break;
                    case 6:
                        message.dataMemoryConsumption = reader.float();
                        break;
                    case 7:
                        if (!(message.firmware && message.firmware.length))
                            message.firmware = [];
                        message.firmware.push($root.fk_app.Firmware.decode(reader, reader.uint32()));
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a MemoryStatus message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.MemoryStatus
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.MemoryStatus} MemoryStatus
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            MemoryStatus.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a MemoryStatus message.
             * @function verify
             * @memberof fk_app.MemoryStatus
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            MemoryStatus.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.sramAvailable != null && message.hasOwnProperty("sramAvailable"))
                    if (!$util.isInteger(message.sramAvailable))
                        return "sramAvailable: integer expected";
                if (message.programFlashAvailable != null && message.hasOwnProperty("programFlashAvailable"))
                    if (!$util.isInteger(message.programFlashAvailable))
                        return "programFlashAvailable: integer expected";
                if (message.extendedMemoryAvailable != null && message.hasOwnProperty("extendedMemoryAvailable"))
                    if (!$util.isInteger(message.extendedMemoryAvailable))
                        return "extendedMemoryAvailable: integer expected";
                if (message.dataMemoryInstalled != null && message.hasOwnProperty("dataMemoryInstalled"))
                    if (!$util.isInteger(message.dataMemoryInstalled))
                        return "dataMemoryInstalled: integer expected";
                if (message.dataMemoryUsed != null && message.hasOwnProperty("dataMemoryUsed"))
                    if (!$util.isInteger(message.dataMemoryUsed))
                        return "dataMemoryUsed: integer expected";
                if (message.dataMemoryConsumption != null && message.hasOwnProperty("dataMemoryConsumption"))
                    if (typeof message.dataMemoryConsumption !== "number")
                        return "dataMemoryConsumption: number expected";
                if (message.firmware != null && message.hasOwnProperty("firmware")) {
                    if (!Array.isArray(message.firmware))
                        return "firmware: array expected";
                    for (var i = 0; i < message.firmware.length; ++i) {
                        var error = $root.fk_app.Firmware.verify(message.firmware[i]);
                        if (error)
                            return "firmware." + error;
                    }
                }
                return null;
            };
    
            /**
             * Creates a MemoryStatus message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.MemoryStatus
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.MemoryStatus} MemoryStatus
             */
            MemoryStatus.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.MemoryStatus)
                    return object;
                var message = new $root.fk_app.MemoryStatus();
                if (object.sramAvailable != null)
                    message.sramAvailable = object.sramAvailable >>> 0;
                if (object.programFlashAvailable != null)
                    message.programFlashAvailable = object.programFlashAvailable >>> 0;
                if (object.extendedMemoryAvailable != null)
                    message.extendedMemoryAvailable = object.extendedMemoryAvailable >>> 0;
                if (object.dataMemoryInstalled != null)
                    message.dataMemoryInstalled = object.dataMemoryInstalled >>> 0;
                if (object.dataMemoryUsed != null)
                    message.dataMemoryUsed = object.dataMemoryUsed >>> 0;
                if (object.dataMemoryConsumption != null)
                    message.dataMemoryConsumption = Number(object.dataMemoryConsumption);
                if (object.firmware) {
                    if (!Array.isArray(object.firmware))
                        throw TypeError(".fk_app.MemoryStatus.firmware: array expected");
                    message.firmware = [];
                    for (var i = 0; i < object.firmware.length; ++i) {
                        if (typeof object.firmware[i] !== "object")
                            throw TypeError(".fk_app.MemoryStatus.firmware: object expected");
                        message.firmware[i] = $root.fk_app.Firmware.fromObject(object.firmware[i]);
                    }
                }
                return message;
            };
    
            /**
             * Creates a plain object from a MemoryStatus message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.MemoryStatus
             * @static
             * @param {fk_app.MemoryStatus} message MemoryStatus
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            MemoryStatus.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults)
                    object.firmware = [];
                if (options.defaults) {
                    object.sramAvailable = 0;
                    object.programFlashAvailable = 0;
                    object.extendedMemoryAvailable = 0;
                    object.dataMemoryInstalled = 0;
                    object.dataMemoryUsed = 0;
                    object.dataMemoryConsumption = 0;
                }
                if (message.sramAvailable != null && message.hasOwnProperty("sramAvailable"))
                    object.sramAvailable = message.sramAvailable;
                if (message.programFlashAvailable != null && message.hasOwnProperty("programFlashAvailable"))
                    object.programFlashAvailable = message.programFlashAvailable;
                if (message.extendedMemoryAvailable != null && message.hasOwnProperty("extendedMemoryAvailable"))
                    object.extendedMemoryAvailable = message.extendedMemoryAvailable;
                if (message.dataMemoryInstalled != null && message.hasOwnProperty("dataMemoryInstalled"))
                    object.dataMemoryInstalled = message.dataMemoryInstalled;
                if (message.dataMemoryUsed != null && message.hasOwnProperty("dataMemoryUsed"))
                    object.dataMemoryUsed = message.dataMemoryUsed;
                if (message.dataMemoryConsumption != null && message.hasOwnProperty("dataMemoryConsumption"))
                    object.dataMemoryConsumption = options.json && !isFinite(message.dataMemoryConsumption) ? String(message.dataMemoryConsumption) : message.dataMemoryConsumption;
                if (message.firmware && message.firmware.length) {
                    object.firmware = [];
                    for (var j = 0; j < message.firmware.length; ++j)
                        object.firmware[j] = $root.fk_app.Firmware.toObject(message.firmware[j], options);
                }
                return object;
            };
    
            /**
             * Converts this MemoryStatus to JSON.
             * @function toJSON
             * @memberof fk_app.MemoryStatus
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            MemoryStatus.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return MemoryStatus;
        })();
    
        fk_app.BatteryStatus = (function() {
    
            /**
             * Properties of a BatteryStatus.
             * @memberof fk_app
             * @interface IBatteryStatus
             * @property {number} [voltage] BatteryStatus voltage
             * @property {number} [percentage] BatteryStatus percentage
             */
    
            /**
             * Constructs a new BatteryStatus.
             * @memberof fk_app
             * @classdesc Represents a BatteryStatus.
             * @constructor
             * @param {fk_app.IBatteryStatus=} [properties] Properties to set
             */
            function BatteryStatus(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * BatteryStatus voltage.
             * @member {number}voltage
             * @memberof fk_app.BatteryStatus
             * @instance
             */
            BatteryStatus.prototype.voltage = 0;
    
            /**
             * BatteryStatus percentage.
             * @member {number}percentage
             * @memberof fk_app.BatteryStatus
             * @instance
             */
            BatteryStatus.prototype.percentage = 0;
    
            /**
             * Creates a new BatteryStatus instance using the specified properties.
             * @function create
             * @memberof fk_app.BatteryStatus
             * @static
             * @param {fk_app.IBatteryStatus=} [properties] Properties to set
             * @returns {fk_app.BatteryStatus} BatteryStatus instance
             */
            BatteryStatus.create = function create(properties) {
                return new BatteryStatus(properties);
            };
    
            /**
             * Encodes the specified BatteryStatus message. Does not implicitly {@link fk_app.BatteryStatus.verify|verify} messages.
             * @function encode
             * @memberof fk_app.BatteryStatus
             * @static
             * @param {fk_app.IBatteryStatus} message BatteryStatus message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            BatteryStatus.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.voltage != null && message.hasOwnProperty("voltage"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.voltage);
                if (message.percentage != null && message.hasOwnProperty("percentage"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint32(message.percentage);
                return writer;
            };
    
            /**
             * Encodes the specified BatteryStatus message, length delimited. Does not implicitly {@link fk_app.BatteryStatus.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.BatteryStatus
             * @static
             * @param {fk_app.IBatteryStatus} message BatteryStatus message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            BatteryStatus.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a BatteryStatus message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.BatteryStatus
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.BatteryStatus} BatteryStatus
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            BatteryStatus.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.BatteryStatus();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.voltage = reader.uint32();
                        break;
                    case 2:
                        message.percentage = reader.uint32();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a BatteryStatus message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.BatteryStatus
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.BatteryStatus} BatteryStatus
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            BatteryStatus.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a BatteryStatus message.
             * @function verify
             * @memberof fk_app.BatteryStatus
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            BatteryStatus.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.voltage != null && message.hasOwnProperty("voltage"))
                    if (!$util.isInteger(message.voltage))
                        return "voltage: integer expected";
                if (message.percentage != null && message.hasOwnProperty("percentage"))
                    if (!$util.isInteger(message.percentage))
                        return "percentage: integer expected";
                return null;
            };
    
            /**
             * Creates a BatteryStatus message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.BatteryStatus
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.BatteryStatus} BatteryStatus
             */
            BatteryStatus.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.BatteryStatus)
                    return object;
                var message = new $root.fk_app.BatteryStatus();
                if (object.voltage != null)
                    message.voltage = object.voltage >>> 0;
                if (object.percentage != null)
                    message.percentage = object.percentage >>> 0;
                return message;
            };
    
            /**
             * Creates a plain object from a BatteryStatus message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.BatteryStatus
             * @static
             * @param {fk_app.BatteryStatus} message BatteryStatus
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            BatteryStatus.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.voltage = 0;
                    object.percentage = 0;
                }
                if (message.voltage != null && message.hasOwnProperty("voltage"))
                    object.voltage = message.voltage;
                if (message.percentage != null && message.hasOwnProperty("percentage"))
                    object.percentage = message.percentage;
                return object;
            };
    
            /**
             * Converts this BatteryStatus to JSON.
             * @function toJSON
             * @memberof fk_app.BatteryStatus
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            BatteryStatus.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return BatteryStatus;
        })();
    
        fk_app.SolarStatus = (function() {
    
            /**
             * Properties of a SolarStatus.
             * @memberof fk_app
             * @interface ISolarStatus
             * @property {number} [voltage] SolarStatus voltage
             */
    
            /**
             * Constructs a new SolarStatus.
             * @memberof fk_app
             * @classdesc Represents a SolarStatus.
             * @constructor
             * @param {fk_app.ISolarStatus=} [properties] Properties to set
             */
            function SolarStatus(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * SolarStatus voltage.
             * @member {number}voltage
             * @memberof fk_app.SolarStatus
             * @instance
             */
            SolarStatus.prototype.voltage = 0;
    
            /**
             * Creates a new SolarStatus instance using the specified properties.
             * @function create
             * @memberof fk_app.SolarStatus
             * @static
             * @param {fk_app.ISolarStatus=} [properties] Properties to set
             * @returns {fk_app.SolarStatus} SolarStatus instance
             */
            SolarStatus.create = function create(properties) {
                return new SolarStatus(properties);
            };
    
            /**
             * Encodes the specified SolarStatus message. Does not implicitly {@link fk_app.SolarStatus.verify|verify} messages.
             * @function encode
             * @memberof fk_app.SolarStatus
             * @static
             * @param {fk_app.ISolarStatus} message SolarStatus message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            SolarStatus.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.voltage != null && message.hasOwnProperty("voltage"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.voltage);
                return writer;
            };
    
            /**
             * Encodes the specified SolarStatus message, length delimited. Does not implicitly {@link fk_app.SolarStatus.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.SolarStatus
             * @static
             * @param {fk_app.ISolarStatus} message SolarStatus message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            SolarStatus.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a SolarStatus message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.SolarStatus
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.SolarStatus} SolarStatus
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            SolarStatus.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.SolarStatus();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.voltage = reader.uint32();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a SolarStatus message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.SolarStatus
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.SolarStatus} SolarStatus
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            SolarStatus.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a SolarStatus message.
             * @function verify
             * @memberof fk_app.SolarStatus
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            SolarStatus.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.voltage != null && message.hasOwnProperty("voltage"))
                    if (!$util.isInteger(message.voltage))
                        return "voltage: integer expected";
                return null;
            };
    
            /**
             * Creates a SolarStatus message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.SolarStatus
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.SolarStatus} SolarStatus
             */
            SolarStatus.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.SolarStatus)
                    return object;
                var message = new $root.fk_app.SolarStatus();
                if (object.voltage != null)
                    message.voltage = object.voltage >>> 0;
                return message;
            };
    
            /**
             * Creates a plain object from a SolarStatus message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.SolarStatus
             * @static
             * @param {fk_app.SolarStatus} message SolarStatus
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            SolarStatus.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults)
                    object.voltage = 0;
                if (message.voltage != null && message.hasOwnProperty("voltage"))
                    object.voltage = message.voltage;
                return object;
            };
    
            /**
             * Converts this SolarStatus to JSON.
             * @function toJSON
             * @memberof fk_app.SolarStatus
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            SolarStatus.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return SolarStatus;
        })();
    
        fk_app.PowerStatus = (function() {
    
            /**
             * Properties of a PowerStatus.
             * @memberof fk_app
             * @interface IPowerStatus
             * @property {fk_app.IBatteryStatus} [battery] PowerStatus battery
             * @property {fk_app.ISolarStatus} [solar] PowerStatus solar
             */
    
            /**
             * Constructs a new PowerStatus.
             * @memberof fk_app
             * @classdesc Represents a PowerStatus.
             * @constructor
             * @param {fk_app.IPowerStatus=} [properties] Properties to set
             */
            function PowerStatus(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * PowerStatus battery.
             * @member {(fk_app.IBatteryStatus|null|undefined)}battery
             * @memberof fk_app.PowerStatus
             * @instance
             */
            PowerStatus.prototype.battery = null;
    
            /**
             * PowerStatus solar.
             * @member {(fk_app.ISolarStatus|null|undefined)}solar
             * @memberof fk_app.PowerStatus
             * @instance
             */
            PowerStatus.prototype.solar = null;
    
            /**
             * Creates a new PowerStatus instance using the specified properties.
             * @function create
             * @memberof fk_app.PowerStatus
             * @static
             * @param {fk_app.IPowerStatus=} [properties] Properties to set
             * @returns {fk_app.PowerStatus} PowerStatus instance
             */
            PowerStatus.create = function create(properties) {
                return new PowerStatus(properties);
            };
    
            /**
             * Encodes the specified PowerStatus message. Does not implicitly {@link fk_app.PowerStatus.verify|verify} messages.
             * @function encode
             * @memberof fk_app.PowerStatus
             * @static
             * @param {fk_app.IPowerStatus} message PowerStatus message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            PowerStatus.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.battery != null && message.hasOwnProperty("battery"))
                    $root.fk_app.BatteryStatus.encode(message.battery, writer.uint32(/* id 1, wireType 2 =*/10).fork()).ldelim();
                if (message.solar != null && message.hasOwnProperty("solar"))
                    $root.fk_app.SolarStatus.encode(message.solar, writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
                return writer;
            };
    
            /**
             * Encodes the specified PowerStatus message, length delimited. Does not implicitly {@link fk_app.PowerStatus.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.PowerStatus
             * @static
             * @param {fk_app.IPowerStatus} message PowerStatus message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            PowerStatus.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a PowerStatus message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.PowerStatus
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.PowerStatus} PowerStatus
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            PowerStatus.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.PowerStatus();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.battery = $root.fk_app.BatteryStatus.decode(reader, reader.uint32());
                        break;
                    case 2:
                        message.solar = $root.fk_app.SolarStatus.decode(reader, reader.uint32());
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a PowerStatus message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.PowerStatus
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.PowerStatus} PowerStatus
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            PowerStatus.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a PowerStatus message.
             * @function verify
             * @memberof fk_app.PowerStatus
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            PowerStatus.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.battery != null && message.hasOwnProperty("battery")) {
                    var error = $root.fk_app.BatteryStatus.verify(message.battery);
                    if (error)
                        return "battery." + error;
                }
                if (message.solar != null && message.hasOwnProperty("solar")) {
                    error = $root.fk_app.SolarStatus.verify(message.solar);
                    if (error)
                        return "solar." + error;
                }
                return null;
            };
    
            /**
             * Creates a PowerStatus message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.PowerStatus
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.PowerStatus} PowerStatus
             */
            PowerStatus.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.PowerStatus)
                    return object;
                var message = new $root.fk_app.PowerStatus();
                if (object.battery != null) {
                    if (typeof object.battery !== "object")
                        throw TypeError(".fk_app.PowerStatus.battery: object expected");
                    message.battery = $root.fk_app.BatteryStatus.fromObject(object.battery);
                }
                if (object.solar != null) {
                    if (typeof object.solar !== "object")
                        throw TypeError(".fk_app.PowerStatus.solar: object expected");
                    message.solar = $root.fk_app.SolarStatus.fromObject(object.solar);
                }
                return message;
            };
    
            /**
             * Creates a plain object from a PowerStatus message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.PowerStatus
             * @static
             * @param {fk_app.PowerStatus} message PowerStatus
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            PowerStatus.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.battery = null;
                    object.solar = null;
                }
                if (message.battery != null && message.hasOwnProperty("battery"))
                    object.battery = $root.fk_app.BatteryStatus.toObject(message.battery, options);
                if (message.solar != null && message.hasOwnProperty("solar"))
                    object.solar = $root.fk_app.SolarStatus.toObject(message.solar, options);
                return object;
            };
    
            /**
             * Converts this PowerStatus to JSON.
             * @function toJSON
             * @memberof fk_app.PowerStatus
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            PowerStatus.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return PowerStatus;
        })();
    
        fk_app.Status = (function() {
    
            /**
             * Properties of a Status.
             * @memberof fk_app
             * @interface IStatus
             * @property {number} [version] Status version
             * @property {number} [uptime] Status uptime
             * @property {fk_app.IIdentity} [identity] Status identity
             * @property {fk_app.IHardwareStatus} [hardware] Status hardware
             * @property {fk_app.IPowerStatus} [power] Status power
             * @property {fk_app.IMemoryStatus} [memory] Status memory
             * @property {fk_app.IGpsStatus} [gps] Status gps
             * @property {fk_app.ISchedules} [schedules] Status schedules
             * @property {fk_app.IRecording} [recording] Status recording
             * @property {fk_app.INetworkSettings} [network] Status network
             * @property {number|Long} [time] Status time
             * @property {fk_app.IFirmware} [firmware] Status firmware
             * @property {string} [logs] Status logs
             */
    
            /**
             * Constructs a new Status.
             * @memberof fk_app
             * @classdesc Represents a Status.
             * @constructor
             * @param {fk_app.IStatus=} [properties] Properties to set
             */
            function Status(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Status version.
             * @member {number}version
             * @memberof fk_app.Status
             * @instance
             */
            Status.prototype.version = 0;
    
            /**
             * Status uptime.
             * @member {number}uptime
             * @memberof fk_app.Status
             * @instance
             */
            Status.prototype.uptime = 0;
    
            /**
             * Status identity.
             * @member {(fk_app.IIdentity|null|undefined)}identity
             * @memberof fk_app.Status
             * @instance
             */
            Status.prototype.identity = null;
    
            /**
             * Status hardware.
             * @member {(fk_app.IHardwareStatus|null|undefined)}hardware
             * @memberof fk_app.Status
             * @instance
             */
            Status.prototype.hardware = null;
    
            /**
             * Status power.
             * @member {(fk_app.IPowerStatus|null|undefined)}power
             * @memberof fk_app.Status
             * @instance
             */
            Status.prototype.power = null;
    
            /**
             * Status memory.
             * @member {(fk_app.IMemoryStatus|null|undefined)}memory
             * @memberof fk_app.Status
             * @instance
             */
            Status.prototype.memory = null;
    
            /**
             * Status gps.
             * @member {(fk_app.IGpsStatus|null|undefined)}gps
             * @memberof fk_app.Status
             * @instance
             */
            Status.prototype.gps = null;
    
            /**
             * Status schedules.
             * @member {(fk_app.ISchedules|null|undefined)}schedules
             * @memberof fk_app.Status
             * @instance
             */
            Status.prototype.schedules = null;
    
            /**
             * Status recording.
             * @member {(fk_app.IRecording|null|undefined)}recording
             * @memberof fk_app.Status
             * @instance
             */
            Status.prototype.recording = null;
    
            /**
             * Status network.
             * @member {(fk_app.INetworkSettings|null|undefined)}network
             * @memberof fk_app.Status
             * @instance
             */
            Status.prototype.network = null;
    
            /**
             * Status time.
             * @member {number|Long}time
             * @memberof fk_app.Status
             * @instance
             */
            Status.prototype.time = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * Status firmware.
             * @member {(fk_app.IFirmware|null|undefined)}firmware
             * @memberof fk_app.Status
             * @instance
             */
            Status.prototype.firmware = null;
    
            /**
             * Status logs.
             * @member {string}logs
             * @memberof fk_app.Status
             * @instance
             */
            Status.prototype.logs = "";
    
            /**
             * Creates a new Status instance using the specified properties.
             * @function create
             * @memberof fk_app.Status
             * @static
             * @param {fk_app.IStatus=} [properties] Properties to set
             * @returns {fk_app.Status} Status instance
             */
            Status.create = function create(properties) {
                return new Status(properties);
            };
    
            /**
             * Encodes the specified Status message. Does not implicitly {@link fk_app.Status.verify|verify} messages.
             * @function encode
             * @memberof fk_app.Status
             * @static
             * @param {fk_app.IStatus} message Status message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Status.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.version != null && message.hasOwnProperty("version"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.version);
                if (message.uptime != null && message.hasOwnProperty("uptime"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint32(message.uptime);
                if (message.identity != null && message.hasOwnProperty("identity"))
                    $root.fk_app.Identity.encode(message.identity, writer.uint32(/* id 3, wireType 2 =*/26).fork()).ldelim();
                if (message.hardware != null && message.hasOwnProperty("hardware"))
                    $root.fk_app.HardwareStatus.encode(message.hardware, writer.uint32(/* id 4, wireType 2 =*/34).fork()).ldelim();
                if (message.power != null && message.hasOwnProperty("power"))
                    $root.fk_app.PowerStatus.encode(message.power, writer.uint32(/* id 5, wireType 2 =*/42).fork()).ldelim();
                if (message.memory != null && message.hasOwnProperty("memory"))
                    $root.fk_app.MemoryStatus.encode(message.memory, writer.uint32(/* id 6, wireType 2 =*/50).fork()).ldelim();
                if (message.gps != null && message.hasOwnProperty("gps"))
                    $root.fk_app.GpsStatus.encode(message.gps, writer.uint32(/* id 7, wireType 2 =*/58).fork()).ldelim();
                if (message.schedules != null && message.hasOwnProperty("schedules"))
                    $root.fk_app.Schedules.encode(message.schedules, writer.uint32(/* id 8, wireType 2 =*/66).fork()).ldelim();
                if (message.recording != null && message.hasOwnProperty("recording"))
                    $root.fk_app.Recording.encode(message.recording, writer.uint32(/* id 9, wireType 2 =*/74).fork()).ldelim();
                if (message.network != null && message.hasOwnProperty("network"))
                    $root.fk_app.NetworkSettings.encode(message.network, writer.uint32(/* id 10, wireType 2 =*/82).fork()).ldelim();
                if (message.time != null && message.hasOwnProperty("time"))
                    writer.uint32(/* id 11, wireType 0 =*/88).uint64(message.time);
                if (message.firmware != null && message.hasOwnProperty("firmware"))
                    $root.fk_app.Firmware.encode(message.firmware, writer.uint32(/* id 12, wireType 2 =*/98).fork()).ldelim();
                if (message.logs != null && message.hasOwnProperty("logs"))
                    writer.uint32(/* id 13, wireType 2 =*/106).string(message.logs);
                return writer;
            };
    
            /**
             * Encodes the specified Status message, length delimited. Does not implicitly {@link fk_app.Status.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.Status
             * @static
             * @param {fk_app.IStatus} message Status message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Status.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a Status message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.Status
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.Status} Status
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Status.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.Status();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.version = reader.uint32();
                        break;
                    case 2:
                        message.uptime = reader.uint32();
                        break;
                    case 3:
                        message.identity = $root.fk_app.Identity.decode(reader, reader.uint32());
                        break;
                    case 4:
                        message.hardware = $root.fk_app.HardwareStatus.decode(reader, reader.uint32());
                        break;
                    case 5:
                        message.power = $root.fk_app.PowerStatus.decode(reader, reader.uint32());
                        break;
                    case 6:
                        message.memory = $root.fk_app.MemoryStatus.decode(reader, reader.uint32());
                        break;
                    case 7:
                        message.gps = $root.fk_app.GpsStatus.decode(reader, reader.uint32());
                        break;
                    case 8:
                        message.schedules = $root.fk_app.Schedules.decode(reader, reader.uint32());
                        break;
                    case 9:
                        message.recording = $root.fk_app.Recording.decode(reader, reader.uint32());
                        break;
                    case 10:
                        message.network = $root.fk_app.NetworkSettings.decode(reader, reader.uint32());
                        break;
                    case 11:
                        message.time = reader.uint64();
                        break;
                    case 12:
                        message.firmware = $root.fk_app.Firmware.decode(reader, reader.uint32());
                        break;
                    case 13:
                        message.logs = reader.string();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a Status message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.Status
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.Status} Status
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Status.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a Status message.
             * @function verify
             * @memberof fk_app.Status
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            Status.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.version != null && message.hasOwnProperty("version"))
                    if (!$util.isInteger(message.version))
                        return "version: integer expected";
                if (message.uptime != null && message.hasOwnProperty("uptime"))
                    if (!$util.isInteger(message.uptime))
                        return "uptime: integer expected";
                if (message.identity != null && message.hasOwnProperty("identity")) {
                    var error = $root.fk_app.Identity.verify(message.identity);
                    if (error)
                        return "identity." + error;
                }
                if (message.hardware != null && message.hasOwnProperty("hardware")) {
                    error = $root.fk_app.HardwareStatus.verify(message.hardware);
                    if (error)
                        return "hardware." + error;
                }
                if (message.power != null && message.hasOwnProperty("power")) {
                    error = $root.fk_app.PowerStatus.verify(message.power);
                    if (error)
                        return "power." + error;
                }
                if (message.memory != null && message.hasOwnProperty("memory")) {
                    error = $root.fk_app.MemoryStatus.verify(message.memory);
                    if (error)
                        return "memory." + error;
                }
                if (message.gps != null && message.hasOwnProperty("gps")) {
                    error = $root.fk_app.GpsStatus.verify(message.gps);
                    if (error)
                        return "gps." + error;
                }
                if (message.schedules != null && message.hasOwnProperty("schedules")) {
                    error = $root.fk_app.Schedules.verify(message.schedules);
                    if (error)
                        return "schedules." + error;
                }
                if (message.recording != null && message.hasOwnProperty("recording")) {
                    error = $root.fk_app.Recording.verify(message.recording);
                    if (error)
                        return "recording." + error;
                }
                if (message.network != null && message.hasOwnProperty("network")) {
                    error = $root.fk_app.NetworkSettings.verify(message.network);
                    if (error)
                        return "network." + error;
                }
                if (message.time != null && message.hasOwnProperty("time"))
                    if (!$util.isInteger(message.time) && !(message.time && $util.isInteger(message.time.low) && $util.isInteger(message.time.high)))
                        return "time: integer|Long expected";
                if (message.firmware != null && message.hasOwnProperty("firmware")) {
                    error = $root.fk_app.Firmware.verify(message.firmware);
                    if (error)
                        return "firmware." + error;
                }
                if (message.logs != null && message.hasOwnProperty("logs"))
                    if (!$util.isString(message.logs))
                        return "logs: string expected";
                return null;
            };
    
            /**
             * Creates a Status message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.Status
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.Status} Status
             */
            Status.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.Status)
                    return object;
                var message = new $root.fk_app.Status();
                if (object.version != null)
                    message.version = object.version >>> 0;
                if (object.uptime != null)
                    message.uptime = object.uptime >>> 0;
                if (object.identity != null) {
                    if (typeof object.identity !== "object")
                        throw TypeError(".fk_app.Status.identity: object expected");
                    message.identity = $root.fk_app.Identity.fromObject(object.identity);
                }
                if (object.hardware != null) {
                    if (typeof object.hardware !== "object")
                        throw TypeError(".fk_app.Status.hardware: object expected");
                    message.hardware = $root.fk_app.HardwareStatus.fromObject(object.hardware);
                }
                if (object.power != null) {
                    if (typeof object.power !== "object")
                        throw TypeError(".fk_app.Status.power: object expected");
                    message.power = $root.fk_app.PowerStatus.fromObject(object.power);
                }
                if (object.memory != null) {
                    if (typeof object.memory !== "object")
                        throw TypeError(".fk_app.Status.memory: object expected");
                    message.memory = $root.fk_app.MemoryStatus.fromObject(object.memory);
                }
                if (object.gps != null) {
                    if (typeof object.gps !== "object")
                        throw TypeError(".fk_app.Status.gps: object expected");
                    message.gps = $root.fk_app.GpsStatus.fromObject(object.gps);
                }
                if (object.schedules != null) {
                    if (typeof object.schedules !== "object")
                        throw TypeError(".fk_app.Status.schedules: object expected");
                    message.schedules = $root.fk_app.Schedules.fromObject(object.schedules);
                }
                if (object.recording != null) {
                    if (typeof object.recording !== "object")
                        throw TypeError(".fk_app.Status.recording: object expected");
                    message.recording = $root.fk_app.Recording.fromObject(object.recording);
                }
                if (object.network != null) {
                    if (typeof object.network !== "object")
                        throw TypeError(".fk_app.Status.network: object expected");
                    message.network = $root.fk_app.NetworkSettings.fromObject(object.network);
                }
                if (object.time != null)
                    if ($util.Long)
                        (message.time = $util.Long.fromValue(object.time)).unsigned = true;
                    else if (typeof object.time === "string")
                        message.time = parseInt(object.time, 10);
                    else if (typeof object.time === "number")
                        message.time = object.time;
                    else if (typeof object.time === "object")
                        message.time = new $util.LongBits(object.time.low >>> 0, object.time.high >>> 0).toNumber(true);
                if (object.firmware != null) {
                    if (typeof object.firmware !== "object")
                        throw TypeError(".fk_app.Status.firmware: object expected");
                    message.firmware = $root.fk_app.Firmware.fromObject(object.firmware);
                }
                if (object.logs != null)
                    message.logs = String(object.logs);
                return message;
            };
    
            /**
             * Creates a plain object from a Status message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.Status
             * @static
             * @param {fk_app.Status} message Status
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            Status.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.version = 0;
                    object.uptime = 0;
                    object.identity = null;
                    object.hardware = null;
                    object.power = null;
                    object.memory = null;
                    object.gps = null;
                    object.schedules = null;
                    object.recording = null;
                    object.network = null;
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.time = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.time = options.longs === String ? "0" : 0;
                    object.firmware = null;
                    object.logs = "";
                }
                if (message.version != null && message.hasOwnProperty("version"))
                    object.version = message.version;
                if (message.uptime != null && message.hasOwnProperty("uptime"))
                    object.uptime = message.uptime;
                if (message.identity != null && message.hasOwnProperty("identity"))
                    object.identity = $root.fk_app.Identity.toObject(message.identity, options);
                if (message.hardware != null && message.hasOwnProperty("hardware"))
                    object.hardware = $root.fk_app.HardwareStatus.toObject(message.hardware, options);
                if (message.power != null && message.hasOwnProperty("power"))
                    object.power = $root.fk_app.PowerStatus.toObject(message.power, options);
                if (message.memory != null && message.hasOwnProperty("memory"))
                    object.memory = $root.fk_app.MemoryStatus.toObject(message.memory, options);
                if (message.gps != null && message.hasOwnProperty("gps"))
                    object.gps = $root.fk_app.GpsStatus.toObject(message.gps, options);
                if (message.schedules != null && message.hasOwnProperty("schedules"))
                    object.schedules = $root.fk_app.Schedules.toObject(message.schedules, options);
                if (message.recording != null && message.hasOwnProperty("recording"))
                    object.recording = $root.fk_app.Recording.toObject(message.recording, options);
                if (message.network != null && message.hasOwnProperty("network"))
                    object.network = $root.fk_app.NetworkSettings.toObject(message.network, options);
                if (message.time != null && message.hasOwnProperty("time"))
                    if (typeof message.time === "number")
                        object.time = options.longs === String ? String(message.time) : message.time;
                    else
                        object.time = options.longs === String ? $util.Long.prototype.toString.call(message.time) : options.longs === Number ? new $util.LongBits(message.time.low >>> 0, message.time.high >>> 0).toNumber(true) : message.time;
                if (message.firmware != null && message.hasOwnProperty("firmware"))
                    object.firmware = $root.fk_app.Firmware.toObject(message.firmware, options);
                if (message.logs != null && message.hasOwnProperty("logs"))
                    object.logs = message.logs;
                return object;
            };
    
            /**
             * Converts this Status to JSON.
             * @function toJSON
             * @memberof fk_app.Status
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Status.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Status;
        })();
    
        fk_app.Range = (function() {
    
            /**
             * Properties of a Range.
             * @memberof fk_app
             * @interface IRange
             * @property {number} [start] Range start
             * @property {number} [end] Range end
             */
    
            /**
             * Constructs a new Range.
             * @memberof fk_app
             * @classdesc Represents a Range.
             * @constructor
             * @param {fk_app.IRange=} [properties] Properties to set
             */
            function Range(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Range start.
             * @member {number}start
             * @memberof fk_app.Range
             * @instance
             */
            Range.prototype.start = 0;
    
            /**
             * Range end.
             * @member {number}end
             * @memberof fk_app.Range
             * @instance
             */
            Range.prototype.end = 0;
    
            /**
             * Creates a new Range instance using the specified properties.
             * @function create
             * @memberof fk_app.Range
             * @static
             * @param {fk_app.IRange=} [properties] Properties to set
             * @returns {fk_app.Range} Range instance
             */
            Range.create = function create(properties) {
                return new Range(properties);
            };
    
            /**
             * Encodes the specified Range message. Does not implicitly {@link fk_app.Range.verify|verify} messages.
             * @function encode
             * @memberof fk_app.Range
             * @static
             * @param {fk_app.IRange} message Range message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Range.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.start != null && message.hasOwnProperty("start"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.start);
                if (message.end != null && message.hasOwnProperty("end"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint32(message.end);
                return writer;
            };
    
            /**
             * Encodes the specified Range message, length delimited. Does not implicitly {@link fk_app.Range.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.Range
             * @static
             * @param {fk_app.IRange} message Range message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Range.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a Range message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.Range
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.Range} Range
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Range.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.Range();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.start = reader.uint32();
                        break;
                    case 2:
                        message.end = reader.uint32();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a Range message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.Range
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.Range} Range
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Range.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a Range message.
             * @function verify
             * @memberof fk_app.Range
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            Range.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.start != null && message.hasOwnProperty("start"))
                    if (!$util.isInteger(message.start))
                        return "start: integer expected";
                if (message.end != null && message.hasOwnProperty("end"))
                    if (!$util.isInteger(message.end))
                        return "end: integer expected";
                return null;
            };
    
            /**
             * Creates a Range message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.Range
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.Range} Range
             */
            Range.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.Range)
                    return object;
                var message = new $root.fk_app.Range();
                if (object.start != null)
                    message.start = object.start >>> 0;
                if (object.end != null)
                    message.end = object.end >>> 0;
                return message;
            };
    
            /**
             * Creates a plain object from a Range message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.Range
             * @static
             * @param {fk_app.Range} message Range
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            Range.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.start = 0;
                    object.end = 0;
                }
                if (message.start != null && message.hasOwnProperty("start"))
                    object.start = message.start;
                if (message.end != null && message.hasOwnProperty("end"))
                    object.end = message.end;
                return object;
            };
    
            /**
             * Converts this Range to JSON.
             * @function toJSON
             * @memberof fk_app.Range
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Range.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Range;
        })();
    
        fk_app.DownloadQuery = (function() {
    
            /**
             * Properties of a DownloadQuery.
             * @memberof fk_app
             * @interface IDownloadQuery
             * @property {number} [stream] DownloadQuery stream
             * @property {Array.<fk_app.IRange>} [ranges] DownloadQuery ranges
             * @property {Array.<number>} [blocks] DownloadQuery blocks
             */
    
            /**
             * Constructs a new DownloadQuery.
             * @memberof fk_app
             * @classdesc Represents a DownloadQuery.
             * @constructor
             * @param {fk_app.IDownloadQuery=} [properties] Properties to set
             */
            function DownloadQuery(properties) {
                this.ranges = [];
                this.blocks = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * DownloadQuery stream.
             * @member {number}stream
             * @memberof fk_app.DownloadQuery
             * @instance
             */
            DownloadQuery.prototype.stream = 0;
    
            /**
             * DownloadQuery ranges.
             * @member {Array.<fk_app.IRange>}ranges
             * @memberof fk_app.DownloadQuery
             * @instance
             */
            DownloadQuery.prototype.ranges = $util.emptyArray;
    
            /**
             * DownloadQuery blocks.
             * @member {Array.<number>}blocks
             * @memberof fk_app.DownloadQuery
             * @instance
             */
            DownloadQuery.prototype.blocks = $util.emptyArray;
    
            /**
             * Creates a new DownloadQuery instance using the specified properties.
             * @function create
             * @memberof fk_app.DownloadQuery
             * @static
             * @param {fk_app.IDownloadQuery=} [properties] Properties to set
             * @returns {fk_app.DownloadQuery} DownloadQuery instance
             */
            DownloadQuery.create = function create(properties) {
                return new DownloadQuery(properties);
            };
    
            /**
             * Encodes the specified DownloadQuery message. Does not implicitly {@link fk_app.DownloadQuery.verify|verify} messages.
             * @function encode
             * @memberof fk_app.DownloadQuery
             * @static
             * @param {fk_app.IDownloadQuery} message DownloadQuery message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            DownloadQuery.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.stream != null && message.hasOwnProperty("stream"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.stream);
                if (message.ranges != null && message.ranges.length)
                    for (var i = 0; i < message.ranges.length; ++i)
                        $root.fk_app.Range.encode(message.ranges[i], writer.uint32(/* id 3, wireType 2 =*/26).fork()).ldelim();
                if (message.blocks != null && message.blocks.length) {
                    writer.uint32(/* id 4, wireType 2 =*/34).fork();
                    for (var i = 0; i < message.blocks.length; ++i)
                        writer.uint32(message.blocks[i]);
                    writer.ldelim();
                }
                return writer;
            };
    
            /**
             * Encodes the specified DownloadQuery message, length delimited. Does not implicitly {@link fk_app.DownloadQuery.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.DownloadQuery
             * @static
             * @param {fk_app.IDownloadQuery} message DownloadQuery message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            DownloadQuery.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a DownloadQuery message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.DownloadQuery
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.DownloadQuery} DownloadQuery
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            DownloadQuery.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.DownloadQuery();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.stream = reader.uint32();
                        break;
                    case 3:
                        if (!(message.ranges && message.ranges.length))
                            message.ranges = [];
                        message.ranges.push($root.fk_app.Range.decode(reader, reader.uint32()));
                        break;
                    case 4:
                        if (!(message.blocks && message.blocks.length))
                            message.blocks = [];
                        if ((tag & 7) === 2) {
                            var end2 = reader.uint32() + reader.pos;
                            while (reader.pos < end2)
                                message.blocks.push(reader.uint32());
                        } else
                            message.blocks.push(reader.uint32());
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a DownloadQuery message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.DownloadQuery
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.DownloadQuery} DownloadQuery
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            DownloadQuery.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a DownloadQuery message.
             * @function verify
             * @memberof fk_app.DownloadQuery
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            DownloadQuery.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.stream != null && message.hasOwnProperty("stream"))
                    if (!$util.isInteger(message.stream))
                        return "stream: integer expected";
                if (message.ranges != null && message.hasOwnProperty("ranges")) {
                    if (!Array.isArray(message.ranges))
                        return "ranges: array expected";
                    for (var i = 0; i < message.ranges.length; ++i) {
                        var error = $root.fk_app.Range.verify(message.ranges[i]);
                        if (error)
                            return "ranges." + error;
                    }
                }
                if (message.blocks != null && message.hasOwnProperty("blocks")) {
                    if (!Array.isArray(message.blocks))
                        return "blocks: array expected";
                    for (var i = 0; i < message.blocks.length; ++i)
                        if (!$util.isInteger(message.blocks[i]))
                            return "blocks: integer[] expected";
                }
                return null;
            };
    
            /**
             * Creates a DownloadQuery message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.DownloadQuery
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.DownloadQuery} DownloadQuery
             */
            DownloadQuery.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.DownloadQuery)
                    return object;
                var message = new $root.fk_app.DownloadQuery();
                if (object.stream != null)
                    message.stream = object.stream >>> 0;
                if (object.ranges) {
                    if (!Array.isArray(object.ranges))
                        throw TypeError(".fk_app.DownloadQuery.ranges: array expected");
                    message.ranges = [];
                    for (var i = 0; i < object.ranges.length; ++i) {
                        if (typeof object.ranges[i] !== "object")
                            throw TypeError(".fk_app.DownloadQuery.ranges: object expected");
                        message.ranges[i] = $root.fk_app.Range.fromObject(object.ranges[i]);
                    }
                }
                if (object.blocks) {
                    if (!Array.isArray(object.blocks))
                        throw TypeError(".fk_app.DownloadQuery.blocks: array expected");
                    message.blocks = [];
                    for (var i = 0; i < object.blocks.length; ++i)
                        message.blocks[i] = object.blocks[i] >>> 0;
                }
                return message;
            };
    
            /**
             * Creates a plain object from a DownloadQuery message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.DownloadQuery
             * @static
             * @param {fk_app.DownloadQuery} message DownloadQuery
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            DownloadQuery.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults) {
                    object.ranges = [];
                    object.blocks = [];
                }
                if (options.defaults)
                    object.stream = 0;
                if (message.stream != null && message.hasOwnProperty("stream"))
                    object.stream = message.stream;
                if (message.ranges && message.ranges.length) {
                    object.ranges = [];
                    for (var j = 0; j < message.ranges.length; ++j)
                        object.ranges[j] = $root.fk_app.Range.toObject(message.ranges[j], options);
                }
                if (message.blocks && message.blocks.length) {
                    object.blocks = [];
                    for (var j = 0; j < message.blocks.length; ++j)
                        object.blocks[j] = message.blocks[j];
                }
                return object;
            };
    
            /**
             * Converts this DownloadQuery to JSON.
             * @function toJSON
             * @memberof fk_app.DownloadQuery
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            DownloadQuery.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return DownloadQuery;
        })();
    
        fk_app.Recording = (function() {
    
            /**
             * Properties of a Recording.
             * @memberof fk_app
             * @interface IRecording
             * @property {boolean} [modifying] Recording modifying
             * @property {boolean} [enabled] Recording enabled
             * @property {number|Long} [startedTime] Recording startedTime
             * @property {fk_app.ILocation} [location] Recording location
             */
    
            /**
             * Constructs a new Recording.
             * @memberof fk_app
             * @classdesc Represents a Recording.
             * @constructor
             * @param {fk_app.IRecording=} [properties] Properties to set
             */
            function Recording(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Recording modifying.
             * @member {boolean}modifying
             * @memberof fk_app.Recording
             * @instance
             */
            Recording.prototype.modifying = false;
    
            /**
             * Recording enabled.
             * @member {boolean}enabled
             * @memberof fk_app.Recording
             * @instance
             */
            Recording.prototype.enabled = false;
    
            /**
             * Recording startedTime.
             * @member {number|Long}startedTime
             * @memberof fk_app.Recording
             * @instance
             */
            Recording.prototype.startedTime = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * Recording location.
             * @member {(fk_app.ILocation|null|undefined)}location
             * @memberof fk_app.Recording
             * @instance
             */
            Recording.prototype.location = null;
    
            /**
             * Creates a new Recording instance using the specified properties.
             * @function create
             * @memberof fk_app.Recording
             * @static
             * @param {fk_app.IRecording=} [properties] Properties to set
             * @returns {fk_app.Recording} Recording instance
             */
            Recording.create = function create(properties) {
                return new Recording(properties);
            };
    
            /**
             * Encodes the specified Recording message. Does not implicitly {@link fk_app.Recording.verify|verify} messages.
             * @function encode
             * @memberof fk_app.Recording
             * @static
             * @param {fk_app.IRecording} message Recording message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Recording.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.modifying != null && message.hasOwnProperty("modifying"))
                    writer.uint32(/* id 1, wireType 0 =*/8).bool(message.modifying);
                if (message.enabled != null && message.hasOwnProperty("enabled"))
                    writer.uint32(/* id 2, wireType 0 =*/16).bool(message.enabled);
                if (message.startedTime != null && message.hasOwnProperty("startedTime"))
                    writer.uint32(/* id 3, wireType 0 =*/24).uint64(message.startedTime);
                if (message.location != null && message.hasOwnProperty("location"))
                    $root.fk_app.Location.encode(message.location, writer.uint32(/* id 4, wireType 2 =*/34).fork()).ldelim();
                return writer;
            };
    
            /**
             * Encodes the specified Recording message, length delimited. Does not implicitly {@link fk_app.Recording.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.Recording
             * @static
             * @param {fk_app.IRecording} message Recording message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Recording.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a Recording message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.Recording
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.Recording} Recording
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Recording.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.Recording();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.modifying = reader.bool();
                        break;
                    case 2:
                        message.enabled = reader.bool();
                        break;
                    case 3:
                        message.startedTime = reader.uint64();
                        break;
                    case 4:
                        message.location = $root.fk_app.Location.decode(reader, reader.uint32());
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a Recording message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.Recording
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.Recording} Recording
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Recording.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a Recording message.
             * @function verify
             * @memberof fk_app.Recording
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            Recording.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.modifying != null && message.hasOwnProperty("modifying"))
                    if (typeof message.modifying !== "boolean")
                        return "modifying: boolean expected";
                if (message.enabled != null && message.hasOwnProperty("enabled"))
                    if (typeof message.enabled !== "boolean")
                        return "enabled: boolean expected";
                if (message.startedTime != null && message.hasOwnProperty("startedTime"))
                    if (!$util.isInteger(message.startedTime) && !(message.startedTime && $util.isInteger(message.startedTime.low) && $util.isInteger(message.startedTime.high)))
                        return "startedTime: integer|Long expected";
                if (message.location != null && message.hasOwnProperty("location")) {
                    var error = $root.fk_app.Location.verify(message.location);
                    if (error)
                        return "location." + error;
                }
                return null;
            };
    
            /**
             * Creates a Recording message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.Recording
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.Recording} Recording
             */
            Recording.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.Recording)
                    return object;
                var message = new $root.fk_app.Recording();
                if (object.modifying != null)
                    message.modifying = Boolean(object.modifying);
                if (object.enabled != null)
                    message.enabled = Boolean(object.enabled);
                if (object.startedTime != null)
                    if ($util.Long)
                        (message.startedTime = $util.Long.fromValue(object.startedTime)).unsigned = true;
                    else if (typeof object.startedTime === "string")
                        message.startedTime = parseInt(object.startedTime, 10);
                    else if (typeof object.startedTime === "number")
                        message.startedTime = object.startedTime;
                    else if (typeof object.startedTime === "object")
                        message.startedTime = new $util.LongBits(object.startedTime.low >>> 0, object.startedTime.high >>> 0).toNumber(true);
                if (object.location != null) {
                    if (typeof object.location !== "object")
                        throw TypeError(".fk_app.Recording.location: object expected");
                    message.location = $root.fk_app.Location.fromObject(object.location);
                }
                return message;
            };
    
            /**
             * Creates a plain object from a Recording message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.Recording
             * @static
             * @param {fk_app.Recording} message Recording
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            Recording.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.modifying = false;
                    object.enabled = false;
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.startedTime = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.startedTime = options.longs === String ? "0" : 0;
                    object.location = null;
                }
                if (message.modifying != null && message.hasOwnProperty("modifying"))
                    object.modifying = message.modifying;
                if (message.enabled != null && message.hasOwnProperty("enabled"))
                    object.enabled = message.enabled;
                if (message.startedTime != null && message.hasOwnProperty("startedTime"))
                    if (typeof message.startedTime === "number")
                        object.startedTime = options.longs === String ? String(message.startedTime) : message.startedTime;
                    else
                        object.startedTime = options.longs === String ? $util.Long.prototype.toString.call(message.startedTime) : options.longs === Number ? new $util.LongBits(message.startedTime.low >>> 0, message.startedTime.high >>> 0).toNumber(true) : message.startedTime;
                if (message.location != null && message.hasOwnProperty("location"))
                    object.location = $root.fk_app.Location.toObject(message.location, options);
                return object;
            };
    
            /**
             * Converts this Recording to JSON.
             * @function toJSON
             * @memberof fk_app.Recording
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Recording.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Recording;
        })();
    
        fk_app.LoraSettings = (function() {
    
            /**
             * Properties of a LoraSettings.
             * @memberof fk_app
             * @interface ILoraSettings
             * @property {boolean} [available] LoraSettings available
             * @property {boolean} [modifying] LoraSettings modifying
             * @property {boolean} [clearing] LoraSettings clearing
             * @property {number} [frequencyBand] LoraSettings frequencyBand
             * @property {Uint8Array} [deviceEui] LoraSettings deviceEui
             * @property {Uint8Array} [appKey] LoraSettings appKey
             * @property {Uint8Array} [joinEui] LoraSettings joinEui
             * @property {Uint8Array} [deviceAddress] LoraSettings deviceAddress
             * @property {Uint8Array} [networkSessionKey] LoraSettings networkSessionKey
             * @property {Uint8Array} [appSessionKey] LoraSettings appSessionKey
             */
    
            /**
             * Constructs a new LoraSettings.
             * @memberof fk_app
             * @classdesc Represents a LoraSettings.
             * @constructor
             * @param {fk_app.ILoraSettings=} [properties] Properties to set
             */
            function LoraSettings(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * LoraSettings available.
             * @member {boolean}available
             * @memberof fk_app.LoraSettings
             * @instance
             */
            LoraSettings.prototype.available = false;
    
            /**
             * LoraSettings modifying.
             * @member {boolean}modifying
             * @memberof fk_app.LoraSettings
             * @instance
             */
            LoraSettings.prototype.modifying = false;
    
            /**
             * LoraSettings clearing.
             * @member {boolean}clearing
             * @memberof fk_app.LoraSettings
             * @instance
             */
            LoraSettings.prototype.clearing = false;
    
            /**
             * LoraSettings frequencyBand.
             * @member {number}frequencyBand
             * @memberof fk_app.LoraSettings
             * @instance
             */
            LoraSettings.prototype.frequencyBand = 0;
    
            /**
             * LoraSettings deviceEui.
             * @member {Uint8Array}deviceEui
             * @memberof fk_app.LoraSettings
             * @instance
             */
            LoraSettings.prototype.deviceEui = $util.newBuffer([]);
    
            /**
             * LoraSettings appKey.
             * @member {Uint8Array}appKey
             * @memberof fk_app.LoraSettings
             * @instance
             */
            LoraSettings.prototype.appKey = $util.newBuffer([]);
    
            /**
             * LoraSettings joinEui.
             * @member {Uint8Array}joinEui
             * @memberof fk_app.LoraSettings
             * @instance
             */
            LoraSettings.prototype.joinEui = $util.newBuffer([]);
    
            /**
             * LoraSettings deviceAddress.
             * @member {Uint8Array}deviceAddress
             * @memberof fk_app.LoraSettings
             * @instance
             */
            LoraSettings.prototype.deviceAddress = $util.newBuffer([]);
    
            /**
             * LoraSettings networkSessionKey.
             * @member {Uint8Array}networkSessionKey
             * @memberof fk_app.LoraSettings
             * @instance
             */
            LoraSettings.prototype.networkSessionKey = $util.newBuffer([]);
    
            /**
             * LoraSettings appSessionKey.
             * @member {Uint8Array}appSessionKey
             * @memberof fk_app.LoraSettings
             * @instance
             */
            LoraSettings.prototype.appSessionKey = $util.newBuffer([]);
    
            /**
             * Creates a new LoraSettings instance using the specified properties.
             * @function create
             * @memberof fk_app.LoraSettings
             * @static
             * @param {fk_app.ILoraSettings=} [properties] Properties to set
             * @returns {fk_app.LoraSettings} LoraSettings instance
             */
            LoraSettings.create = function create(properties) {
                return new LoraSettings(properties);
            };
    
            /**
             * Encodes the specified LoraSettings message. Does not implicitly {@link fk_app.LoraSettings.verify|verify} messages.
             * @function encode
             * @memberof fk_app.LoraSettings
             * @static
             * @param {fk_app.ILoraSettings} message LoraSettings message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            LoraSettings.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.available != null && message.hasOwnProperty("available"))
                    writer.uint32(/* id 1, wireType 0 =*/8).bool(message.available);
                if (message.modifying != null && message.hasOwnProperty("modifying"))
                    writer.uint32(/* id 2, wireType 0 =*/16).bool(message.modifying);
                if (message.clearing != null && message.hasOwnProperty("clearing"))
                    writer.uint32(/* id 3, wireType 0 =*/24).bool(message.clearing);
                if (message.frequencyBand != null && message.hasOwnProperty("frequencyBand"))
                    writer.uint32(/* id 4, wireType 0 =*/32).uint32(message.frequencyBand);
                if (message.deviceEui != null && message.hasOwnProperty("deviceEui"))
                    writer.uint32(/* id 5, wireType 2 =*/42).bytes(message.deviceEui);
                if (message.appKey != null && message.hasOwnProperty("appKey"))
                    writer.uint32(/* id 6, wireType 2 =*/50).bytes(message.appKey);
                if (message.joinEui != null && message.hasOwnProperty("joinEui"))
                    writer.uint32(/* id 7, wireType 2 =*/58).bytes(message.joinEui);
                if (message.deviceAddress != null && message.hasOwnProperty("deviceAddress"))
                    writer.uint32(/* id 8, wireType 2 =*/66).bytes(message.deviceAddress);
                if (message.networkSessionKey != null && message.hasOwnProperty("networkSessionKey"))
                    writer.uint32(/* id 9, wireType 2 =*/74).bytes(message.networkSessionKey);
                if (message.appSessionKey != null && message.hasOwnProperty("appSessionKey"))
                    writer.uint32(/* id 10, wireType 2 =*/82).bytes(message.appSessionKey);
                return writer;
            };
    
            /**
             * Encodes the specified LoraSettings message, length delimited. Does not implicitly {@link fk_app.LoraSettings.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.LoraSettings
             * @static
             * @param {fk_app.ILoraSettings} message LoraSettings message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            LoraSettings.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a LoraSettings message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.LoraSettings
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.LoraSettings} LoraSettings
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            LoraSettings.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.LoraSettings();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.available = reader.bool();
                        break;
                    case 2:
                        message.modifying = reader.bool();
                        break;
                    case 3:
                        message.clearing = reader.bool();
                        break;
                    case 4:
                        message.frequencyBand = reader.uint32();
                        break;
                    case 5:
                        message.deviceEui = reader.bytes();
                        break;
                    case 6:
                        message.appKey = reader.bytes();
                        break;
                    case 7:
                        message.joinEui = reader.bytes();
                        break;
                    case 8:
                        message.deviceAddress = reader.bytes();
                        break;
                    case 9:
                        message.networkSessionKey = reader.bytes();
                        break;
                    case 10:
                        message.appSessionKey = reader.bytes();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a LoraSettings message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.LoraSettings
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.LoraSettings} LoraSettings
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            LoraSettings.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a LoraSettings message.
             * @function verify
             * @memberof fk_app.LoraSettings
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            LoraSettings.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.available != null && message.hasOwnProperty("available"))
                    if (typeof message.available !== "boolean")
                        return "available: boolean expected";
                if (message.modifying != null && message.hasOwnProperty("modifying"))
                    if (typeof message.modifying !== "boolean")
                        return "modifying: boolean expected";
                if (message.clearing != null && message.hasOwnProperty("clearing"))
                    if (typeof message.clearing !== "boolean")
                        return "clearing: boolean expected";
                if (message.frequencyBand != null && message.hasOwnProperty("frequencyBand"))
                    if (!$util.isInteger(message.frequencyBand))
                        return "frequencyBand: integer expected";
                if (message.deviceEui != null && message.hasOwnProperty("deviceEui"))
                    if (!(message.deviceEui && typeof message.deviceEui.length === "number" || $util.isString(message.deviceEui)))
                        return "deviceEui: buffer expected";
                if (message.appKey != null && message.hasOwnProperty("appKey"))
                    if (!(message.appKey && typeof message.appKey.length === "number" || $util.isString(message.appKey)))
                        return "appKey: buffer expected";
                if (message.joinEui != null && message.hasOwnProperty("joinEui"))
                    if (!(message.joinEui && typeof message.joinEui.length === "number" || $util.isString(message.joinEui)))
                        return "joinEui: buffer expected";
                if (message.deviceAddress != null && message.hasOwnProperty("deviceAddress"))
                    if (!(message.deviceAddress && typeof message.deviceAddress.length === "number" || $util.isString(message.deviceAddress)))
                        return "deviceAddress: buffer expected";
                if (message.networkSessionKey != null && message.hasOwnProperty("networkSessionKey"))
                    if (!(message.networkSessionKey && typeof message.networkSessionKey.length === "number" || $util.isString(message.networkSessionKey)))
                        return "networkSessionKey: buffer expected";
                if (message.appSessionKey != null && message.hasOwnProperty("appSessionKey"))
                    if (!(message.appSessionKey && typeof message.appSessionKey.length === "number" || $util.isString(message.appSessionKey)))
                        return "appSessionKey: buffer expected";
                return null;
            };
    
            /**
             * Creates a LoraSettings message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.LoraSettings
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.LoraSettings} LoraSettings
             */
            LoraSettings.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.LoraSettings)
                    return object;
                var message = new $root.fk_app.LoraSettings();
                if (object.available != null)
                    message.available = Boolean(object.available);
                if (object.modifying != null)
                    message.modifying = Boolean(object.modifying);
                if (object.clearing != null)
                    message.clearing = Boolean(object.clearing);
                if (object.frequencyBand != null)
                    message.frequencyBand = object.frequencyBand >>> 0;
                if (object.deviceEui != null)
                    if (typeof object.deviceEui === "string")
                        $util.base64.decode(object.deviceEui, message.deviceEui = $util.newBuffer($util.base64.length(object.deviceEui)), 0);
                    else if (object.deviceEui.length)
                        message.deviceEui = object.deviceEui;
                if (object.appKey != null)
                    if (typeof object.appKey === "string")
                        $util.base64.decode(object.appKey, message.appKey = $util.newBuffer($util.base64.length(object.appKey)), 0);
                    else if (object.appKey.length)
                        message.appKey = object.appKey;
                if (object.joinEui != null)
                    if (typeof object.joinEui === "string")
                        $util.base64.decode(object.joinEui, message.joinEui = $util.newBuffer($util.base64.length(object.joinEui)), 0);
                    else if (object.joinEui.length)
                        message.joinEui = object.joinEui;
                if (object.deviceAddress != null)
                    if (typeof object.deviceAddress === "string")
                        $util.base64.decode(object.deviceAddress, message.deviceAddress = $util.newBuffer($util.base64.length(object.deviceAddress)), 0);
                    else if (object.deviceAddress.length)
                        message.deviceAddress = object.deviceAddress;
                if (object.networkSessionKey != null)
                    if (typeof object.networkSessionKey === "string")
                        $util.base64.decode(object.networkSessionKey, message.networkSessionKey = $util.newBuffer($util.base64.length(object.networkSessionKey)), 0);
                    else if (object.networkSessionKey.length)
                        message.networkSessionKey = object.networkSessionKey;
                if (object.appSessionKey != null)
                    if (typeof object.appSessionKey === "string")
                        $util.base64.decode(object.appSessionKey, message.appSessionKey = $util.newBuffer($util.base64.length(object.appSessionKey)), 0);
                    else if (object.appSessionKey.length)
                        message.appSessionKey = object.appSessionKey;
                return message;
            };
    
            /**
             * Creates a plain object from a LoraSettings message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.LoraSettings
             * @static
             * @param {fk_app.LoraSettings} message LoraSettings
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            LoraSettings.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.available = false;
                    object.modifying = false;
                    object.clearing = false;
                    object.frequencyBand = 0;
                    object.deviceEui = options.bytes === String ? "" : [];
                    object.appKey = options.bytes === String ? "" : [];
                    object.joinEui = options.bytes === String ? "" : [];
                    object.deviceAddress = options.bytes === String ? "" : [];
                    object.networkSessionKey = options.bytes === String ? "" : [];
                    object.appSessionKey = options.bytes === String ? "" : [];
                }
                if (message.available != null && message.hasOwnProperty("available"))
                    object.available = message.available;
                if (message.modifying != null && message.hasOwnProperty("modifying"))
                    object.modifying = message.modifying;
                if (message.clearing != null && message.hasOwnProperty("clearing"))
                    object.clearing = message.clearing;
                if (message.frequencyBand != null && message.hasOwnProperty("frequencyBand"))
                    object.frequencyBand = message.frequencyBand;
                if (message.deviceEui != null && message.hasOwnProperty("deviceEui"))
                    object.deviceEui = options.bytes === String ? $util.base64.encode(message.deviceEui, 0, message.deviceEui.length) : options.bytes === Array ? Array.prototype.slice.call(message.deviceEui) : message.deviceEui;
                if (message.appKey != null && message.hasOwnProperty("appKey"))
                    object.appKey = options.bytes === String ? $util.base64.encode(message.appKey, 0, message.appKey.length) : options.bytes === Array ? Array.prototype.slice.call(message.appKey) : message.appKey;
                if (message.joinEui != null && message.hasOwnProperty("joinEui"))
                    object.joinEui = options.bytes === String ? $util.base64.encode(message.joinEui, 0, message.joinEui.length) : options.bytes === Array ? Array.prototype.slice.call(message.joinEui) : message.joinEui;
                if (message.deviceAddress != null && message.hasOwnProperty("deviceAddress"))
                    object.deviceAddress = options.bytes === String ? $util.base64.encode(message.deviceAddress, 0, message.deviceAddress.length) : options.bytes === Array ? Array.prototype.slice.call(message.deviceAddress) : message.deviceAddress;
                if (message.networkSessionKey != null && message.hasOwnProperty("networkSessionKey"))
                    object.networkSessionKey = options.bytes === String ? $util.base64.encode(message.networkSessionKey, 0, message.networkSessionKey.length) : options.bytes === Array ? Array.prototype.slice.call(message.networkSessionKey) : message.networkSessionKey;
                if (message.appSessionKey != null && message.hasOwnProperty("appSessionKey"))
                    object.appSessionKey = options.bytes === String ? $util.base64.encode(message.appSessionKey, 0, message.appSessionKey.length) : options.bytes === Array ? Array.prototype.slice.call(message.appSessionKey) : message.appSessionKey;
                return object;
            };
    
            /**
             * Converts this LoraSettings to JSON.
             * @function toJSON
             * @memberof fk_app.LoraSettings
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            LoraSettings.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return LoraSettings;
        })();
    
        fk_app.Location = (function() {
    
            /**
             * Properties of a Location.
             * @memberof fk_app
             * @interface ILocation
             * @property {boolean} [modifying] Location modifying
             * @property {number} [longitude] Location longitude
             * @property {number} [latitude] Location latitude
             * @property {number|Long} [time] Location time
             */
    
            /**
             * Constructs a new Location.
             * @memberof fk_app
             * @classdesc Represents a Location.
             * @constructor
             * @param {fk_app.ILocation=} [properties] Properties to set
             */
            function Location(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Location modifying.
             * @member {boolean}modifying
             * @memberof fk_app.Location
             * @instance
             */
            Location.prototype.modifying = false;
    
            /**
             * Location longitude.
             * @member {number}longitude
             * @memberof fk_app.Location
             * @instance
             */
            Location.prototype.longitude = 0;
    
            /**
             * Location latitude.
             * @member {number}latitude
             * @memberof fk_app.Location
             * @instance
             */
            Location.prototype.latitude = 0;
    
            /**
             * Location time.
             * @member {number|Long}time
             * @memberof fk_app.Location
             * @instance
             */
            Location.prototype.time = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * Creates a new Location instance using the specified properties.
             * @function create
             * @memberof fk_app.Location
             * @static
             * @param {fk_app.ILocation=} [properties] Properties to set
             * @returns {fk_app.Location} Location instance
             */
            Location.create = function create(properties) {
                return new Location(properties);
            };
    
            /**
             * Encodes the specified Location message. Does not implicitly {@link fk_app.Location.verify|verify} messages.
             * @function encode
             * @memberof fk_app.Location
             * @static
             * @param {fk_app.ILocation} message Location message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Location.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.modifying != null && message.hasOwnProperty("modifying"))
                    writer.uint32(/* id 1, wireType 0 =*/8).bool(message.modifying);
                if (message.longitude != null && message.hasOwnProperty("longitude"))
                    writer.uint32(/* id 2, wireType 5 =*/21).float(message.longitude);
                if (message.latitude != null && message.hasOwnProperty("latitude"))
                    writer.uint32(/* id 3, wireType 5 =*/29).float(message.latitude);
                if (message.time != null && message.hasOwnProperty("time"))
                    writer.uint32(/* id 4, wireType 0 =*/32).uint64(message.time);
                return writer;
            };
    
            /**
             * Encodes the specified Location message, length delimited. Does not implicitly {@link fk_app.Location.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.Location
             * @static
             * @param {fk_app.ILocation} message Location message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Location.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a Location message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.Location
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.Location} Location
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Location.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.Location();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.modifying = reader.bool();
                        break;
                    case 2:
                        message.longitude = reader.float();
                        break;
                    case 3:
                        message.latitude = reader.float();
                        break;
                    case 4:
                        message.time = reader.uint64();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a Location message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.Location
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.Location} Location
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Location.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a Location message.
             * @function verify
             * @memberof fk_app.Location
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            Location.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.modifying != null && message.hasOwnProperty("modifying"))
                    if (typeof message.modifying !== "boolean")
                        return "modifying: boolean expected";
                if (message.longitude != null && message.hasOwnProperty("longitude"))
                    if (typeof message.longitude !== "number")
                        return "longitude: number expected";
                if (message.latitude != null && message.hasOwnProperty("latitude"))
                    if (typeof message.latitude !== "number")
                        return "latitude: number expected";
                if (message.time != null && message.hasOwnProperty("time"))
                    if (!$util.isInteger(message.time) && !(message.time && $util.isInteger(message.time.low) && $util.isInteger(message.time.high)))
                        return "time: integer|Long expected";
                return null;
            };
    
            /**
             * Creates a Location message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.Location
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.Location} Location
             */
            Location.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.Location)
                    return object;
                var message = new $root.fk_app.Location();
                if (object.modifying != null)
                    message.modifying = Boolean(object.modifying);
                if (object.longitude != null)
                    message.longitude = Number(object.longitude);
                if (object.latitude != null)
                    message.latitude = Number(object.latitude);
                if (object.time != null)
                    if ($util.Long)
                        (message.time = $util.Long.fromValue(object.time)).unsigned = true;
                    else if (typeof object.time === "string")
                        message.time = parseInt(object.time, 10);
                    else if (typeof object.time === "number")
                        message.time = object.time;
                    else if (typeof object.time === "object")
                        message.time = new $util.LongBits(object.time.low >>> 0, object.time.high >>> 0).toNumber(true);
                return message;
            };
    
            /**
             * Creates a plain object from a Location message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.Location
             * @static
             * @param {fk_app.Location} message Location
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            Location.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.modifying = false;
                    object.longitude = 0;
                    object.latitude = 0;
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.time = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.time = options.longs === String ? "0" : 0;
                }
                if (message.modifying != null && message.hasOwnProperty("modifying"))
                    object.modifying = message.modifying;
                if (message.longitude != null && message.hasOwnProperty("longitude"))
                    object.longitude = options.json && !isFinite(message.longitude) ? String(message.longitude) : message.longitude;
                if (message.latitude != null && message.hasOwnProperty("latitude"))
                    object.latitude = options.json && !isFinite(message.latitude) ? String(message.latitude) : message.latitude;
                if (message.time != null && message.hasOwnProperty("time"))
                    if (typeof message.time === "number")
                        object.time = options.longs === String ? String(message.time) : message.time;
                    else
                        object.time = options.longs === String ? $util.Long.prototype.toString.call(message.time) : options.longs === Number ? new $util.LongBits(message.time.low >>> 0, message.time.high >>> 0).toNumber(true) : message.time;
                return object;
            };
    
            /**
             * Converts this Location to JSON.
             * @function toJSON
             * @memberof fk_app.Location
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Location.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Location;
        })();
    
        fk_app.WifiTransmission = (function() {
    
            /**
             * Properties of a WifiTransmission.
             * @memberof fk_app
             * @interface IWifiTransmission
             * @property {boolean} [modifying] WifiTransmission modifying
             * @property {string} [url] WifiTransmission url
             * @property {string} [token] WifiTransmission token
             * @property {boolean} [enabled] WifiTransmission enabled
             */
    
            /**
             * Constructs a new WifiTransmission.
             * @memberof fk_app
             * @classdesc Represents a WifiTransmission.
             * @constructor
             * @param {fk_app.IWifiTransmission=} [properties] Properties to set
             */
            function WifiTransmission(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * WifiTransmission modifying.
             * @member {boolean}modifying
             * @memberof fk_app.WifiTransmission
             * @instance
             */
            WifiTransmission.prototype.modifying = false;
    
            /**
             * WifiTransmission url.
             * @member {string}url
             * @memberof fk_app.WifiTransmission
             * @instance
             */
            WifiTransmission.prototype.url = "";
    
            /**
             * WifiTransmission token.
             * @member {string}token
             * @memberof fk_app.WifiTransmission
             * @instance
             */
            WifiTransmission.prototype.token = "";
    
            /**
             * WifiTransmission enabled.
             * @member {boolean}enabled
             * @memberof fk_app.WifiTransmission
             * @instance
             */
            WifiTransmission.prototype.enabled = false;
    
            /**
             * Creates a new WifiTransmission instance using the specified properties.
             * @function create
             * @memberof fk_app.WifiTransmission
             * @static
             * @param {fk_app.IWifiTransmission=} [properties] Properties to set
             * @returns {fk_app.WifiTransmission} WifiTransmission instance
             */
            WifiTransmission.create = function create(properties) {
                return new WifiTransmission(properties);
            };
    
            /**
             * Encodes the specified WifiTransmission message. Does not implicitly {@link fk_app.WifiTransmission.verify|verify} messages.
             * @function encode
             * @memberof fk_app.WifiTransmission
             * @static
             * @param {fk_app.IWifiTransmission} message WifiTransmission message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            WifiTransmission.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.modifying != null && message.hasOwnProperty("modifying"))
                    writer.uint32(/* id 1, wireType 0 =*/8).bool(message.modifying);
                if (message.url != null && message.hasOwnProperty("url"))
                    writer.uint32(/* id 2, wireType 2 =*/18).string(message.url);
                if (message.token != null && message.hasOwnProperty("token"))
                    writer.uint32(/* id 3, wireType 2 =*/26).string(message.token);
                if (message.enabled != null && message.hasOwnProperty("enabled"))
                    writer.uint32(/* id 4, wireType 0 =*/32).bool(message.enabled);
                return writer;
            };
    
            /**
             * Encodes the specified WifiTransmission message, length delimited. Does not implicitly {@link fk_app.WifiTransmission.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.WifiTransmission
             * @static
             * @param {fk_app.IWifiTransmission} message WifiTransmission message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            WifiTransmission.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a WifiTransmission message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.WifiTransmission
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.WifiTransmission} WifiTransmission
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            WifiTransmission.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.WifiTransmission();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.modifying = reader.bool();
                        break;
                    case 2:
                        message.url = reader.string();
                        break;
                    case 3:
                        message.token = reader.string();
                        break;
                    case 4:
                        message.enabled = reader.bool();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a WifiTransmission message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.WifiTransmission
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.WifiTransmission} WifiTransmission
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            WifiTransmission.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a WifiTransmission message.
             * @function verify
             * @memberof fk_app.WifiTransmission
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            WifiTransmission.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.modifying != null && message.hasOwnProperty("modifying"))
                    if (typeof message.modifying !== "boolean")
                        return "modifying: boolean expected";
                if (message.url != null && message.hasOwnProperty("url"))
                    if (!$util.isString(message.url))
                        return "url: string expected";
                if (message.token != null && message.hasOwnProperty("token"))
                    if (!$util.isString(message.token))
                        return "token: string expected";
                if (message.enabled != null && message.hasOwnProperty("enabled"))
                    if (typeof message.enabled !== "boolean")
                        return "enabled: boolean expected";
                return null;
            };
    
            /**
             * Creates a WifiTransmission message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.WifiTransmission
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.WifiTransmission} WifiTransmission
             */
            WifiTransmission.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.WifiTransmission)
                    return object;
                var message = new $root.fk_app.WifiTransmission();
                if (object.modifying != null)
                    message.modifying = Boolean(object.modifying);
                if (object.url != null)
                    message.url = String(object.url);
                if (object.token != null)
                    message.token = String(object.token);
                if (object.enabled != null)
                    message.enabled = Boolean(object.enabled);
                return message;
            };
    
            /**
             * Creates a plain object from a WifiTransmission message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.WifiTransmission
             * @static
             * @param {fk_app.WifiTransmission} message WifiTransmission
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            WifiTransmission.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.modifying = false;
                    object.url = "";
                    object.token = "";
                    object.enabled = false;
                }
                if (message.modifying != null && message.hasOwnProperty("modifying"))
                    object.modifying = message.modifying;
                if (message.url != null && message.hasOwnProperty("url"))
                    object.url = message.url;
                if (message.token != null && message.hasOwnProperty("token"))
                    object.token = message.token;
                if (message.enabled != null && message.hasOwnProperty("enabled"))
                    object.enabled = message.enabled;
                return object;
            };
    
            /**
             * Converts this WifiTransmission to JSON.
             * @function toJSON
             * @memberof fk_app.WifiTransmission
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            WifiTransmission.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return WifiTransmission;
        })();
    
        fk_app.Transmission = (function() {
    
            /**
             * Properties of a Transmission.
             * @memberof fk_app
             * @interface ITransmission
             * @property {fk_app.IWifiTransmission} [wifi] Transmission wifi
             */
    
            /**
             * Constructs a new Transmission.
             * @memberof fk_app
             * @classdesc Represents a Transmission.
             * @constructor
             * @param {fk_app.ITransmission=} [properties] Properties to set
             */
            function Transmission(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Transmission wifi.
             * @member {(fk_app.IWifiTransmission|null|undefined)}wifi
             * @memberof fk_app.Transmission
             * @instance
             */
            Transmission.prototype.wifi = null;
    
            /**
             * Creates a new Transmission instance using the specified properties.
             * @function create
             * @memberof fk_app.Transmission
             * @static
             * @param {fk_app.ITransmission=} [properties] Properties to set
             * @returns {fk_app.Transmission} Transmission instance
             */
            Transmission.create = function create(properties) {
                return new Transmission(properties);
            };
    
            /**
             * Encodes the specified Transmission message. Does not implicitly {@link fk_app.Transmission.verify|verify} messages.
             * @function encode
             * @memberof fk_app.Transmission
             * @static
             * @param {fk_app.ITransmission} message Transmission message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Transmission.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.wifi != null && message.hasOwnProperty("wifi"))
                    $root.fk_app.WifiTransmission.encode(message.wifi, writer.uint32(/* id 1, wireType 2 =*/10).fork()).ldelim();
                return writer;
            };
    
            /**
             * Encodes the specified Transmission message, length delimited. Does not implicitly {@link fk_app.Transmission.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.Transmission
             * @static
             * @param {fk_app.ITransmission} message Transmission message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Transmission.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a Transmission message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.Transmission
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.Transmission} Transmission
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Transmission.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.Transmission();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.wifi = $root.fk_app.WifiTransmission.decode(reader, reader.uint32());
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a Transmission message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.Transmission
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.Transmission} Transmission
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Transmission.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a Transmission message.
             * @function verify
             * @memberof fk_app.Transmission
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            Transmission.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.wifi != null && message.hasOwnProperty("wifi")) {
                    var error = $root.fk_app.WifiTransmission.verify(message.wifi);
                    if (error)
                        return "wifi." + error;
                }
                return null;
            };
    
            /**
             * Creates a Transmission message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.Transmission
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.Transmission} Transmission
             */
            Transmission.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.Transmission)
                    return object;
                var message = new $root.fk_app.Transmission();
                if (object.wifi != null) {
                    if (typeof object.wifi !== "object")
                        throw TypeError(".fk_app.Transmission.wifi: object expected");
                    message.wifi = $root.fk_app.WifiTransmission.fromObject(object.wifi);
                }
                return message;
            };
    
            /**
             * Creates a plain object from a Transmission message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.Transmission
             * @static
             * @param {fk_app.Transmission} message Transmission
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            Transmission.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults)
                    object.wifi = null;
                if (message.wifi != null && message.hasOwnProperty("wifi"))
                    object.wifi = $root.fk_app.WifiTransmission.toObject(message.wifi, options);
                return object;
            };
    
            /**
             * Converts this Transmission to JSON.
             * @function toJSON
             * @memberof fk_app.Transmission
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Transmission.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Transmission;
        })();
    
        fk_app.ListDirectory = (function() {
    
            /**
             * Properties of a ListDirectory.
             * @memberof fk_app
             * @interface IListDirectory
             * @property {string} [path] ListDirectory path
             * @property {number} [page] ListDirectory page
             */
    
            /**
             * Constructs a new ListDirectory.
             * @memberof fk_app
             * @classdesc Represents a ListDirectory.
             * @constructor
             * @param {fk_app.IListDirectory=} [properties] Properties to set
             */
            function ListDirectory(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * ListDirectory path.
             * @member {string}path
             * @memberof fk_app.ListDirectory
             * @instance
             */
            ListDirectory.prototype.path = "";
    
            /**
             * ListDirectory page.
             * @member {number}page
             * @memberof fk_app.ListDirectory
             * @instance
             */
            ListDirectory.prototype.page = 0;
    
            /**
             * Creates a new ListDirectory instance using the specified properties.
             * @function create
             * @memberof fk_app.ListDirectory
             * @static
             * @param {fk_app.IListDirectory=} [properties] Properties to set
             * @returns {fk_app.ListDirectory} ListDirectory instance
             */
            ListDirectory.create = function create(properties) {
                return new ListDirectory(properties);
            };
    
            /**
             * Encodes the specified ListDirectory message. Does not implicitly {@link fk_app.ListDirectory.verify|verify} messages.
             * @function encode
             * @memberof fk_app.ListDirectory
             * @static
             * @param {fk_app.IListDirectory} message ListDirectory message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            ListDirectory.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.path != null && message.hasOwnProperty("path"))
                    writer.uint32(/* id 1, wireType 2 =*/10).string(message.path);
                if (message.page != null && message.hasOwnProperty("page"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint32(message.page);
                return writer;
            };
    
            /**
             * Encodes the specified ListDirectory message, length delimited. Does not implicitly {@link fk_app.ListDirectory.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.ListDirectory
             * @static
             * @param {fk_app.IListDirectory} message ListDirectory message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            ListDirectory.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a ListDirectory message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.ListDirectory
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.ListDirectory} ListDirectory
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            ListDirectory.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.ListDirectory();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.path = reader.string();
                        break;
                    case 2:
                        message.page = reader.uint32();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a ListDirectory message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.ListDirectory
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.ListDirectory} ListDirectory
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            ListDirectory.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a ListDirectory message.
             * @function verify
             * @memberof fk_app.ListDirectory
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            ListDirectory.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.path != null && message.hasOwnProperty("path"))
                    if (!$util.isString(message.path))
                        return "path: string expected";
                if (message.page != null && message.hasOwnProperty("page"))
                    if (!$util.isInteger(message.page))
                        return "page: integer expected";
                return null;
            };
    
            /**
             * Creates a ListDirectory message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.ListDirectory
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.ListDirectory} ListDirectory
             */
            ListDirectory.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.ListDirectory)
                    return object;
                var message = new $root.fk_app.ListDirectory();
                if (object.path != null)
                    message.path = String(object.path);
                if (object.page != null)
                    message.page = object.page >>> 0;
                return message;
            };
    
            /**
             * Creates a plain object from a ListDirectory message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.ListDirectory
             * @static
             * @param {fk_app.ListDirectory} message ListDirectory
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            ListDirectory.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.path = "";
                    object.page = 0;
                }
                if (message.path != null && message.hasOwnProperty("path"))
                    object.path = message.path;
                if (message.page != null && message.hasOwnProperty("page"))
                    object.page = message.page;
                return object;
            };
    
            /**
             * Converts this ListDirectory to JSON.
             * @function toJSON
             * @memberof fk_app.ListDirectory
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            ListDirectory.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return ListDirectory;
        })();
    
        fk_app.HttpQuery = (function() {
    
            /**
             * Properties of a HttpQuery.
             * @memberof fk_app
             * @interface IHttpQuery
             * @property {fk_app.QueryType} [type] HttpQuery type
             * @property {fk_app.IIdentity} [identity] HttpQuery identity
             * @property {fk_app.IRecording} [recording] HttpQuery recording
             * @property {fk_app.ISchedules} [schedules] HttpQuery schedules
             * @property {fk_app.INetworkSettings} [networkSettings] HttpQuery networkSettings
             * @property {fk_app.ILoraSettings} [loraSettings] HttpQuery loraSettings
             * @property {fk_app.ILocation} [locate] HttpQuery locate
             * @property {fk_app.ITransmission} [transmission] HttpQuery transmission
             * @property {fk_app.IListDirectory} [directory] HttpQuery directory
             * @property {number} [flags] HttpQuery flags
             * @property {number|Long} [time] HttpQuery time
             * @property {number} [counter] HttpQuery counter
             */
    
            /**
             * Constructs a new HttpQuery.
             * @memberof fk_app
             * @classdesc Represents a HttpQuery.
             * @constructor
             * @param {fk_app.IHttpQuery=} [properties] Properties to set
             */
            function HttpQuery(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * HttpQuery type.
             * @member {fk_app.QueryType}type
             * @memberof fk_app.HttpQuery
             * @instance
             */
            HttpQuery.prototype.type = 0;
    
            /**
             * HttpQuery identity.
             * @member {(fk_app.IIdentity|null|undefined)}identity
             * @memberof fk_app.HttpQuery
             * @instance
             */
            HttpQuery.prototype.identity = null;
    
            /**
             * HttpQuery recording.
             * @member {(fk_app.IRecording|null|undefined)}recording
             * @memberof fk_app.HttpQuery
             * @instance
             */
            HttpQuery.prototype.recording = null;
    
            /**
             * HttpQuery schedules.
             * @member {(fk_app.ISchedules|null|undefined)}schedules
             * @memberof fk_app.HttpQuery
             * @instance
             */
            HttpQuery.prototype.schedules = null;
    
            /**
             * HttpQuery networkSettings.
             * @member {(fk_app.INetworkSettings|null|undefined)}networkSettings
             * @memberof fk_app.HttpQuery
             * @instance
             */
            HttpQuery.prototype.networkSettings = null;
    
            /**
             * HttpQuery loraSettings.
             * @member {(fk_app.ILoraSettings|null|undefined)}loraSettings
             * @memberof fk_app.HttpQuery
             * @instance
             */
            HttpQuery.prototype.loraSettings = null;
    
            /**
             * HttpQuery locate.
             * @member {(fk_app.ILocation|null|undefined)}locate
             * @memberof fk_app.HttpQuery
             * @instance
             */
            HttpQuery.prototype.locate = null;
    
            /**
             * HttpQuery transmission.
             * @member {(fk_app.ITransmission|null|undefined)}transmission
             * @memberof fk_app.HttpQuery
             * @instance
             */
            HttpQuery.prototype.transmission = null;
    
            /**
             * HttpQuery directory.
             * @member {(fk_app.IListDirectory|null|undefined)}directory
             * @memberof fk_app.HttpQuery
             * @instance
             */
            HttpQuery.prototype.directory = null;
    
            /**
             * HttpQuery flags.
             * @member {number}flags
             * @memberof fk_app.HttpQuery
             * @instance
             */
            HttpQuery.prototype.flags = 0;
    
            /**
             * HttpQuery time.
             * @member {number|Long}time
             * @memberof fk_app.HttpQuery
             * @instance
             */
            HttpQuery.prototype.time = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * HttpQuery counter.
             * @member {number}counter
             * @memberof fk_app.HttpQuery
             * @instance
             */
            HttpQuery.prototype.counter = 0;
    
            /**
             * Creates a new HttpQuery instance using the specified properties.
             * @function create
             * @memberof fk_app.HttpQuery
             * @static
             * @param {fk_app.IHttpQuery=} [properties] Properties to set
             * @returns {fk_app.HttpQuery} HttpQuery instance
             */
            HttpQuery.create = function create(properties) {
                return new HttpQuery(properties);
            };
    
            /**
             * Encodes the specified HttpQuery message. Does not implicitly {@link fk_app.HttpQuery.verify|verify} messages.
             * @function encode
             * @memberof fk_app.HttpQuery
             * @static
             * @param {fk_app.IHttpQuery} message HttpQuery message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            HttpQuery.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.type != null && message.hasOwnProperty("type"))
                    writer.uint32(/* id 1, wireType 0 =*/8).int32(message.type);
                if (message.identity != null && message.hasOwnProperty("identity"))
                    $root.fk_app.Identity.encode(message.identity, writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
                if (message.recording != null && message.hasOwnProperty("recording"))
                    $root.fk_app.Recording.encode(message.recording, writer.uint32(/* id 3, wireType 2 =*/26).fork()).ldelim();
                if (message.schedules != null && message.hasOwnProperty("schedules"))
                    $root.fk_app.Schedules.encode(message.schedules, writer.uint32(/* id 4, wireType 2 =*/34).fork()).ldelim();
                if (message.flags != null && message.hasOwnProperty("flags"))
                    writer.uint32(/* id 5, wireType 0 =*/40).uint32(message.flags);
                if (message.networkSettings != null && message.hasOwnProperty("networkSettings"))
                    $root.fk_app.NetworkSettings.encode(message.networkSettings, writer.uint32(/* id 6, wireType 2 =*/50).fork()).ldelim();
                if (message.loraSettings != null && message.hasOwnProperty("loraSettings"))
                    $root.fk_app.LoraSettings.encode(message.loraSettings, writer.uint32(/* id 7, wireType 2 =*/58).fork()).ldelim();
                if (message.time != null && message.hasOwnProperty("time"))
                    writer.uint32(/* id 8, wireType 0 =*/64).uint64(message.time);
                if (message.locate != null && message.hasOwnProperty("locate"))
                    $root.fk_app.Location.encode(message.locate, writer.uint32(/* id 9, wireType 2 =*/74).fork()).ldelim();
                if (message.transmission != null && message.hasOwnProperty("transmission"))
                    $root.fk_app.Transmission.encode(message.transmission, writer.uint32(/* id 10, wireType 2 =*/82).fork()).ldelim();
                if (message.directory != null && message.hasOwnProperty("directory"))
                    $root.fk_app.ListDirectory.encode(message.directory, writer.uint32(/* id 11, wireType 2 =*/90).fork()).ldelim();
                if (message.counter != null && message.hasOwnProperty("counter"))
                    writer.uint32(/* id 12, wireType 0 =*/96).uint32(message.counter);
                return writer;
            };
    
            /**
             * Encodes the specified HttpQuery message, length delimited. Does not implicitly {@link fk_app.HttpQuery.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.HttpQuery
             * @static
             * @param {fk_app.IHttpQuery} message HttpQuery message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            HttpQuery.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a HttpQuery message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.HttpQuery
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.HttpQuery} HttpQuery
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            HttpQuery.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.HttpQuery();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.type = reader.int32();
                        break;
                    case 2:
                        message.identity = $root.fk_app.Identity.decode(reader, reader.uint32());
                        break;
                    case 3:
                        message.recording = $root.fk_app.Recording.decode(reader, reader.uint32());
                        break;
                    case 4:
                        message.schedules = $root.fk_app.Schedules.decode(reader, reader.uint32());
                        break;
                    case 6:
                        message.networkSettings = $root.fk_app.NetworkSettings.decode(reader, reader.uint32());
                        break;
                    case 7:
                        message.loraSettings = $root.fk_app.LoraSettings.decode(reader, reader.uint32());
                        break;
                    case 9:
                        message.locate = $root.fk_app.Location.decode(reader, reader.uint32());
                        break;
                    case 10:
                        message.transmission = $root.fk_app.Transmission.decode(reader, reader.uint32());
                        break;
                    case 11:
                        message.directory = $root.fk_app.ListDirectory.decode(reader, reader.uint32());
                        break;
                    case 5:
                        message.flags = reader.uint32();
                        break;
                    case 8:
                        message.time = reader.uint64();
                        break;
                    case 12:
                        message.counter = reader.uint32();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a HttpQuery message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.HttpQuery
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.HttpQuery} HttpQuery
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            HttpQuery.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a HttpQuery message.
             * @function verify
             * @memberof fk_app.HttpQuery
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            HttpQuery.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.type != null && message.hasOwnProperty("type"))
                    switch (message.type) {
                    default:
                        return "type: enum value expected";
                    case 0:
                    case 1:
                    case 2:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                        break;
                    }
                if (message.identity != null && message.hasOwnProperty("identity")) {
                    var error = $root.fk_app.Identity.verify(message.identity);
                    if (error)
                        return "identity." + error;
                }
                if (message.recording != null && message.hasOwnProperty("recording")) {
                    error = $root.fk_app.Recording.verify(message.recording);
                    if (error)
                        return "recording." + error;
                }
                if (message.schedules != null && message.hasOwnProperty("schedules")) {
                    error = $root.fk_app.Schedules.verify(message.schedules);
                    if (error)
                        return "schedules." + error;
                }
                if (message.networkSettings != null && message.hasOwnProperty("networkSettings")) {
                    error = $root.fk_app.NetworkSettings.verify(message.networkSettings);
                    if (error)
                        return "networkSettings." + error;
                }
                if (message.loraSettings != null && message.hasOwnProperty("loraSettings")) {
                    error = $root.fk_app.LoraSettings.verify(message.loraSettings);
                    if (error)
                        return "loraSettings." + error;
                }
                if (message.locate != null && message.hasOwnProperty("locate")) {
                    error = $root.fk_app.Location.verify(message.locate);
                    if (error)
                        return "locate." + error;
                }
                if (message.transmission != null && message.hasOwnProperty("transmission")) {
                    error = $root.fk_app.Transmission.verify(message.transmission);
                    if (error)
                        return "transmission." + error;
                }
                if (message.directory != null && message.hasOwnProperty("directory")) {
                    error = $root.fk_app.ListDirectory.verify(message.directory);
                    if (error)
                        return "directory." + error;
                }
                if (message.flags != null && message.hasOwnProperty("flags"))
                    if (!$util.isInteger(message.flags))
                        return "flags: integer expected";
                if (message.time != null && message.hasOwnProperty("time"))
                    if (!$util.isInteger(message.time) && !(message.time && $util.isInteger(message.time.low) && $util.isInteger(message.time.high)))
                        return "time: integer|Long expected";
                if (message.counter != null && message.hasOwnProperty("counter"))
                    if (!$util.isInteger(message.counter))
                        return "counter: integer expected";
                return null;
            };
    
            /**
             * Creates a HttpQuery message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.HttpQuery
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.HttpQuery} HttpQuery
             */
            HttpQuery.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.HttpQuery)
                    return object;
                var message = new $root.fk_app.HttpQuery();
                switch (object.type) {
                case "QUERY_NONE":
                case 0:
                    message.type = 0;
                    break;
                case "QUERY_CAPABILITIES":
                case 1:
                    message.type = 1;
                    break;
                case "QUERY_CONFIGURE_SENSOR":
                case 2:
                    message.type = 2;
                    break;
                case "QUERY_LIVE_DATA_POLL":
                case 7:
                    message.type = 7;
                    break;
                case "QUERY_SCHEDULES":
                case 8:
                    message.type = 8;
                    break;
                case "QUERY_CONFIGURE_SCHEDULES":
                case 9:
                    message.type = 9;
                    break;
                case "QUERY_FILES_SD":
                case 10:
                    message.type = 10;
                    break;
                case "QUERY_DOWNLOAD_FILE":
                case 11:
                    message.type = 11;
                    break;
                case "QUERY_ERASE_FILE":
                case 12:
                    message.type = 12;
                    break;
                case "QUERY_RESET":
                case 13:
                    message.type = 13;
                    break;
                case "QUERY_NETWORK_SETTINGS":
                case 14:
                    message.type = 14;
                    break;
                case "QUERY_CONFIGURE_NETWORK_SETTINGS":
                case 15:
                    message.type = 15;
                    break;
                case "QUERY_SCAN_MODULES":
                case 16:
                    message.type = 16;
                    break;
                case "QUERY_CONFIGURE_IDENTITY":
                case 17:
                    message.type = 17;
                    break;
                case "QUERY_STATUS":
                case 18:
                    message.type = 18;
                    break;
                case "QUERY_MODULE":
                case 19:
                    message.type = 19;
                    break;
                case "QUERY_METADATA":
                case 20:
                    message.type = 20;
                    break;
                case "QUERY_FORMAT":
                case 21:
                    message.type = 21;
                    break;
                case "QUERY_GET_READINGS":
                case 22:
                    message.type = 22;
                    break;
                case "QUERY_TAKE_READINGS":
                case 23:
                    message.type = 23;
                    break;
                case "QUERY_RECORDING_CONTROL":
                case 24:
                    message.type = 24;
                    break;
                case "QUERY_CONFIGURE":
                case 25:
                    message.type = 25;
                    break;
                case "QUERY_SCAN_NETWORKS":
                case 26:
                    message.type = 26;
                    break;
                case "QUERY_FILES_SPI":
                case 27:
                    message.type = 27;
                    break;
                case "QUERY_FILES_QSPI":
                case 28:
                    message.type = 28;
                    break;
                }
                if (object.identity != null) {
                    if (typeof object.identity !== "object")
                        throw TypeError(".fk_app.HttpQuery.identity: object expected");
                    message.identity = $root.fk_app.Identity.fromObject(object.identity);
                }
                if (object.recording != null) {
                    if (typeof object.recording !== "object")
                        throw TypeError(".fk_app.HttpQuery.recording: object expected");
                    message.recording = $root.fk_app.Recording.fromObject(object.recording);
                }
                if (object.schedules != null) {
                    if (typeof object.schedules !== "object")
                        throw TypeError(".fk_app.HttpQuery.schedules: object expected");
                    message.schedules = $root.fk_app.Schedules.fromObject(object.schedules);
                }
                if (object.networkSettings != null) {
                    if (typeof object.networkSettings !== "object")
                        throw TypeError(".fk_app.HttpQuery.networkSettings: object expected");
                    message.networkSettings = $root.fk_app.NetworkSettings.fromObject(object.networkSettings);
                }
                if (object.loraSettings != null) {
                    if (typeof object.loraSettings !== "object")
                        throw TypeError(".fk_app.HttpQuery.loraSettings: object expected");
                    message.loraSettings = $root.fk_app.LoraSettings.fromObject(object.loraSettings);
                }
                if (object.locate != null) {
                    if (typeof object.locate !== "object")
                        throw TypeError(".fk_app.HttpQuery.locate: object expected");
                    message.locate = $root.fk_app.Location.fromObject(object.locate);
                }
                if (object.transmission != null) {
                    if (typeof object.transmission !== "object")
                        throw TypeError(".fk_app.HttpQuery.transmission: object expected");
                    message.transmission = $root.fk_app.Transmission.fromObject(object.transmission);
                }
                if (object.directory != null) {
                    if (typeof object.directory !== "object")
                        throw TypeError(".fk_app.HttpQuery.directory: object expected");
                    message.directory = $root.fk_app.ListDirectory.fromObject(object.directory);
                }
                if (object.flags != null)
                    message.flags = object.flags >>> 0;
                if (object.time != null)
                    if ($util.Long)
                        (message.time = $util.Long.fromValue(object.time)).unsigned = true;
                    else if (typeof object.time === "string")
                        message.time = parseInt(object.time, 10);
                    else if (typeof object.time === "number")
                        message.time = object.time;
                    else if (typeof object.time === "object")
                        message.time = new $util.LongBits(object.time.low >>> 0, object.time.high >>> 0).toNumber(true);
                if (object.counter != null)
                    message.counter = object.counter >>> 0;
                return message;
            };
    
            /**
             * Creates a plain object from a HttpQuery message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.HttpQuery
             * @static
             * @param {fk_app.HttpQuery} message HttpQuery
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            HttpQuery.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.type = options.enums === String ? "QUERY_NONE" : 0;
                    object.identity = null;
                    object.recording = null;
                    object.schedules = null;
                    object.flags = 0;
                    object.networkSettings = null;
                    object.loraSettings = null;
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.time = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.time = options.longs === String ? "0" : 0;
                    object.locate = null;
                    object.transmission = null;
                    object.directory = null;
                    object.counter = 0;
                }
                if (message.type != null && message.hasOwnProperty("type"))
                    object.type = options.enums === String ? $root.fk_app.QueryType[message.type] : message.type;
                if (message.identity != null && message.hasOwnProperty("identity"))
                    object.identity = $root.fk_app.Identity.toObject(message.identity, options);
                if (message.recording != null && message.hasOwnProperty("recording"))
                    object.recording = $root.fk_app.Recording.toObject(message.recording, options);
                if (message.schedules != null && message.hasOwnProperty("schedules"))
                    object.schedules = $root.fk_app.Schedules.toObject(message.schedules, options);
                if (message.flags != null && message.hasOwnProperty("flags"))
                    object.flags = message.flags;
                if (message.networkSettings != null && message.hasOwnProperty("networkSettings"))
                    object.networkSettings = $root.fk_app.NetworkSettings.toObject(message.networkSettings, options);
                if (message.loraSettings != null && message.hasOwnProperty("loraSettings"))
                    object.loraSettings = $root.fk_app.LoraSettings.toObject(message.loraSettings, options);
                if (message.time != null && message.hasOwnProperty("time"))
                    if (typeof message.time === "number")
                        object.time = options.longs === String ? String(message.time) : message.time;
                    else
                        object.time = options.longs === String ? $util.Long.prototype.toString.call(message.time) : options.longs === Number ? new $util.LongBits(message.time.low >>> 0, message.time.high >>> 0).toNumber(true) : message.time;
                if (message.locate != null && message.hasOwnProperty("locate"))
                    object.locate = $root.fk_app.Location.toObject(message.locate, options);
                if (message.transmission != null && message.hasOwnProperty("transmission"))
                    object.transmission = $root.fk_app.Transmission.toObject(message.transmission, options);
                if (message.directory != null && message.hasOwnProperty("directory"))
                    object.directory = $root.fk_app.ListDirectory.toObject(message.directory, options);
                if (message.counter != null && message.hasOwnProperty("counter"))
                    object.counter = message.counter;
                return object;
            };
    
            /**
             * Converts this HttpQuery to JSON.
             * @function toJSON
             * @memberof fk_app.HttpQuery
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            HttpQuery.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return HttpQuery;
        })();
    
        fk_app.DataStream = (function() {
    
            /**
             * Properties of a DataStream.
             * @memberof fk_app
             * @interface IDataStream
             * @property {number} [id] DataStream id
             * @property {number|Long} [time] DataStream time
             * @property {number|Long} [size] DataStream size
             * @property {number} [version] DataStream version
             * @property {number|Long} [block] DataStream block
             * @property {Uint8Array} [hash] DataStream hash
             * @property {string} [name] DataStream name
             * @property {string} [path] DataStream path
             */
    
            /**
             * Constructs a new DataStream.
             * @memberof fk_app
             * @classdesc Represents a DataStream.
             * @constructor
             * @param {fk_app.IDataStream=} [properties] Properties to set
             */
            function DataStream(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * DataStream id.
             * @member {number}id
             * @memberof fk_app.DataStream
             * @instance
             */
            DataStream.prototype.id = 0;
    
            /**
             * DataStream time.
             * @member {number|Long}time
             * @memberof fk_app.DataStream
             * @instance
             */
            DataStream.prototype.time = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * DataStream size.
             * @member {number|Long}size
             * @memberof fk_app.DataStream
             * @instance
             */
            DataStream.prototype.size = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * DataStream version.
             * @member {number}version
             * @memberof fk_app.DataStream
             * @instance
             */
            DataStream.prototype.version = 0;
    
            /**
             * DataStream block.
             * @member {number|Long}block
             * @memberof fk_app.DataStream
             * @instance
             */
            DataStream.prototype.block = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * DataStream hash.
             * @member {Uint8Array}hash
             * @memberof fk_app.DataStream
             * @instance
             */
            DataStream.prototype.hash = $util.newBuffer([]);
    
            /**
             * DataStream name.
             * @member {string}name
             * @memberof fk_app.DataStream
             * @instance
             */
            DataStream.prototype.name = "";
    
            /**
             * DataStream path.
             * @member {string}path
             * @memberof fk_app.DataStream
             * @instance
             */
            DataStream.prototype.path = "";
    
            /**
             * Creates a new DataStream instance using the specified properties.
             * @function create
             * @memberof fk_app.DataStream
             * @static
             * @param {fk_app.IDataStream=} [properties] Properties to set
             * @returns {fk_app.DataStream} DataStream instance
             */
            DataStream.create = function create(properties) {
                return new DataStream(properties);
            };
    
            /**
             * Encodes the specified DataStream message. Does not implicitly {@link fk_app.DataStream.verify|verify} messages.
             * @function encode
             * @memberof fk_app.DataStream
             * @static
             * @param {fk_app.IDataStream} message DataStream message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            DataStream.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.id != null && message.hasOwnProperty("id"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.id);
                if (message.time != null && message.hasOwnProperty("time"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint64(message.time);
                if (message.size != null && message.hasOwnProperty("size"))
                    writer.uint32(/* id 3, wireType 0 =*/24).uint64(message.size);
                if (message.version != null && message.hasOwnProperty("version"))
                    writer.uint32(/* id 4, wireType 0 =*/32).uint32(message.version);
                if (message.block != null && message.hasOwnProperty("block"))
                    writer.uint32(/* id 5, wireType 0 =*/40).uint64(message.block);
                if (message.hash != null && message.hasOwnProperty("hash"))
                    writer.uint32(/* id 6, wireType 2 =*/50).bytes(message.hash);
                if (message.name != null && message.hasOwnProperty("name"))
                    writer.uint32(/* id 7, wireType 2 =*/58).string(message.name);
                if (message.path != null && message.hasOwnProperty("path"))
                    writer.uint32(/* id 8, wireType 2 =*/66).string(message.path);
                return writer;
            };
    
            /**
             * Encodes the specified DataStream message, length delimited. Does not implicitly {@link fk_app.DataStream.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.DataStream
             * @static
             * @param {fk_app.IDataStream} message DataStream message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            DataStream.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a DataStream message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.DataStream
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.DataStream} DataStream
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            DataStream.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.DataStream();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.id = reader.uint32();
                        break;
                    case 2:
                        message.time = reader.uint64();
                        break;
                    case 3:
                        message.size = reader.uint64();
                        break;
                    case 4:
                        message.version = reader.uint32();
                        break;
                    case 5:
                        message.block = reader.uint64();
                        break;
                    case 6:
                        message.hash = reader.bytes();
                        break;
                    case 7:
                        message.name = reader.string();
                        break;
                    case 8:
                        message.path = reader.string();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a DataStream message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.DataStream
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.DataStream} DataStream
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            DataStream.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a DataStream message.
             * @function verify
             * @memberof fk_app.DataStream
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            DataStream.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.id != null && message.hasOwnProperty("id"))
                    if (!$util.isInteger(message.id))
                        return "id: integer expected";
                if (message.time != null && message.hasOwnProperty("time"))
                    if (!$util.isInteger(message.time) && !(message.time && $util.isInteger(message.time.low) && $util.isInteger(message.time.high)))
                        return "time: integer|Long expected";
                if (message.size != null && message.hasOwnProperty("size"))
                    if (!$util.isInteger(message.size) && !(message.size && $util.isInteger(message.size.low) && $util.isInteger(message.size.high)))
                        return "size: integer|Long expected";
                if (message.version != null && message.hasOwnProperty("version"))
                    if (!$util.isInteger(message.version))
                        return "version: integer expected";
                if (message.block != null && message.hasOwnProperty("block"))
                    if (!$util.isInteger(message.block) && !(message.block && $util.isInteger(message.block.low) && $util.isInteger(message.block.high)))
                        return "block: integer|Long expected";
                if (message.hash != null && message.hasOwnProperty("hash"))
                    if (!(message.hash && typeof message.hash.length === "number" || $util.isString(message.hash)))
                        return "hash: buffer expected";
                if (message.name != null && message.hasOwnProperty("name"))
                    if (!$util.isString(message.name))
                        return "name: string expected";
                if (message.path != null && message.hasOwnProperty("path"))
                    if (!$util.isString(message.path))
                        return "path: string expected";
                return null;
            };
    
            /**
             * Creates a DataStream message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.DataStream
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.DataStream} DataStream
             */
            DataStream.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.DataStream)
                    return object;
                var message = new $root.fk_app.DataStream();
                if (object.id != null)
                    message.id = object.id >>> 0;
                if (object.time != null)
                    if ($util.Long)
                        (message.time = $util.Long.fromValue(object.time)).unsigned = true;
                    else if (typeof object.time === "string")
                        message.time = parseInt(object.time, 10);
                    else if (typeof object.time === "number")
                        message.time = object.time;
                    else if (typeof object.time === "object")
                        message.time = new $util.LongBits(object.time.low >>> 0, object.time.high >>> 0).toNumber(true);
                if (object.size != null)
                    if ($util.Long)
                        (message.size = $util.Long.fromValue(object.size)).unsigned = true;
                    else if (typeof object.size === "string")
                        message.size = parseInt(object.size, 10);
                    else if (typeof object.size === "number")
                        message.size = object.size;
                    else if (typeof object.size === "object")
                        message.size = new $util.LongBits(object.size.low >>> 0, object.size.high >>> 0).toNumber(true);
                if (object.version != null)
                    message.version = object.version >>> 0;
                if (object.block != null)
                    if ($util.Long)
                        (message.block = $util.Long.fromValue(object.block)).unsigned = true;
                    else if (typeof object.block === "string")
                        message.block = parseInt(object.block, 10);
                    else if (typeof object.block === "number")
                        message.block = object.block;
                    else if (typeof object.block === "object")
                        message.block = new $util.LongBits(object.block.low >>> 0, object.block.high >>> 0).toNumber(true);
                if (object.hash != null)
                    if (typeof object.hash === "string")
                        $util.base64.decode(object.hash, message.hash = $util.newBuffer($util.base64.length(object.hash)), 0);
                    else if (object.hash.length)
                        message.hash = object.hash;
                if (object.name != null)
                    message.name = String(object.name);
                if (object.path != null)
                    message.path = String(object.path);
                return message;
            };
    
            /**
             * Creates a plain object from a DataStream message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.DataStream
             * @static
             * @param {fk_app.DataStream} message DataStream
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            DataStream.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.id = 0;
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.time = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.time = options.longs === String ? "0" : 0;
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.size = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.size = options.longs === String ? "0" : 0;
                    object.version = 0;
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.block = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.block = options.longs === String ? "0" : 0;
                    object.hash = options.bytes === String ? "" : [];
                    object.name = "";
                    object.path = "";
                }
                if (message.id != null && message.hasOwnProperty("id"))
                    object.id = message.id;
                if (message.time != null && message.hasOwnProperty("time"))
                    if (typeof message.time === "number")
                        object.time = options.longs === String ? String(message.time) : message.time;
                    else
                        object.time = options.longs === String ? $util.Long.prototype.toString.call(message.time) : options.longs === Number ? new $util.LongBits(message.time.low >>> 0, message.time.high >>> 0).toNumber(true) : message.time;
                if (message.size != null && message.hasOwnProperty("size"))
                    if (typeof message.size === "number")
                        object.size = options.longs === String ? String(message.size) : message.size;
                    else
                        object.size = options.longs === String ? $util.Long.prototype.toString.call(message.size) : options.longs === Number ? new $util.LongBits(message.size.low >>> 0, message.size.high >>> 0).toNumber(true) : message.size;
                if (message.version != null && message.hasOwnProperty("version"))
                    object.version = message.version;
                if (message.block != null && message.hasOwnProperty("block"))
                    if (typeof message.block === "number")
                        object.block = options.longs === String ? String(message.block) : message.block;
                    else
                        object.block = options.longs === String ? $util.Long.prototype.toString.call(message.block) : options.longs === Number ? new $util.LongBits(message.block.low >>> 0, message.block.high >>> 0).toNumber(true) : message.block;
                if (message.hash != null && message.hasOwnProperty("hash"))
                    object.hash = options.bytes === String ? $util.base64.encode(message.hash, 0, message.hash.length) : options.bytes === Array ? Array.prototype.slice.call(message.hash) : message.hash;
                if (message.name != null && message.hasOwnProperty("name"))
                    object.name = message.name;
                if (message.path != null && message.hasOwnProperty("path"))
                    object.path = message.path;
                return object;
            };
    
            /**
             * Converts this DataStream to JSON.
             * @function toJSON
             * @memberof fk_app.DataStream
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            DataStream.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return DataStream;
        })();
    
        fk_app.LiveSensorReading = (function() {
    
            /**
             * Properties of a LiveSensorReading.
             * @memberof fk_app
             * @interface ILiveSensorReading
             * @property {fk_app.ISensorCapabilities} [sensor] LiveSensorReading sensor
             * @property {number} [value] LiveSensorReading value
             * @property {number} [uncalibrated] LiveSensorReading uncalibrated
             * @property {number} [factory] LiveSensorReading factory
             * @property {Uint8Array} [adc] LiveSensorReading adc
             */
    
            /**
             * Constructs a new LiveSensorReading.
             * @memberof fk_app
             * @classdesc Represents a LiveSensorReading.
             * @constructor
             * @param {fk_app.ILiveSensorReading=} [properties] Properties to set
             */
            function LiveSensorReading(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * LiveSensorReading sensor.
             * @member {(fk_app.ISensorCapabilities|null|undefined)}sensor
             * @memberof fk_app.LiveSensorReading
             * @instance
             */
            LiveSensorReading.prototype.sensor = null;
    
            /**
             * LiveSensorReading value.
             * @member {number}value
             * @memberof fk_app.LiveSensorReading
             * @instance
             */
            LiveSensorReading.prototype.value = 0;
    
            /**
             * LiveSensorReading uncalibrated.
             * @member {number}uncalibrated
             * @memberof fk_app.LiveSensorReading
             * @instance
             */
            LiveSensorReading.prototype.uncalibrated = 0;
    
            /**
             * LiveSensorReading factory.
             * @member {number}factory
             * @memberof fk_app.LiveSensorReading
             * @instance
             */
            LiveSensorReading.prototype.factory = 0;
    
            /**
             * LiveSensorReading adc.
             * @member {Uint8Array}adc
             * @memberof fk_app.LiveSensorReading
             * @instance
             */
            LiveSensorReading.prototype.adc = $util.newBuffer([]);
    
            /**
             * Creates a new LiveSensorReading instance using the specified properties.
             * @function create
             * @memberof fk_app.LiveSensorReading
             * @static
             * @param {fk_app.ILiveSensorReading=} [properties] Properties to set
             * @returns {fk_app.LiveSensorReading} LiveSensorReading instance
             */
            LiveSensorReading.create = function create(properties) {
                return new LiveSensorReading(properties);
            };
    
            /**
             * Encodes the specified LiveSensorReading message. Does not implicitly {@link fk_app.LiveSensorReading.verify|verify} messages.
             * @function encode
             * @memberof fk_app.LiveSensorReading
             * @static
             * @param {fk_app.ILiveSensorReading} message LiveSensorReading message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            LiveSensorReading.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.sensor != null && message.hasOwnProperty("sensor"))
                    $root.fk_app.SensorCapabilities.encode(message.sensor, writer.uint32(/* id 1, wireType 2 =*/10).fork()).ldelim();
                if (message.value != null && message.hasOwnProperty("value"))
                    writer.uint32(/* id 2, wireType 5 =*/21).float(message.value);
                if (message.uncalibrated != null && message.hasOwnProperty("uncalibrated"))
                    writer.uint32(/* id 3, wireType 5 =*/29).float(message.uncalibrated);
                if (message.factory != null && message.hasOwnProperty("factory"))
                    writer.uint32(/* id 4, wireType 5 =*/37).float(message.factory);
                if (message.adc != null && message.hasOwnProperty("adc"))
                    writer.uint32(/* id 5, wireType 2 =*/42).bytes(message.adc);
                return writer;
            };
    
            /**
             * Encodes the specified LiveSensorReading message, length delimited. Does not implicitly {@link fk_app.LiveSensorReading.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.LiveSensorReading
             * @static
             * @param {fk_app.ILiveSensorReading} message LiveSensorReading message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            LiveSensorReading.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a LiveSensorReading message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.LiveSensorReading
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.LiveSensorReading} LiveSensorReading
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            LiveSensorReading.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.LiveSensorReading();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.sensor = $root.fk_app.SensorCapabilities.decode(reader, reader.uint32());
                        break;
                    case 2:
                        message.value = reader.float();
                        break;
                    case 3:
                        message.uncalibrated = reader.float();
                        break;
                    case 4:
                        message.factory = reader.float();
                        break;
                    case 5:
                        message.adc = reader.bytes();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a LiveSensorReading message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.LiveSensorReading
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.LiveSensorReading} LiveSensorReading
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            LiveSensorReading.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a LiveSensorReading message.
             * @function verify
             * @memberof fk_app.LiveSensorReading
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            LiveSensorReading.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.sensor != null && message.hasOwnProperty("sensor")) {
                    var error = $root.fk_app.SensorCapabilities.verify(message.sensor);
                    if (error)
                        return "sensor." + error;
                }
                if (message.value != null && message.hasOwnProperty("value"))
                    if (typeof message.value !== "number")
                        return "value: number expected";
                if (message.uncalibrated != null && message.hasOwnProperty("uncalibrated"))
                    if (typeof message.uncalibrated !== "number")
                        return "uncalibrated: number expected";
                if (message.factory != null && message.hasOwnProperty("factory"))
                    if (typeof message.factory !== "number")
                        return "factory: number expected";
                if (message.adc != null && message.hasOwnProperty("adc"))
                    if (!(message.adc && typeof message.adc.length === "number" || $util.isString(message.adc)))
                        return "adc: buffer expected";
                return null;
            };
    
            /**
             * Creates a LiveSensorReading message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.LiveSensorReading
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.LiveSensorReading} LiveSensorReading
             */
            LiveSensorReading.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.LiveSensorReading)
                    return object;
                var message = new $root.fk_app.LiveSensorReading();
                if (object.sensor != null) {
                    if (typeof object.sensor !== "object")
                        throw TypeError(".fk_app.LiveSensorReading.sensor: object expected");
                    message.sensor = $root.fk_app.SensorCapabilities.fromObject(object.sensor);
                }
                if (object.value != null)
                    message.value = Number(object.value);
                if (object.uncalibrated != null)
                    message.uncalibrated = Number(object.uncalibrated);
                if (object.factory != null)
                    message.factory = Number(object.factory);
                if (object.adc != null)
                    if (typeof object.adc === "string")
                        $util.base64.decode(object.adc, message.adc = $util.newBuffer($util.base64.length(object.adc)), 0);
                    else if (object.adc.length)
                        message.adc = object.adc;
                return message;
            };
    
            /**
             * Creates a plain object from a LiveSensorReading message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.LiveSensorReading
             * @static
             * @param {fk_app.LiveSensorReading} message LiveSensorReading
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            LiveSensorReading.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.sensor = null;
                    object.value = 0;
                    object.uncalibrated = 0;
                    object.factory = 0;
                    object.adc = options.bytes === String ? "" : [];
                }
                if (message.sensor != null && message.hasOwnProperty("sensor"))
                    object.sensor = $root.fk_app.SensorCapabilities.toObject(message.sensor, options);
                if (message.value != null && message.hasOwnProperty("value"))
                    object.value = options.json && !isFinite(message.value) ? String(message.value) : message.value;
                if (message.uncalibrated != null && message.hasOwnProperty("uncalibrated"))
                    object.uncalibrated = options.json && !isFinite(message.uncalibrated) ? String(message.uncalibrated) : message.uncalibrated;
                if (message.factory != null && message.hasOwnProperty("factory"))
                    object.factory = options.json && !isFinite(message.factory) ? String(message.factory) : message.factory;
                if (message.adc != null && message.hasOwnProperty("adc"))
                    object.adc = options.bytes === String ? $util.base64.encode(message.adc, 0, message.adc.length) : options.bytes === Array ? Array.prototype.slice.call(message.adc) : message.adc;
                return object;
            };
    
            /**
             * Converts this LiveSensorReading to JSON.
             * @function toJSON
             * @memberof fk_app.LiveSensorReading
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            LiveSensorReading.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return LiveSensorReading;
        })();
    
        fk_app.LiveModuleReadings = (function() {
    
            /**
             * Properties of a LiveModuleReadings.
             * @memberof fk_app
             * @interface ILiveModuleReadings
             * @property {fk_app.IModuleCapabilities} [module] LiveModuleReadings module
             * @property {Array.<fk_app.ILiveSensorReading>} [readings] LiveModuleReadings readings
             */
    
            /**
             * Constructs a new LiveModuleReadings.
             * @memberof fk_app
             * @classdesc Represents a LiveModuleReadings.
             * @constructor
             * @param {fk_app.ILiveModuleReadings=} [properties] Properties to set
             */
            function LiveModuleReadings(properties) {
                this.readings = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * LiveModuleReadings module.
             * @member {(fk_app.IModuleCapabilities|null|undefined)}module
             * @memberof fk_app.LiveModuleReadings
             * @instance
             */
            LiveModuleReadings.prototype.module = null;
    
            /**
             * LiveModuleReadings readings.
             * @member {Array.<fk_app.ILiveSensorReading>}readings
             * @memberof fk_app.LiveModuleReadings
             * @instance
             */
            LiveModuleReadings.prototype.readings = $util.emptyArray;
    
            /**
             * Creates a new LiveModuleReadings instance using the specified properties.
             * @function create
             * @memberof fk_app.LiveModuleReadings
             * @static
             * @param {fk_app.ILiveModuleReadings=} [properties] Properties to set
             * @returns {fk_app.LiveModuleReadings} LiveModuleReadings instance
             */
            LiveModuleReadings.create = function create(properties) {
                return new LiveModuleReadings(properties);
            };
    
            /**
             * Encodes the specified LiveModuleReadings message. Does not implicitly {@link fk_app.LiveModuleReadings.verify|verify} messages.
             * @function encode
             * @memberof fk_app.LiveModuleReadings
             * @static
             * @param {fk_app.ILiveModuleReadings} message LiveModuleReadings message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            LiveModuleReadings.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.module != null && message.hasOwnProperty("module"))
                    $root.fk_app.ModuleCapabilities.encode(message.module, writer.uint32(/* id 1, wireType 2 =*/10).fork()).ldelim();
                if (message.readings != null && message.readings.length)
                    for (var i = 0; i < message.readings.length; ++i)
                        $root.fk_app.LiveSensorReading.encode(message.readings[i], writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
                return writer;
            };
    
            /**
             * Encodes the specified LiveModuleReadings message, length delimited. Does not implicitly {@link fk_app.LiveModuleReadings.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.LiveModuleReadings
             * @static
             * @param {fk_app.ILiveModuleReadings} message LiveModuleReadings message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            LiveModuleReadings.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a LiveModuleReadings message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.LiveModuleReadings
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.LiveModuleReadings} LiveModuleReadings
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            LiveModuleReadings.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.LiveModuleReadings();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.module = $root.fk_app.ModuleCapabilities.decode(reader, reader.uint32());
                        break;
                    case 2:
                        if (!(message.readings && message.readings.length))
                            message.readings = [];
                        message.readings.push($root.fk_app.LiveSensorReading.decode(reader, reader.uint32()));
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a LiveModuleReadings message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.LiveModuleReadings
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.LiveModuleReadings} LiveModuleReadings
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            LiveModuleReadings.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a LiveModuleReadings message.
             * @function verify
             * @memberof fk_app.LiveModuleReadings
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            LiveModuleReadings.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.module != null && message.hasOwnProperty("module")) {
                    var error = $root.fk_app.ModuleCapabilities.verify(message.module);
                    if (error)
                        return "module." + error;
                }
                if (message.readings != null && message.hasOwnProperty("readings")) {
                    if (!Array.isArray(message.readings))
                        return "readings: array expected";
                    for (var i = 0; i < message.readings.length; ++i) {
                        error = $root.fk_app.LiveSensorReading.verify(message.readings[i]);
                        if (error)
                            return "readings." + error;
                    }
                }
                return null;
            };
    
            /**
             * Creates a LiveModuleReadings message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.LiveModuleReadings
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.LiveModuleReadings} LiveModuleReadings
             */
            LiveModuleReadings.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.LiveModuleReadings)
                    return object;
                var message = new $root.fk_app.LiveModuleReadings();
                if (object.module != null) {
                    if (typeof object.module !== "object")
                        throw TypeError(".fk_app.LiveModuleReadings.module: object expected");
                    message.module = $root.fk_app.ModuleCapabilities.fromObject(object.module);
                }
                if (object.readings) {
                    if (!Array.isArray(object.readings))
                        throw TypeError(".fk_app.LiveModuleReadings.readings: array expected");
                    message.readings = [];
                    for (var i = 0; i < object.readings.length; ++i) {
                        if (typeof object.readings[i] !== "object")
                            throw TypeError(".fk_app.LiveModuleReadings.readings: object expected");
                        message.readings[i] = $root.fk_app.LiveSensorReading.fromObject(object.readings[i]);
                    }
                }
                return message;
            };
    
            /**
             * Creates a plain object from a LiveModuleReadings message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.LiveModuleReadings
             * @static
             * @param {fk_app.LiveModuleReadings} message LiveModuleReadings
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            LiveModuleReadings.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults)
                    object.readings = [];
                if (options.defaults)
                    object.module = null;
                if (message.module != null && message.hasOwnProperty("module"))
                    object.module = $root.fk_app.ModuleCapabilities.toObject(message.module, options);
                if (message.readings && message.readings.length) {
                    object.readings = [];
                    for (var j = 0; j < message.readings.length; ++j)
                        object.readings[j] = $root.fk_app.LiveSensorReading.toObject(message.readings[j], options);
                }
                return object;
            };
    
            /**
             * Converts this LiveModuleReadings to JSON.
             * @function toJSON
             * @memberof fk_app.LiveModuleReadings
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            LiveModuleReadings.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return LiveModuleReadings;
        })();
    
        fk_app.LiveReadings = (function() {
    
            /**
             * Properties of a LiveReadings.
             * @memberof fk_app
             * @interface ILiveReadings
             * @property {number|Long} [time] LiveReadings time
             * @property {Array.<fk_app.ILiveModuleReadings>} [modules] LiveReadings modules
             */
    
            /**
             * Constructs a new LiveReadings.
             * @memberof fk_app
             * @classdesc Represents a LiveReadings.
             * @constructor
             * @param {fk_app.ILiveReadings=} [properties] Properties to set
             */
            function LiveReadings(properties) {
                this.modules = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * LiveReadings time.
             * @member {number|Long}time
             * @memberof fk_app.LiveReadings
             * @instance
             */
            LiveReadings.prototype.time = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * LiveReadings modules.
             * @member {Array.<fk_app.ILiveModuleReadings>}modules
             * @memberof fk_app.LiveReadings
             * @instance
             */
            LiveReadings.prototype.modules = $util.emptyArray;
    
            /**
             * Creates a new LiveReadings instance using the specified properties.
             * @function create
             * @memberof fk_app.LiveReadings
             * @static
             * @param {fk_app.ILiveReadings=} [properties] Properties to set
             * @returns {fk_app.LiveReadings} LiveReadings instance
             */
            LiveReadings.create = function create(properties) {
                return new LiveReadings(properties);
            };
    
            /**
             * Encodes the specified LiveReadings message. Does not implicitly {@link fk_app.LiveReadings.verify|verify} messages.
             * @function encode
             * @memberof fk_app.LiveReadings
             * @static
             * @param {fk_app.ILiveReadings} message LiveReadings message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            LiveReadings.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.time != null && message.hasOwnProperty("time"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint64(message.time);
                if (message.modules != null && message.modules.length)
                    for (var i = 0; i < message.modules.length; ++i)
                        $root.fk_app.LiveModuleReadings.encode(message.modules[i], writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
                return writer;
            };
    
            /**
             * Encodes the specified LiveReadings message, length delimited. Does not implicitly {@link fk_app.LiveReadings.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.LiveReadings
             * @static
             * @param {fk_app.ILiveReadings} message LiveReadings message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            LiveReadings.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a LiveReadings message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.LiveReadings
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.LiveReadings} LiveReadings
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            LiveReadings.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.LiveReadings();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.time = reader.uint64();
                        break;
                    case 2:
                        if (!(message.modules && message.modules.length))
                            message.modules = [];
                        message.modules.push($root.fk_app.LiveModuleReadings.decode(reader, reader.uint32()));
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a LiveReadings message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.LiveReadings
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.LiveReadings} LiveReadings
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            LiveReadings.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a LiveReadings message.
             * @function verify
             * @memberof fk_app.LiveReadings
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            LiveReadings.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.time != null && message.hasOwnProperty("time"))
                    if (!$util.isInteger(message.time) && !(message.time && $util.isInteger(message.time.low) && $util.isInteger(message.time.high)))
                        return "time: integer|Long expected";
                if (message.modules != null && message.hasOwnProperty("modules")) {
                    if (!Array.isArray(message.modules))
                        return "modules: array expected";
                    for (var i = 0; i < message.modules.length; ++i) {
                        var error = $root.fk_app.LiveModuleReadings.verify(message.modules[i]);
                        if (error)
                            return "modules." + error;
                    }
                }
                return null;
            };
    
            /**
             * Creates a LiveReadings message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.LiveReadings
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.LiveReadings} LiveReadings
             */
            LiveReadings.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.LiveReadings)
                    return object;
                var message = new $root.fk_app.LiveReadings();
                if (object.time != null)
                    if ($util.Long)
                        (message.time = $util.Long.fromValue(object.time)).unsigned = true;
                    else if (typeof object.time === "string")
                        message.time = parseInt(object.time, 10);
                    else if (typeof object.time === "number")
                        message.time = object.time;
                    else if (typeof object.time === "object")
                        message.time = new $util.LongBits(object.time.low >>> 0, object.time.high >>> 0).toNumber(true);
                if (object.modules) {
                    if (!Array.isArray(object.modules))
                        throw TypeError(".fk_app.LiveReadings.modules: array expected");
                    message.modules = [];
                    for (var i = 0; i < object.modules.length; ++i) {
                        if (typeof object.modules[i] !== "object")
                            throw TypeError(".fk_app.LiveReadings.modules: object expected");
                        message.modules[i] = $root.fk_app.LiveModuleReadings.fromObject(object.modules[i]);
                    }
                }
                return message;
            };
    
            /**
             * Creates a plain object from a LiveReadings message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.LiveReadings
             * @static
             * @param {fk_app.LiveReadings} message LiveReadings
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            LiveReadings.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults)
                    object.modules = [];
                if (options.defaults)
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.time = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.time = options.longs === String ? "0" : 0;
                if (message.time != null && message.hasOwnProperty("time"))
                    if (typeof message.time === "number")
                        object.time = options.longs === String ? String(message.time) : message.time;
                    else
                        object.time = options.longs === String ? $util.Long.prototype.toString.call(message.time) : options.longs === Number ? new $util.LongBits(message.time.low >>> 0, message.time.high >>> 0).toNumber(true) : message.time;
                if (message.modules && message.modules.length) {
                    object.modules = [];
                    for (var j = 0; j < message.modules.length; ++j)
                        object.modules[j] = $root.fk_app.LiveModuleReadings.toObject(message.modules[j], options);
                }
                return object;
            };
    
            /**
             * Converts this LiveReadings to JSON.
             * @function toJSON
             * @memberof fk_app.LiveReadings
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            LiveReadings.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return LiveReadings;
        })();
    
        fk_app.DirectoryEntry = (function() {
    
            /**
             * Properties of a DirectoryEntry.
             * @memberof fk_app
             * @interface IDirectoryEntry
             * @property {string} [name] DirectoryEntry name
             * @property {string} [path] DirectoryEntry path
             * @property {number} [size] DirectoryEntry size
             * @property {boolean} [directory] DirectoryEntry directory
             */
    
            /**
             * Constructs a new DirectoryEntry.
             * @memberof fk_app
             * @classdesc Represents a DirectoryEntry.
             * @constructor
             * @param {fk_app.IDirectoryEntry=} [properties] Properties to set
             */
            function DirectoryEntry(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * DirectoryEntry name.
             * @member {string}name
             * @memberof fk_app.DirectoryEntry
             * @instance
             */
            DirectoryEntry.prototype.name = "";
    
            /**
             * DirectoryEntry path.
             * @member {string}path
             * @memberof fk_app.DirectoryEntry
             * @instance
             */
            DirectoryEntry.prototype.path = "";
    
            /**
             * DirectoryEntry size.
             * @member {number}size
             * @memberof fk_app.DirectoryEntry
             * @instance
             */
            DirectoryEntry.prototype.size = 0;
    
            /**
             * DirectoryEntry directory.
             * @member {boolean}directory
             * @memberof fk_app.DirectoryEntry
             * @instance
             */
            DirectoryEntry.prototype.directory = false;
    
            /**
             * Creates a new DirectoryEntry instance using the specified properties.
             * @function create
             * @memberof fk_app.DirectoryEntry
             * @static
             * @param {fk_app.IDirectoryEntry=} [properties] Properties to set
             * @returns {fk_app.DirectoryEntry} DirectoryEntry instance
             */
            DirectoryEntry.create = function create(properties) {
                return new DirectoryEntry(properties);
            };
    
            /**
             * Encodes the specified DirectoryEntry message. Does not implicitly {@link fk_app.DirectoryEntry.verify|verify} messages.
             * @function encode
             * @memberof fk_app.DirectoryEntry
             * @static
             * @param {fk_app.IDirectoryEntry} message DirectoryEntry message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            DirectoryEntry.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.name != null && message.hasOwnProperty("name"))
                    writer.uint32(/* id 1, wireType 2 =*/10).string(message.name);
                if (message.path != null && message.hasOwnProperty("path"))
                    writer.uint32(/* id 2, wireType 2 =*/18).string(message.path);
                if (message.size != null && message.hasOwnProperty("size"))
                    writer.uint32(/* id 3, wireType 0 =*/24).uint32(message.size);
                if (message.directory != null && message.hasOwnProperty("directory"))
                    writer.uint32(/* id 4, wireType 0 =*/32).bool(message.directory);
                return writer;
            };
    
            /**
             * Encodes the specified DirectoryEntry message, length delimited. Does not implicitly {@link fk_app.DirectoryEntry.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.DirectoryEntry
             * @static
             * @param {fk_app.IDirectoryEntry} message DirectoryEntry message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            DirectoryEntry.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a DirectoryEntry message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.DirectoryEntry
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.DirectoryEntry} DirectoryEntry
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            DirectoryEntry.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.DirectoryEntry();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.name = reader.string();
                        break;
                    case 2:
                        message.path = reader.string();
                        break;
                    case 3:
                        message.size = reader.uint32();
                        break;
                    case 4:
                        message.directory = reader.bool();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a DirectoryEntry message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.DirectoryEntry
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.DirectoryEntry} DirectoryEntry
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            DirectoryEntry.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a DirectoryEntry message.
             * @function verify
             * @memberof fk_app.DirectoryEntry
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            DirectoryEntry.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.name != null && message.hasOwnProperty("name"))
                    if (!$util.isString(message.name))
                        return "name: string expected";
                if (message.path != null && message.hasOwnProperty("path"))
                    if (!$util.isString(message.path))
                        return "path: string expected";
                if (message.size != null && message.hasOwnProperty("size"))
                    if (!$util.isInteger(message.size))
                        return "size: integer expected";
                if (message.directory != null && message.hasOwnProperty("directory"))
                    if (typeof message.directory !== "boolean")
                        return "directory: boolean expected";
                return null;
            };
    
            /**
             * Creates a DirectoryEntry message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.DirectoryEntry
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.DirectoryEntry} DirectoryEntry
             */
            DirectoryEntry.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.DirectoryEntry)
                    return object;
                var message = new $root.fk_app.DirectoryEntry();
                if (object.name != null)
                    message.name = String(object.name);
                if (object.path != null)
                    message.path = String(object.path);
                if (object.size != null)
                    message.size = object.size >>> 0;
                if (object.directory != null)
                    message.directory = Boolean(object.directory);
                return message;
            };
    
            /**
             * Creates a plain object from a DirectoryEntry message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.DirectoryEntry
             * @static
             * @param {fk_app.DirectoryEntry} message DirectoryEntry
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            DirectoryEntry.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.name = "";
                    object.path = "";
                    object.size = 0;
                    object.directory = false;
                }
                if (message.name != null && message.hasOwnProperty("name"))
                    object.name = message.name;
                if (message.path != null && message.hasOwnProperty("path"))
                    object.path = message.path;
                if (message.size != null && message.hasOwnProperty("size"))
                    object.size = message.size;
                if (message.directory != null && message.hasOwnProperty("directory"))
                    object.directory = message.directory;
                return object;
            };
    
            /**
             * Converts this DirectoryEntry to JSON.
             * @function toJSON
             * @memberof fk_app.DirectoryEntry
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            DirectoryEntry.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return DirectoryEntry;
        })();
    
        fk_app.DirectoryListing = (function() {
    
            /**
             * Properties of a DirectoryListing.
             * @memberof fk_app
             * @interface IDirectoryListing
             * @property {string} [path] DirectoryListing path
             * @property {number} [totalEntries] DirectoryListing totalEntries
             * @property {Array.<fk_app.IDirectoryEntry>} [entries] DirectoryListing entries
             */
    
            /**
             * Constructs a new DirectoryListing.
             * @memberof fk_app
             * @classdesc Represents a DirectoryListing.
             * @constructor
             * @param {fk_app.IDirectoryListing=} [properties] Properties to set
             */
            function DirectoryListing(properties) {
                this.entries = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * DirectoryListing path.
             * @member {string}path
             * @memberof fk_app.DirectoryListing
             * @instance
             */
            DirectoryListing.prototype.path = "";
    
            /**
             * DirectoryListing totalEntries.
             * @member {number}totalEntries
             * @memberof fk_app.DirectoryListing
             * @instance
             */
            DirectoryListing.prototype.totalEntries = 0;
    
            /**
             * DirectoryListing entries.
             * @member {Array.<fk_app.IDirectoryEntry>}entries
             * @memberof fk_app.DirectoryListing
             * @instance
             */
            DirectoryListing.prototype.entries = $util.emptyArray;
    
            /**
             * Creates a new DirectoryListing instance using the specified properties.
             * @function create
             * @memberof fk_app.DirectoryListing
             * @static
             * @param {fk_app.IDirectoryListing=} [properties] Properties to set
             * @returns {fk_app.DirectoryListing} DirectoryListing instance
             */
            DirectoryListing.create = function create(properties) {
                return new DirectoryListing(properties);
            };
    
            /**
             * Encodes the specified DirectoryListing message. Does not implicitly {@link fk_app.DirectoryListing.verify|verify} messages.
             * @function encode
             * @memberof fk_app.DirectoryListing
             * @static
             * @param {fk_app.IDirectoryListing} message DirectoryListing message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            DirectoryListing.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.path != null && message.hasOwnProperty("path"))
                    writer.uint32(/* id 1, wireType 2 =*/10).string(message.path);
                if (message.entries != null && message.entries.length)
                    for (var i = 0; i < message.entries.length; ++i)
                        $root.fk_app.DirectoryEntry.encode(message.entries[i], writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
                if (message.totalEntries != null && message.hasOwnProperty("totalEntries"))
                    writer.uint32(/* id 3, wireType 0 =*/24).uint32(message.totalEntries);
                return writer;
            };
    
            /**
             * Encodes the specified DirectoryListing message, length delimited. Does not implicitly {@link fk_app.DirectoryListing.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.DirectoryListing
             * @static
             * @param {fk_app.IDirectoryListing} message DirectoryListing message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            DirectoryListing.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a DirectoryListing message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.DirectoryListing
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.DirectoryListing} DirectoryListing
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            DirectoryListing.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.DirectoryListing();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.path = reader.string();
                        break;
                    case 3:
                        message.totalEntries = reader.uint32();
                        break;
                    case 2:
                        if (!(message.entries && message.entries.length))
                            message.entries = [];
                        message.entries.push($root.fk_app.DirectoryEntry.decode(reader, reader.uint32()));
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a DirectoryListing message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.DirectoryListing
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.DirectoryListing} DirectoryListing
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            DirectoryListing.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a DirectoryListing message.
             * @function verify
             * @memberof fk_app.DirectoryListing
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            DirectoryListing.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.path != null && message.hasOwnProperty("path"))
                    if (!$util.isString(message.path))
                        return "path: string expected";
                if (message.totalEntries != null && message.hasOwnProperty("totalEntries"))
                    if (!$util.isInteger(message.totalEntries))
                        return "totalEntries: integer expected";
                if (message.entries != null && message.hasOwnProperty("entries")) {
                    if (!Array.isArray(message.entries))
                        return "entries: array expected";
                    for (var i = 0; i < message.entries.length; ++i) {
                        var error = $root.fk_app.DirectoryEntry.verify(message.entries[i]);
                        if (error)
                            return "entries." + error;
                    }
                }
                return null;
            };
    
            /**
             * Creates a DirectoryListing message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.DirectoryListing
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.DirectoryListing} DirectoryListing
             */
            DirectoryListing.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.DirectoryListing)
                    return object;
                var message = new $root.fk_app.DirectoryListing();
                if (object.path != null)
                    message.path = String(object.path);
                if (object.totalEntries != null)
                    message.totalEntries = object.totalEntries >>> 0;
                if (object.entries) {
                    if (!Array.isArray(object.entries))
                        throw TypeError(".fk_app.DirectoryListing.entries: array expected");
                    message.entries = [];
                    for (var i = 0; i < object.entries.length; ++i) {
                        if (typeof object.entries[i] !== "object")
                            throw TypeError(".fk_app.DirectoryListing.entries: object expected");
                        message.entries[i] = $root.fk_app.DirectoryEntry.fromObject(object.entries[i]);
                    }
                }
                return message;
            };
    
            /**
             * Creates a plain object from a DirectoryListing message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.DirectoryListing
             * @static
             * @param {fk_app.DirectoryListing} message DirectoryListing
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            DirectoryListing.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults)
                    object.entries = [];
                if (options.defaults) {
                    object.path = "";
                    object.totalEntries = 0;
                }
                if (message.path != null && message.hasOwnProperty("path"))
                    object.path = message.path;
                if (message.entries && message.entries.length) {
                    object.entries = [];
                    for (var j = 0; j < message.entries.length; ++j)
                        object.entries[j] = $root.fk_app.DirectoryEntry.toObject(message.entries[j], options);
                }
                if (message.totalEntries != null && message.hasOwnProperty("totalEntries"))
                    object.totalEntries = message.totalEntries;
                return object;
            };
    
            /**
             * Converts this DirectoryListing to JSON.
             * @function toJSON
             * @memberof fk_app.DirectoryListing
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            DirectoryListing.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return DirectoryListing;
        })();
    
        fk_app.NearbyNetwork = (function() {
    
            /**
             * Properties of a NearbyNetwork.
             * @memberof fk_app
             * @interface INearbyNetwork
             * @property {string} [ssid] NearbyNetwork ssid
             */
    
            /**
             * Constructs a new NearbyNetwork.
             * @memberof fk_app
             * @classdesc Represents a NearbyNetwork.
             * @constructor
             * @param {fk_app.INearbyNetwork=} [properties] Properties to set
             */
            function NearbyNetwork(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * NearbyNetwork ssid.
             * @member {string}ssid
             * @memberof fk_app.NearbyNetwork
             * @instance
             */
            NearbyNetwork.prototype.ssid = "";
    
            /**
             * Creates a new NearbyNetwork instance using the specified properties.
             * @function create
             * @memberof fk_app.NearbyNetwork
             * @static
             * @param {fk_app.INearbyNetwork=} [properties] Properties to set
             * @returns {fk_app.NearbyNetwork} NearbyNetwork instance
             */
            NearbyNetwork.create = function create(properties) {
                return new NearbyNetwork(properties);
            };
    
            /**
             * Encodes the specified NearbyNetwork message. Does not implicitly {@link fk_app.NearbyNetwork.verify|verify} messages.
             * @function encode
             * @memberof fk_app.NearbyNetwork
             * @static
             * @param {fk_app.INearbyNetwork} message NearbyNetwork message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            NearbyNetwork.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.ssid != null && message.hasOwnProperty("ssid"))
                    writer.uint32(/* id 1, wireType 2 =*/10).string(message.ssid);
                return writer;
            };
    
            /**
             * Encodes the specified NearbyNetwork message, length delimited. Does not implicitly {@link fk_app.NearbyNetwork.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.NearbyNetwork
             * @static
             * @param {fk_app.INearbyNetwork} message NearbyNetwork message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            NearbyNetwork.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a NearbyNetwork message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.NearbyNetwork
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.NearbyNetwork} NearbyNetwork
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            NearbyNetwork.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.NearbyNetwork();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.ssid = reader.string();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a NearbyNetwork message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.NearbyNetwork
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.NearbyNetwork} NearbyNetwork
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            NearbyNetwork.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a NearbyNetwork message.
             * @function verify
             * @memberof fk_app.NearbyNetwork
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            NearbyNetwork.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.ssid != null && message.hasOwnProperty("ssid"))
                    if (!$util.isString(message.ssid))
                        return "ssid: string expected";
                return null;
            };
    
            /**
             * Creates a NearbyNetwork message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.NearbyNetwork
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.NearbyNetwork} NearbyNetwork
             */
            NearbyNetwork.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.NearbyNetwork)
                    return object;
                var message = new $root.fk_app.NearbyNetwork();
                if (object.ssid != null)
                    message.ssid = String(object.ssid);
                return message;
            };
    
            /**
             * Creates a plain object from a NearbyNetwork message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.NearbyNetwork
             * @static
             * @param {fk_app.NearbyNetwork} message NearbyNetwork
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            NearbyNetwork.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults)
                    object.ssid = "";
                if (message.ssid != null && message.hasOwnProperty("ssid"))
                    object.ssid = message.ssid;
                return object;
            };
    
            /**
             * Converts this NearbyNetwork to JSON.
             * @function toJSON
             * @memberof fk_app.NearbyNetwork
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            NearbyNetwork.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return NearbyNetwork;
        })();
    
        fk_app.NearbyNetworks = (function() {
    
            /**
             * Properties of a NearbyNetworks.
             * @memberof fk_app
             * @interface INearbyNetworks
             * @property {Array.<fk_app.INearbyNetwork>} [networks] NearbyNetworks networks
             */
    
            /**
             * Constructs a new NearbyNetworks.
             * @memberof fk_app
             * @classdesc Represents a NearbyNetworks.
             * @constructor
             * @param {fk_app.INearbyNetworks=} [properties] Properties to set
             */
            function NearbyNetworks(properties) {
                this.networks = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * NearbyNetworks networks.
             * @member {Array.<fk_app.INearbyNetwork>}networks
             * @memberof fk_app.NearbyNetworks
             * @instance
             */
            NearbyNetworks.prototype.networks = $util.emptyArray;
    
            /**
             * Creates a new NearbyNetworks instance using the specified properties.
             * @function create
             * @memberof fk_app.NearbyNetworks
             * @static
             * @param {fk_app.INearbyNetworks=} [properties] Properties to set
             * @returns {fk_app.NearbyNetworks} NearbyNetworks instance
             */
            NearbyNetworks.create = function create(properties) {
                return new NearbyNetworks(properties);
            };
    
            /**
             * Encodes the specified NearbyNetworks message. Does not implicitly {@link fk_app.NearbyNetworks.verify|verify} messages.
             * @function encode
             * @memberof fk_app.NearbyNetworks
             * @static
             * @param {fk_app.INearbyNetworks} message NearbyNetworks message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            NearbyNetworks.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.networks != null && message.networks.length)
                    for (var i = 0; i < message.networks.length; ++i)
                        $root.fk_app.NearbyNetwork.encode(message.networks[i], writer.uint32(/* id 1, wireType 2 =*/10).fork()).ldelim();
                return writer;
            };
    
            /**
             * Encodes the specified NearbyNetworks message, length delimited. Does not implicitly {@link fk_app.NearbyNetworks.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.NearbyNetworks
             * @static
             * @param {fk_app.INearbyNetworks} message NearbyNetworks message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            NearbyNetworks.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a NearbyNetworks message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.NearbyNetworks
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.NearbyNetworks} NearbyNetworks
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            NearbyNetworks.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.NearbyNetworks();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        if (!(message.networks && message.networks.length))
                            message.networks = [];
                        message.networks.push($root.fk_app.NearbyNetwork.decode(reader, reader.uint32()));
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a NearbyNetworks message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.NearbyNetworks
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.NearbyNetworks} NearbyNetworks
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            NearbyNetworks.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a NearbyNetworks message.
             * @function verify
             * @memberof fk_app.NearbyNetworks
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            NearbyNetworks.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.networks != null && message.hasOwnProperty("networks")) {
                    if (!Array.isArray(message.networks))
                        return "networks: array expected";
                    for (var i = 0; i < message.networks.length; ++i) {
                        var error = $root.fk_app.NearbyNetwork.verify(message.networks[i]);
                        if (error)
                            return "networks." + error;
                    }
                }
                return null;
            };
    
            /**
             * Creates a NearbyNetworks message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.NearbyNetworks
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.NearbyNetworks} NearbyNetworks
             */
            NearbyNetworks.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.NearbyNetworks)
                    return object;
                var message = new $root.fk_app.NearbyNetworks();
                if (object.networks) {
                    if (!Array.isArray(object.networks))
                        throw TypeError(".fk_app.NearbyNetworks.networks: array expected");
                    message.networks = [];
                    for (var i = 0; i < object.networks.length; ++i) {
                        if (typeof object.networks[i] !== "object")
                            throw TypeError(".fk_app.NearbyNetworks.networks: object expected");
                        message.networks[i] = $root.fk_app.NearbyNetwork.fromObject(object.networks[i]);
                    }
                }
                return message;
            };
    
            /**
             * Creates a plain object from a NearbyNetworks message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.NearbyNetworks
             * @static
             * @param {fk_app.NearbyNetworks} message NearbyNetworks
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            NearbyNetworks.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults)
                    object.networks = [];
                if (message.networks && message.networks.length) {
                    object.networks = [];
                    for (var j = 0; j < message.networks.length; ++j)
                        object.networks[j] = $root.fk_app.NearbyNetwork.toObject(message.networks[j], options);
                }
                return object;
            };
    
            /**
             * Converts this NearbyNetworks to JSON.
             * @function toJSON
             * @memberof fk_app.NearbyNetworks
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            NearbyNetworks.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return NearbyNetworks;
        })();
    
        fk_app.Fault = (function() {
    
            /**
             * Properties of a Fault.
             * @memberof fk_app
             * @interface IFault
             * @property {number} [time] Fault time
             * @property {number} [code] Fault code
             * @property {string} [description] Fault description
             * @property {Uint8Array} [debug] Fault debug
             */
    
            /**
             * Constructs a new Fault.
             * @memberof fk_app
             * @classdesc Represents a Fault.
             * @constructor
             * @param {fk_app.IFault=} [properties] Properties to set
             */
            function Fault(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Fault time.
             * @member {number}time
             * @memberof fk_app.Fault
             * @instance
             */
            Fault.prototype.time = 0;
    
            /**
             * Fault code.
             * @member {number}code
             * @memberof fk_app.Fault
             * @instance
             */
            Fault.prototype.code = 0;
    
            /**
             * Fault description.
             * @member {string}description
             * @memberof fk_app.Fault
             * @instance
             */
            Fault.prototype.description = "";
    
            /**
             * Fault debug.
             * @member {Uint8Array}debug
             * @memberof fk_app.Fault
             * @instance
             */
            Fault.prototype.debug = $util.newBuffer([]);
    
            /**
             * Creates a new Fault instance using the specified properties.
             * @function create
             * @memberof fk_app.Fault
             * @static
             * @param {fk_app.IFault=} [properties] Properties to set
             * @returns {fk_app.Fault} Fault instance
             */
            Fault.create = function create(properties) {
                return new Fault(properties);
            };
    
            /**
             * Encodes the specified Fault message. Does not implicitly {@link fk_app.Fault.verify|verify} messages.
             * @function encode
             * @memberof fk_app.Fault
             * @static
             * @param {fk_app.IFault} message Fault message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Fault.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.time != null && message.hasOwnProperty("time"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.time);
                if (message.code != null && message.hasOwnProperty("code"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint32(message.code);
                if (message.description != null && message.hasOwnProperty("description"))
                    writer.uint32(/* id 3, wireType 2 =*/26).string(message.description);
                if (message.debug != null && message.hasOwnProperty("debug"))
                    writer.uint32(/* id 4, wireType 2 =*/34).bytes(message.debug);
                return writer;
            };
    
            /**
             * Encodes the specified Fault message, length delimited. Does not implicitly {@link fk_app.Fault.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.Fault
             * @static
             * @param {fk_app.IFault} message Fault message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Fault.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a Fault message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.Fault
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.Fault} Fault
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Fault.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.Fault();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.time = reader.uint32();
                        break;
                    case 2:
                        message.code = reader.uint32();
                        break;
                    case 3:
                        message.description = reader.string();
                        break;
                    case 4:
                        message.debug = reader.bytes();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a Fault message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.Fault
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.Fault} Fault
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Fault.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a Fault message.
             * @function verify
             * @memberof fk_app.Fault
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            Fault.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.time != null && message.hasOwnProperty("time"))
                    if (!$util.isInteger(message.time))
                        return "time: integer expected";
                if (message.code != null && message.hasOwnProperty("code"))
                    if (!$util.isInteger(message.code))
                        return "code: integer expected";
                if (message.description != null && message.hasOwnProperty("description"))
                    if (!$util.isString(message.description))
                        return "description: string expected";
                if (message.debug != null && message.hasOwnProperty("debug"))
                    if (!(message.debug && typeof message.debug.length === "number" || $util.isString(message.debug)))
                        return "debug: buffer expected";
                return null;
            };
    
            /**
             * Creates a Fault message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.Fault
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.Fault} Fault
             */
            Fault.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.Fault)
                    return object;
                var message = new $root.fk_app.Fault();
                if (object.time != null)
                    message.time = object.time >>> 0;
                if (object.code != null)
                    message.code = object.code >>> 0;
                if (object.description != null)
                    message.description = String(object.description);
                if (object.debug != null)
                    if (typeof object.debug === "string")
                        $util.base64.decode(object.debug, message.debug = $util.newBuffer($util.base64.length(object.debug)), 0);
                    else if (object.debug.length)
                        message.debug = object.debug;
                return message;
            };
    
            /**
             * Creates a plain object from a Fault message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.Fault
             * @static
             * @param {fk_app.Fault} message Fault
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            Fault.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.time = 0;
                    object.code = 0;
                    object.description = "";
                    object.debug = options.bytes === String ? "" : [];
                }
                if (message.time != null && message.hasOwnProperty("time"))
                    object.time = message.time;
                if (message.code != null && message.hasOwnProperty("code"))
                    object.code = message.code;
                if (message.description != null && message.hasOwnProperty("description"))
                    object.description = message.description;
                if (message.debug != null && message.hasOwnProperty("debug"))
                    object.debug = options.bytes === String ? $util.base64.encode(message.debug, 0, message.debug.length) : options.bytes === Array ? Array.prototype.slice.call(message.debug) : message.debug;
                return object;
            };
    
            /**
             * Converts this Fault to JSON.
             * @function toJSON
             * @memberof fk_app.Fault
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Fault.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Fault;
        })();
    
        fk_app.HttpReply = (function() {
    
            /**
             * Properties of a HttpReply.
             * @memberof fk_app
             * @interface IHttpReply
             * @property {fk_app.ReplyType} [type] HttpReply type
             * @property {Array.<fk_app.IError>} [errors] HttpReply errors
             * @property {fk_app.IStatus} [status] HttpReply status
             * @property {fk_app.INetworkSettings} [networkSettings] HttpReply networkSettings
             * @property {fk_app.ILoraSettings} [loraSettings] HttpReply loraSettings
             * @property {Array.<fk_app.IModuleCapabilities>} [modules] HttpReply modules
             * @property {Array.<fk_app.IDataStream>} [streams] HttpReply streams
             * @property {Array.<fk_app.ILiveReadings>} [liveReadings] HttpReply liveReadings
             * @property {fk_app.ISchedules} [schedules] HttpReply schedules
             * @property {fk_app.ITransmission} [transmission] HttpReply transmission
             * @property {fk_app.IDirectoryListing} [listing] HttpReply listing
             * @property {fk_app.INearbyNetworks} [nearbyNetworks] HttpReply nearbyNetworks
             * @property {Array.<fk_app.IFault>} [faults] HttpReply faults
             */
    
            /**
             * Constructs a new HttpReply.
             * @memberof fk_app
             * @classdesc Represents a HttpReply.
             * @constructor
             * @param {fk_app.IHttpReply=} [properties] Properties to set
             */
            function HttpReply(properties) {
                this.errors = [];
                this.modules = [];
                this.streams = [];
                this.liveReadings = [];
                this.faults = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * HttpReply type.
             * @member {fk_app.ReplyType}type
             * @memberof fk_app.HttpReply
             * @instance
             */
            HttpReply.prototype.type = 0;
    
            /**
             * HttpReply errors.
             * @member {Array.<fk_app.IError>}errors
             * @memberof fk_app.HttpReply
             * @instance
             */
            HttpReply.prototype.errors = $util.emptyArray;
    
            /**
             * HttpReply status.
             * @member {(fk_app.IStatus|null|undefined)}status
             * @memberof fk_app.HttpReply
             * @instance
             */
            HttpReply.prototype.status = null;
    
            /**
             * HttpReply networkSettings.
             * @member {(fk_app.INetworkSettings|null|undefined)}networkSettings
             * @memberof fk_app.HttpReply
             * @instance
             */
            HttpReply.prototype.networkSettings = null;
    
            /**
             * HttpReply loraSettings.
             * @member {(fk_app.ILoraSettings|null|undefined)}loraSettings
             * @memberof fk_app.HttpReply
             * @instance
             */
            HttpReply.prototype.loraSettings = null;
    
            /**
             * HttpReply modules.
             * @member {Array.<fk_app.IModuleCapabilities>}modules
             * @memberof fk_app.HttpReply
             * @instance
             */
            HttpReply.prototype.modules = $util.emptyArray;
    
            /**
             * HttpReply streams.
             * @member {Array.<fk_app.IDataStream>}streams
             * @memberof fk_app.HttpReply
             * @instance
             */
            HttpReply.prototype.streams = $util.emptyArray;
    
            /**
             * HttpReply liveReadings.
             * @member {Array.<fk_app.ILiveReadings>}liveReadings
             * @memberof fk_app.HttpReply
             * @instance
             */
            HttpReply.prototype.liveReadings = $util.emptyArray;
    
            /**
             * HttpReply schedules.
             * @member {(fk_app.ISchedules|null|undefined)}schedules
             * @memberof fk_app.HttpReply
             * @instance
             */
            HttpReply.prototype.schedules = null;
    
            /**
             * HttpReply transmission.
             * @member {(fk_app.ITransmission|null|undefined)}transmission
             * @memberof fk_app.HttpReply
             * @instance
             */
            HttpReply.prototype.transmission = null;
    
            /**
             * HttpReply listing.
             * @member {(fk_app.IDirectoryListing|null|undefined)}listing
             * @memberof fk_app.HttpReply
             * @instance
             */
            HttpReply.prototype.listing = null;
    
            /**
             * HttpReply nearbyNetworks.
             * @member {(fk_app.INearbyNetworks|null|undefined)}nearbyNetworks
             * @memberof fk_app.HttpReply
             * @instance
             */
            HttpReply.prototype.nearbyNetworks = null;
    
            /**
             * HttpReply faults.
             * @member {Array.<fk_app.IFault>}faults
             * @memberof fk_app.HttpReply
             * @instance
             */
            HttpReply.prototype.faults = $util.emptyArray;
    
            /**
             * Creates a new HttpReply instance using the specified properties.
             * @function create
             * @memberof fk_app.HttpReply
             * @static
             * @param {fk_app.IHttpReply=} [properties] Properties to set
             * @returns {fk_app.HttpReply} HttpReply instance
             */
            HttpReply.create = function create(properties) {
                return new HttpReply(properties);
            };
    
            /**
             * Encodes the specified HttpReply message. Does not implicitly {@link fk_app.HttpReply.verify|verify} messages.
             * @function encode
             * @memberof fk_app.HttpReply
             * @static
             * @param {fk_app.IHttpReply} message HttpReply message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            HttpReply.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.type != null && message.hasOwnProperty("type"))
                    writer.uint32(/* id 1, wireType 0 =*/8).int32(message.type);
                if (message.errors != null && message.errors.length)
                    for (var i = 0; i < message.errors.length; ++i)
                        $root.fk_app.Error.encode(message.errors[i], writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
                if (message.status != null && message.hasOwnProperty("status"))
                    $root.fk_app.Status.encode(message.status, writer.uint32(/* id 3, wireType 2 =*/26).fork()).ldelim();
                if (message.networkSettings != null && message.hasOwnProperty("networkSettings"))
                    $root.fk_app.NetworkSettings.encode(message.networkSettings, writer.uint32(/* id 4, wireType 2 =*/34).fork()).ldelim();
                if (message.modules != null && message.modules.length)
                    for (var i = 0; i < message.modules.length; ++i)
                        $root.fk_app.ModuleCapabilities.encode(message.modules[i], writer.uint32(/* id 5, wireType 2 =*/42).fork()).ldelim();
                if (message.streams != null && message.streams.length)
                    for (var i = 0; i < message.streams.length; ++i)
                        $root.fk_app.DataStream.encode(message.streams[i], writer.uint32(/* id 6, wireType 2 =*/50).fork()).ldelim();
                if (message.liveReadings != null && message.liveReadings.length)
                    for (var i = 0; i < message.liveReadings.length; ++i)
                        $root.fk_app.LiveReadings.encode(message.liveReadings[i], writer.uint32(/* id 7, wireType 2 =*/58).fork()).ldelim();
                if (message.loraSettings != null && message.hasOwnProperty("loraSettings"))
                    $root.fk_app.LoraSettings.encode(message.loraSettings, writer.uint32(/* id 8, wireType 2 =*/66).fork()).ldelim();
                if (message.schedules != null && message.hasOwnProperty("schedules"))
                    $root.fk_app.Schedules.encode(message.schedules, writer.uint32(/* id 9, wireType 2 =*/74).fork()).ldelim();
                if (message.transmission != null && message.hasOwnProperty("transmission"))
                    $root.fk_app.Transmission.encode(message.transmission, writer.uint32(/* id 10, wireType 2 =*/82).fork()).ldelim();
                if (message.listing != null && message.hasOwnProperty("listing"))
                    $root.fk_app.DirectoryListing.encode(message.listing, writer.uint32(/* id 11, wireType 2 =*/90).fork()).ldelim();
                if (message.nearbyNetworks != null && message.hasOwnProperty("nearbyNetworks"))
                    $root.fk_app.NearbyNetworks.encode(message.nearbyNetworks, writer.uint32(/* id 12, wireType 2 =*/98).fork()).ldelim();
                if (message.faults != null && message.faults.length)
                    for (var i = 0; i < message.faults.length; ++i)
                        $root.fk_app.Fault.encode(message.faults[i], writer.uint32(/* id 13, wireType 2 =*/106).fork()).ldelim();
                return writer;
            };
    
            /**
             * Encodes the specified HttpReply message, length delimited. Does not implicitly {@link fk_app.HttpReply.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.HttpReply
             * @static
             * @param {fk_app.IHttpReply} message HttpReply message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            HttpReply.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a HttpReply message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.HttpReply
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.HttpReply} HttpReply
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            HttpReply.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.HttpReply();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.type = reader.int32();
                        break;
                    case 2:
                        if (!(message.errors && message.errors.length))
                            message.errors = [];
                        message.errors.push($root.fk_app.Error.decode(reader, reader.uint32()));
                        break;
                    case 3:
                        message.status = $root.fk_app.Status.decode(reader, reader.uint32());
                        break;
                    case 4:
                        message.networkSettings = $root.fk_app.NetworkSettings.decode(reader, reader.uint32());
                        break;
                    case 8:
                        message.loraSettings = $root.fk_app.LoraSettings.decode(reader, reader.uint32());
                        break;
                    case 5:
                        if (!(message.modules && message.modules.length))
                            message.modules = [];
                        message.modules.push($root.fk_app.ModuleCapabilities.decode(reader, reader.uint32()));
                        break;
                    case 6:
                        if (!(message.streams && message.streams.length))
                            message.streams = [];
                        message.streams.push($root.fk_app.DataStream.decode(reader, reader.uint32()));
                        break;
                    case 7:
                        if (!(message.liveReadings && message.liveReadings.length))
                            message.liveReadings = [];
                        message.liveReadings.push($root.fk_app.LiveReadings.decode(reader, reader.uint32()));
                        break;
                    case 9:
                        message.schedules = $root.fk_app.Schedules.decode(reader, reader.uint32());
                        break;
                    case 10:
                        message.transmission = $root.fk_app.Transmission.decode(reader, reader.uint32());
                        break;
                    case 11:
                        message.listing = $root.fk_app.DirectoryListing.decode(reader, reader.uint32());
                        break;
                    case 12:
                        message.nearbyNetworks = $root.fk_app.NearbyNetworks.decode(reader, reader.uint32());
                        break;
                    case 13:
                        if (!(message.faults && message.faults.length))
                            message.faults = [];
                        message.faults.push($root.fk_app.Fault.decode(reader, reader.uint32()));
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a HttpReply message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.HttpReply
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.HttpReply} HttpReply
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            HttpReply.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a HttpReply message.
             * @function verify
             * @memberof fk_app.HttpReply
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            HttpReply.verify = function verify(message) {
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
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                        break;
                    }
                if (message.errors != null && message.hasOwnProperty("errors")) {
                    if (!Array.isArray(message.errors))
                        return "errors: array expected";
                    for (var i = 0; i < message.errors.length; ++i) {
                        var error = $root.fk_app.Error.verify(message.errors[i]);
                        if (error)
                            return "errors." + error;
                    }
                }
                if (message.status != null && message.hasOwnProperty("status")) {
                    error = $root.fk_app.Status.verify(message.status);
                    if (error)
                        return "status." + error;
                }
                if (message.networkSettings != null && message.hasOwnProperty("networkSettings")) {
                    error = $root.fk_app.NetworkSettings.verify(message.networkSettings);
                    if (error)
                        return "networkSettings." + error;
                }
                if (message.loraSettings != null && message.hasOwnProperty("loraSettings")) {
                    error = $root.fk_app.LoraSettings.verify(message.loraSettings);
                    if (error)
                        return "loraSettings." + error;
                }
                if (message.modules != null && message.hasOwnProperty("modules")) {
                    if (!Array.isArray(message.modules))
                        return "modules: array expected";
                    for (var i = 0; i < message.modules.length; ++i) {
                        error = $root.fk_app.ModuleCapabilities.verify(message.modules[i]);
                        if (error)
                            return "modules." + error;
                    }
                }
                if (message.streams != null && message.hasOwnProperty("streams")) {
                    if (!Array.isArray(message.streams))
                        return "streams: array expected";
                    for (var i = 0; i < message.streams.length; ++i) {
                        error = $root.fk_app.DataStream.verify(message.streams[i]);
                        if (error)
                            return "streams." + error;
                    }
                }
                if (message.liveReadings != null && message.hasOwnProperty("liveReadings")) {
                    if (!Array.isArray(message.liveReadings))
                        return "liveReadings: array expected";
                    for (var i = 0; i < message.liveReadings.length; ++i) {
                        error = $root.fk_app.LiveReadings.verify(message.liveReadings[i]);
                        if (error)
                            return "liveReadings." + error;
                    }
                }
                if (message.schedules != null && message.hasOwnProperty("schedules")) {
                    error = $root.fk_app.Schedules.verify(message.schedules);
                    if (error)
                        return "schedules." + error;
                }
                if (message.transmission != null && message.hasOwnProperty("transmission")) {
                    error = $root.fk_app.Transmission.verify(message.transmission);
                    if (error)
                        return "transmission." + error;
                }
                if (message.listing != null && message.hasOwnProperty("listing")) {
                    error = $root.fk_app.DirectoryListing.verify(message.listing);
                    if (error)
                        return "listing." + error;
                }
                if (message.nearbyNetworks != null && message.hasOwnProperty("nearbyNetworks")) {
                    error = $root.fk_app.NearbyNetworks.verify(message.nearbyNetworks);
                    if (error)
                        return "nearbyNetworks." + error;
                }
                if (message.faults != null && message.hasOwnProperty("faults")) {
                    if (!Array.isArray(message.faults))
                        return "faults: array expected";
                    for (var i = 0; i < message.faults.length; ++i) {
                        error = $root.fk_app.Fault.verify(message.faults[i]);
                        if (error)
                            return "faults." + error;
                    }
                }
                return null;
            };
    
            /**
             * Creates a HttpReply message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.HttpReply
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.HttpReply} HttpReply
             */
            HttpReply.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.HttpReply)
                    return object;
                var message = new $root.fk_app.HttpReply();
                switch (object.type) {
                case "REPLY_NONE":
                case 0:
                    message.type = 0;
                    break;
                case "REPLY_SUCCESS":
                case 1:
                    message.type = 1;
                    break;
                case "REPLY_BUSY":
                case 2:
                    message.type = 2;
                    break;
                case "REPLY_ERROR":
                case 3:
                    message.type = 3;
                    break;
                case "REPLY_CAPABILITIES":
                case 4:
                    message.type = 4;
                    break;
                case "REPLY_LIVE_DATA_POLL":
                case 8:
                    message.type = 8;
                    break;
                case "REPLY_SCHEDULES":
                case 9:
                    message.type = 9;
                    break;
                case "REPLY_FILES":
                case 10:
                    message.type = 10;
                    break;
                case "REPLY_DOWNLOAD_FILE":
                case 11:
                    message.type = 11;
                    break;
                case "REPLY_RESET":
                case 12:
                    message.type = 12;
                    break;
                case "REPLY_NETWORK_SETTINGS":
                case 13:
                    message.type = 13;
                    break;
                case "REPLY_IDENTITY":
                case 14:
                    message.type = 14;
                    break;
                case "REPLY_STATUS":
                case 15:
                    message.type = 15;
                    break;
                case "REPLY_MODULE":
                case 16:
                    message.type = 16;
                    break;
                case "REPLY_METADATA":
                case 17:
                    message.type = 17;
                    break;
                case "REPLY_READINGS":
                case 18:
                    message.type = 18;
                    break;
                case "REPLY_NETWORKS":
                case 19:
                    message.type = 19;
                    break;
                }
                if (object.errors) {
                    if (!Array.isArray(object.errors))
                        throw TypeError(".fk_app.HttpReply.errors: array expected");
                    message.errors = [];
                    for (var i = 0; i < object.errors.length; ++i) {
                        if (typeof object.errors[i] !== "object")
                            throw TypeError(".fk_app.HttpReply.errors: object expected");
                        message.errors[i] = $root.fk_app.Error.fromObject(object.errors[i]);
                    }
                }
                if (object.status != null) {
                    if (typeof object.status !== "object")
                        throw TypeError(".fk_app.HttpReply.status: object expected");
                    message.status = $root.fk_app.Status.fromObject(object.status);
                }
                if (object.networkSettings != null) {
                    if (typeof object.networkSettings !== "object")
                        throw TypeError(".fk_app.HttpReply.networkSettings: object expected");
                    message.networkSettings = $root.fk_app.NetworkSettings.fromObject(object.networkSettings);
                }
                if (object.loraSettings != null) {
                    if (typeof object.loraSettings !== "object")
                        throw TypeError(".fk_app.HttpReply.loraSettings: object expected");
                    message.loraSettings = $root.fk_app.LoraSettings.fromObject(object.loraSettings);
                }
                if (object.modules) {
                    if (!Array.isArray(object.modules))
                        throw TypeError(".fk_app.HttpReply.modules: array expected");
                    message.modules = [];
                    for (var i = 0; i < object.modules.length; ++i) {
                        if (typeof object.modules[i] !== "object")
                            throw TypeError(".fk_app.HttpReply.modules: object expected");
                        message.modules[i] = $root.fk_app.ModuleCapabilities.fromObject(object.modules[i]);
                    }
                }
                if (object.streams) {
                    if (!Array.isArray(object.streams))
                        throw TypeError(".fk_app.HttpReply.streams: array expected");
                    message.streams = [];
                    for (var i = 0; i < object.streams.length; ++i) {
                        if (typeof object.streams[i] !== "object")
                            throw TypeError(".fk_app.HttpReply.streams: object expected");
                        message.streams[i] = $root.fk_app.DataStream.fromObject(object.streams[i]);
                    }
                }
                if (object.liveReadings) {
                    if (!Array.isArray(object.liveReadings))
                        throw TypeError(".fk_app.HttpReply.liveReadings: array expected");
                    message.liveReadings = [];
                    for (var i = 0; i < object.liveReadings.length; ++i) {
                        if (typeof object.liveReadings[i] !== "object")
                            throw TypeError(".fk_app.HttpReply.liveReadings: object expected");
                        message.liveReadings[i] = $root.fk_app.LiveReadings.fromObject(object.liveReadings[i]);
                    }
                }
                if (object.schedules != null) {
                    if (typeof object.schedules !== "object")
                        throw TypeError(".fk_app.HttpReply.schedules: object expected");
                    message.schedules = $root.fk_app.Schedules.fromObject(object.schedules);
                }
                if (object.transmission != null) {
                    if (typeof object.transmission !== "object")
                        throw TypeError(".fk_app.HttpReply.transmission: object expected");
                    message.transmission = $root.fk_app.Transmission.fromObject(object.transmission);
                }
                if (object.listing != null) {
                    if (typeof object.listing !== "object")
                        throw TypeError(".fk_app.HttpReply.listing: object expected");
                    message.listing = $root.fk_app.DirectoryListing.fromObject(object.listing);
                }
                if (object.nearbyNetworks != null) {
                    if (typeof object.nearbyNetworks !== "object")
                        throw TypeError(".fk_app.HttpReply.nearbyNetworks: object expected");
                    message.nearbyNetworks = $root.fk_app.NearbyNetworks.fromObject(object.nearbyNetworks);
                }
                if (object.faults) {
                    if (!Array.isArray(object.faults))
                        throw TypeError(".fk_app.HttpReply.faults: array expected");
                    message.faults = [];
                    for (var i = 0; i < object.faults.length; ++i) {
                        if (typeof object.faults[i] !== "object")
                            throw TypeError(".fk_app.HttpReply.faults: object expected");
                        message.faults[i] = $root.fk_app.Fault.fromObject(object.faults[i]);
                    }
                }
                return message;
            };
    
            /**
             * Creates a plain object from a HttpReply message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.HttpReply
             * @static
             * @param {fk_app.HttpReply} message HttpReply
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            HttpReply.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults) {
                    object.errors = [];
                    object.modules = [];
                    object.streams = [];
                    object.liveReadings = [];
                    object.faults = [];
                }
                if (options.defaults) {
                    object.type = options.enums === String ? "REPLY_NONE" : 0;
                    object.status = null;
                    object.networkSettings = null;
                    object.loraSettings = null;
                    object.schedules = null;
                    object.transmission = null;
                    object.listing = null;
                    object.nearbyNetworks = null;
                }
                if (message.type != null && message.hasOwnProperty("type"))
                    object.type = options.enums === String ? $root.fk_app.ReplyType[message.type] : message.type;
                if (message.errors && message.errors.length) {
                    object.errors = [];
                    for (var j = 0; j < message.errors.length; ++j)
                        object.errors[j] = $root.fk_app.Error.toObject(message.errors[j], options);
                }
                if (message.status != null && message.hasOwnProperty("status"))
                    object.status = $root.fk_app.Status.toObject(message.status, options);
                if (message.networkSettings != null && message.hasOwnProperty("networkSettings"))
                    object.networkSettings = $root.fk_app.NetworkSettings.toObject(message.networkSettings, options);
                if (message.modules && message.modules.length) {
                    object.modules = [];
                    for (var j = 0; j < message.modules.length; ++j)
                        object.modules[j] = $root.fk_app.ModuleCapabilities.toObject(message.modules[j], options);
                }
                if (message.streams && message.streams.length) {
                    object.streams = [];
                    for (var j = 0; j < message.streams.length; ++j)
                        object.streams[j] = $root.fk_app.DataStream.toObject(message.streams[j], options);
                }
                if (message.liveReadings && message.liveReadings.length) {
                    object.liveReadings = [];
                    for (var j = 0; j < message.liveReadings.length; ++j)
                        object.liveReadings[j] = $root.fk_app.LiveReadings.toObject(message.liveReadings[j], options);
                }
                if (message.loraSettings != null && message.hasOwnProperty("loraSettings"))
                    object.loraSettings = $root.fk_app.LoraSettings.toObject(message.loraSettings, options);
                if (message.schedules != null && message.hasOwnProperty("schedules"))
                    object.schedules = $root.fk_app.Schedules.toObject(message.schedules, options);
                if (message.transmission != null && message.hasOwnProperty("transmission"))
                    object.transmission = $root.fk_app.Transmission.toObject(message.transmission, options);
                if (message.listing != null && message.hasOwnProperty("listing"))
                    object.listing = $root.fk_app.DirectoryListing.toObject(message.listing, options);
                if (message.nearbyNetworks != null && message.hasOwnProperty("nearbyNetworks"))
                    object.nearbyNetworks = $root.fk_app.NearbyNetworks.toObject(message.nearbyNetworks, options);
                if (message.faults && message.faults.length) {
                    object.faults = [];
                    for (var j = 0; j < message.faults.length; ++j)
                        object.faults[j] = $root.fk_app.Fault.toObject(message.faults[j], options);
                }
                return object;
            };
    
            /**
             * Converts this HttpReply to JSON.
             * @function toJSON
             * @memberof fk_app.HttpReply
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            HttpReply.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return HttpReply;
        })();
    
        /**
         * ModuleQueryType enum.
         * @enum {string}
         * @property {number} MODULE_QUERY_NONE=0 MODULE_QUERY_NONE value
         * @property {number} MODULE_QUERY_STATUS=1 MODULE_QUERY_STATUS value
         * @property {number} MODULE_QUERY_CONFIGURE=2 MODULE_QUERY_CONFIGURE value
         * @property {number} MODULE_QUERY_RESET=3 MODULE_QUERY_RESET value
         */
        fk_app.ModuleQueryType = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "MODULE_QUERY_NONE"] = 0;
            values[valuesById[1] = "MODULE_QUERY_STATUS"] = 1;
            values[valuesById[2] = "MODULE_QUERY_CONFIGURE"] = 2;
            values[valuesById[3] = "MODULE_QUERY_RESET"] = 3;
            return values;
        })();
    
        fk_app.ModuleHttpQuery = (function() {
    
            /**
             * Properties of a ModuleHttpQuery.
             * @memberof fk_app
             * @interface IModuleHttpQuery
             * @property {fk_app.ModuleQueryType} [type] ModuleHttpQuery type
             * @property {Array.<fk_app.IError>} [errors] ModuleHttpQuery errors
             * @property {Uint8Array} [configuration] ModuleHttpQuery configuration
             */
    
            /**
             * Constructs a new ModuleHttpQuery.
             * @memberof fk_app
             * @classdesc Represents a ModuleHttpQuery.
             * @constructor
             * @param {fk_app.IModuleHttpQuery=} [properties] Properties to set
             */
            function ModuleHttpQuery(properties) {
                this.errors = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * ModuleHttpQuery type.
             * @member {fk_app.ModuleQueryType}type
             * @memberof fk_app.ModuleHttpQuery
             * @instance
             */
            ModuleHttpQuery.prototype.type = 0;
    
            /**
             * ModuleHttpQuery errors.
             * @member {Array.<fk_app.IError>}errors
             * @memberof fk_app.ModuleHttpQuery
             * @instance
             */
            ModuleHttpQuery.prototype.errors = $util.emptyArray;
    
            /**
             * ModuleHttpQuery configuration.
             * @member {Uint8Array}configuration
             * @memberof fk_app.ModuleHttpQuery
             * @instance
             */
            ModuleHttpQuery.prototype.configuration = $util.newBuffer([]);
    
            /**
             * Creates a new ModuleHttpQuery instance using the specified properties.
             * @function create
             * @memberof fk_app.ModuleHttpQuery
             * @static
             * @param {fk_app.IModuleHttpQuery=} [properties] Properties to set
             * @returns {fk_app.ModuleHttpQuery} ModuleHttpQuery instance
             */
            ModuleHttpQuery.create = function create(properties) {
                return new ModuleHttpQuery(properties);
            };
    
            /**
             * Encodes the specified ModuleHttpQuery message. Does not implicitly {@link fk_app.ModuleHttpQuery.verify|verify} messages.
             * @function encode
             * @memberof fk_app.ModuleHttpQuery
             * @static
             * @param {fk_app.IModuleHttpQuery} message ModuleHttpQuery message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            ModuleHttpQuery.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.type != null && message.hasOwnProperty("type"))
                    writer.uint32(/* id 1, wireType 0 =*/8).int32(message.type);
                if (message.errors != null && message.errors.length)
                    for (var i = 0; i < message.errors.length; ++i)
                        $root.fk_app.Error.encode(message.errors[i], writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
                if (message.configuration != null && message.hasOwnProperty("configuration"))
                    writer.uint32(/* id 3, wireType 2 =*/26).bytes(message.configuration);
                return writer;
            };
    
            /**
             * Encodes the specified ModuleHttpQuery message, length delimited. Does not implicitly {@link fk_app.ModuleHttpQuery.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.ModuleHttpQuery
             * @static
             * @param {fk_app.IModuleHttpQuery} message ModuleHttpQuery message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            ModuleHttpQuery.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a ModuleHttpQuery message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.ModuleHttpQuery
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.ModuleHttpQuery} ModuleHttpQuery
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            ModuleHttpQuery.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.ModuleHttpQuery();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.type = reader.int32();
                        break;
                    case 2:
                        if (!(message.errors && message.errors.length))
                            message.errors = [];
                        message.errors.push($root.fk_app.Error.decode(reader, reader.uint32()));
                        break;
                    case 3:
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
             * Decodes a ModuleHttpQuery message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.ModuleHttpQuery
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.ModuleHttpQuery} ModuleHttpQuery
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            ModuleHttpQuery.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a ModuleHttpQuery message.
             * @function verify
             * @memberof fk_app.ModuleHttpQuery
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            ModuleHttpQuery.verify = function verify(message) {
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
                        break;
                    }
                if (message.errors != null && message.hasOwnProperty("errors")) {
                    if (!Array.isArray(message.errors))
                        return "errors: array expected";
                    for (var i = 0; i < message.errors.length; ++i) {
                        var error = $root.fk_app.Error.verify(message.errors[i]);
                        if (error)
                            return "errors." + error;
                    }
                }
                if (message.configuration != null && message.hasOwnProperty("configuration"))
                    if (!(message.configuration && typeof message.configuration.length === "number" || $util.isString(message.configuration)))
                        return "configuration: buffer expected";
                return null;
            };
    
            /**
             * Creates a ModuleHttpQuery message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.ModuleHttpQuery
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.ModuleHttpQuery} ModuleHttpQuery
             */
            ModuleHttpQuery.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.ModuleHttpQuery)
                    return object;
                var message = new $root.fk_app.ModuleHttpQuery();
                switch (object.type) {
                case "MODULE_QUERY_NONE":
                case 0:
                    message.type = 0;
                    break;
                case "MODULE_QUERY_STATUS":
                case 1:
                    message.type = 1;
                    break;
                case "MODULE_QUERY_CONFIGURE":
                case 2:
                    message.type = 2;
                    break;
                case "MODULE_QUERY_RESET":
                case 3:
                    message.type = 3;
                    break;
                }
                if (object.errors) {
                    if (!Array.isArray(object.errors))
                        throw TypeError(".fk_app.ModuleHttpQuery.errors: array expected");
                    message.errors = [];
                    for (var i = 0; i < object.errors.length; ++i) {
                        if (typeof object.errors[i] !== "object")
                            throw TypeError(".fk_app.ModuleHttpQuery.errors: object expected");
                        message.errors[i] = $root.fk_app.Error.fromObject(object.errors[i]);
                    }
                }
                if (object.configuration != null)
                    if (typeof object.configuration === "string")
                        $util.base64.decode(object.configuration, message.configuration = $util.newBuffer($util.base64.length(object.configuration)), 0);
                    else if (object.configuration.length)
                        message.configuration = object.configuration;
                return message;
            };
    
            /**
             * Creates a plain object from a ModuleHttpQuery message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.ModuleHttpQuery
             * @static
             * @param {fk_app.ModuleHttpQuery} message ModuleHttpQuery
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            ModuleHttpQuery.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults)
                    object.errors = [];
                if (options.defaults) {
                    object.type = options.enums === String ? "MODULE_QUERY_NONE" : 0;
                    object.configuration = options.bytes === String ? "" : [];
                }
                if (message.type != null && message.hasOwnProperty("type"))
                    object.type = options.enums === String ? $root.fk_app.ModuleQueryType[message.type] : message.type;
                if (message.errors && message.errors.length) {
                    object.errors = [];
                    for (var j = 0; j < message.errors.length; ++j)
                        object.errors[j] = $root.fk_app.Error.toObject(message.errors[j], options);
                }
                if (message.configuration != null && message.hasOwnProperty("configuration"))
                    object.configuration = options.bytes === String ? $util.base64.encode(message.configuration, 0, message.configuration.length) : options.bytes === Array ? Array.prototype.slice.call(message.configuration) : message.configuration;
                return object;
            };
    
            /**
             * Converts this ModuleHttpQuery to JSON.
             * @function toJSON
             * @memberof fk_app.ModuleHttpQuery
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            ModuleHttpQuery.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return ModuleHttpQuery;
        })();
    
        /**
         * ModuleReplyType enum.
         * @enum {string}
         * @property {number} MODULE_REPLY_NONE=0 MODULE_REPLY_NONE value
         * @property {number} MODULE_REPLY_SUCCESS=1 MODULE_REPLY_SUCCESS value
         * @property {number} MODULE_REPLY_BUSY=2 MODULE_REPLY_BUSY value
         * @property {number} MODULE_REPLY_ERROR=3 MODULE_REPLY_ERROR value
         */
        fk_app.ModuleReplyType = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "MODULE_REPLY_NONE"] = 0;
            values[valuesById[1] = "MODULE_REPLY_SUCCESS"] = 1;
            values[valuesById[2] = "MODULE_REPLY_BUSY"] = 2;
            values[valuesById[3] = "MODULE_REPLY_ERROR"] = 3;
            return values;
        })();
    
        fk_app.ModuleHttpReply = (function() {
    
            /**
             * Properties of a ModuleHttpReply.
             * @memberof fk_app
             * @interface IModuleHttpReply
             * @property {fk_app.ModuleReplyType} [type] ModuleHttpReply type
             * @property {Array.<fk_app.IError>} [errors] ModuleHttpReply errors
             * @property {Uint8Array} [configuration] ModuleHttpReply configuration
             */
    
            /**
             * Constructs a new ModuleHttpReply.
             * @memberof fk_app
             * @classdesc Represents a ModuleHttpReply.
             * @constructor
             * @param {fk_app.IModuleHttpReply=} [properties] Properties to set
             */
            function ModuleHttpReply(properties) {
                this.errors = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * ModuleHttpReply type.
             * @member {fk_app.ModuleReplyType}type
             * @memberof fk_app.ModuleHttpReply
             * @instance
             */
            ModuleHttpReply.prototype.type = 0;
    
            /**
             * ModuleHttpReply errors.
             * @member {Array.<fk_app.IError>}errors
             * @memberof fk_app.ModuleHttpReply
             * @instance
             */
            ModuleHttpReply.prototype.errors = $util.emptyArray;
    
            /**
             * ModuleHttpReply configuration.
             * @member {Uint8Array}configuration
             * @memberof fk_app.ModuleHttpReply
             * @instance
             */
            ModuleHttpReply.prototype.configuration = $util.newBuffer([]);
    
            /**
             * Creates a new ModuleHttpReply instance using the specified properties.
             * @function create
             * @memberof fk_app.ModuleHttpReply
             * @static
             * @param {fk_app.IModuleHttpReply=} [properties] Properties to set
             * @returns {fk_app.ModuleHttpReply} ModuleHttpReply instance
             */
            ModuleHttpReply.create = function create(properties) {
                return new ModuleHttpReply(properties);
            };
    
            /**
             * Encodes the specified ModuleHttpReply message. Does not implicitly {@link fk_app.ModuleHttpReply.verify|verify} messages.
             * @function encode
             * @memberof fk_app.ModuleHttpReply
             * @static
             * @param {fk_app.IModuleHttpReply} message ModuleHttpReply message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            ModuleHttpReply.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.type != null && message.hasOwnProperty("type"))
                    writer.uint32(/* id 1, wireType 0 =*/8).int32(message.type);
                if (message.errors != null && message.errors.length)
                    for (var i = 0; i < message.errors.length; ++i)
                        $root.fk_app.Error.encode(message.errors[i], writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
                if (message.configuration != null && message.hasOwnProperty("configuration"))
                    writer.uint32(/* id 3, wireType 2 =*/26).bytes(message.configuration);
                return writer;
            };
    
            /**
             * Encodes the specified ModuleHttpReply message, length delimited. Does not implicitly {@link fk_app.ModuleHttpReply.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.ModuleHttpReply
             * @static
             * @param {fk_app.IModuleHttpReply} message ModuleHttpReply message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            ModuleHttpReply.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a ModuleHttpReply message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.ModuleHttpReply
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.ModuleHttpReply} ModuleHttpReply
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            ModuleHttpReply.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.ModuleHttpReply();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.type = reader.int32();
                        break;
                    case 2:
                        if (!(message.errors && message.errors.length))
                            message.errors = [];
                        message.errors.push($root.fk_app.Error.decode(reader, reader.uint32()));
                        break;
                    case 3:
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
             * Decodes a ModuleHttpReply message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.ModuleHttpReply
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.ModuleHttpReply} ModuleHttpReply
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            ModuleHttpReply.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a ModuleHttpReply message.
             * @function verify
             * @memberof fk_app.ModuleHttpReply
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            ModuleHttpReply.verify = function verify(message) {
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
                        break;
                    }
                if (message.errors != null && message.hasOwnProperty("errors")) {
                    if (!Array.isArray(message.errors))
                        return "errors: array expected";
                    for (var i = 0; i < message.errors.length; ++i) {
                        var error = $root.fk_app.Error.verify(message.errors[i]);
                        if (error)
                            return "errors." + error;
                    }
                }
                if (message.configuration != null && message.hasOwnProperty("configuration"))
                    if (!(message.configuration && typeof message.configuration.length === "number" || $util.isString(message.configuration)))
                        return "configuration: buffer expected";
                return null;
            };
    
            /**
             * Creates a ModuleHttpReply message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.ModuleHttpReply
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.ModuleHttpReply} ModuleHttpReply
             */
            ModuleHttpReply.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.ModuleHttpReply)
                    return object;
                var message = new $root.fk_app.ModuleHttpReply();
                switch (object.type) {
                case "MODULE_REPLY_NONE":
                case 0:
                    message.type = 0;
                    break;
                case "MODULE_REPLY_SUCCESS":
                case 1:
                    message.type = 1;
                    break;
                case "MODULE_REPLY_BUSY":
                case 2:
                    message.type = 2;
                    break;
                case "MODULE_REPLY_ERROR":
                case 3:
                    message.type = 3;
                    break;
                }
                if (object.errors) {
                    if (!Array.isArray(object.errors))
                        throw TypeError(".fk_app.ModuleHttpReply.errors: array expected");
                    message.errors = [];
                    for (var i = 0; i < object.errors.length; ++i) {
                        if (typeof object.errors[i] !== "object")
                            throw TypeError(".fk_app.ModuleHttpReply.errors: object expected");
                        message.errors[i] = $root.fk_app.Error.fromObject(object.errors[i]);
                    }
                }
                if (object.configuration != null)
                    if (typeof object.configuration === "string")
                        $util.base64.decode(object.configuration, message.configuration = $util.newBuffer($util.base64.length(object.configuration)), 0);
                    else if (object.configuration.length)
                        message.configuration = object.configuration;
                return message;
            };
    
            /**
             * Creates a plain object from a ModuleHttpReply message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.ModuleHttpReply
             * @static
             * @param {fk_app.ModuleHttpReply} message ModuleHttpReply
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            ModuleHttpReply.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults)
                    object.errors = [];
                if (options.defaults) {
                    object.type = options.enums === String ? "MODULE_REPLY_NONE" : 0;
                    object.configuration = options.bytes === String ? "" : [];
                }
                if (message.type != null && message.hasOwnProperty("type"))
                    object.type = options.enums === String ? $root.fk_app.ModuleReplyType[message.type] : message.type;
                if (message.errors && message.errors.length) {
                    object.errors = [];
                    for (var j = 0; j < message.errors.length; ++j)
                        object.errors[j] = $root.fk_app.Error.toObject(message.errors[j], options);
                }
                if (message.configuration != null && message.hasOwnProperty("configuration"))
                    object.configuration = options.bytes === String ? $util.base64.encode(message.configuration, 0, message.configuration.length) : options.bytes === Array ? Array.prototype.slice.call(message.configuration) : message.configuration;
                return object;
            };
    
            /**
             * Converts this ModuleHttpReply to JSON.
             * @function toJSON
             * @memberof fk_app.ModuleHttpReply
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            ModuleHttpReply.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return ModuleHttpReply;
        })();
    
        /**
         * UdpStatus enum.
         * @enum {string}
         * @property {number} UDP_STATUS_ONLINE=0 UDP_STATUS_ONLINE value
         * @property {number} UDP_STATUS_BYE=1 UDP_STATUS_BYE value
         */
        fk_app.UdpStatus = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "UDP_STATUS_ONLINE"] = 0;
            values[valuesById[1] = "UDP_STATUS_BYE"] = 1;
            return values;
        })();
    
        fk_app.UdpMessage = (function() {
    
            /**
             * Properties of an UdpMessage.
             * @memberof fk_app
             * @interface IUdpMessage
             * @property {Uint8Array} [deviceId] UdpMessage deviceId
             * @property {fk_app.UdpStatus} [status] UdpMessage status
             * @property {number} [counter] UdpMessage counter
             */
    
            /**
             * Constructs a new UdpMessage.
             * @memberof fk_app
             * @classdesc Represents an UdpMessage.
             * @constructor
             * @param {fk_app.IUdpMessage=} [properties] Properties to set
             */
            function UdpMessage(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * UdpMessage deviceId.
             * @member {Uint8Array}deviceId
             * @memberof fk_app.UdpMessage
             * @instance
             */
            UdpMessage.prototype.deviceId = $util.newBuffer([]);
    
            /**
             * UdpMessage status.
             * @member {fk_app.UdpStatus}status
             * @memberof fk_app.UdpMessage
             * @instance
             */
            UdpMessage.prototype.status = 0;
    
            /**
             * UdpMessage counter.
             * @member {number}counter
             * @memberof fk_app.UdpMessage
             * @instance
             */
            UdpMessage.prototype.counter = 0;
    
            /**
             * Creates a new UdpMessage instance using the specified properties.
             * @function create
             * @memberof fk_app.UdpMessage
             * @static
             * @param {fk_app.IUdpMessage=} [properties] Properties to set
             * @returns {fk_app.UdpMessage} UdpMessage instance
             */
            UdpMessage.create = function create(properties) {
                return new UdpMessage(properties);
            };
    
            /**
             * Encodes the specified UdpMessage message. Does not implicitly {@link fk_app.UdpMessage.verify|verify} messages.
             * @function encode
             * @memberof fk_app.UdpMessage
             * @static
             * @param {fk_app.IUdpMessage} message UdpMessage message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            UdpMessage.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.deviceId != null && message.hasOwnProperty("deviceId"))
                    writer.uint32(/* id 1, wireType 2 =*/10).bytes(message.deviceId);
                if (message.status != null && message.hasOwnProperty("status"))
                    writer.uint32(/* id 2, wireType 0 =*/16).int32(message.status);
                if (message.counter != null && message.hasOwnProperty("counter"))
                    writer.uint32(/* id 3, wireType 0 =*/24).uint32(message.counter);
                return writer;
            };
    
            /**
             * Encodes the specified UdpMessage message, length delimited. Does not implicitly {@link fk_app.UdpMessage.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_app.UdpMessage
             * @static
             * @param {fk_app.IUdpMessage} message UdpMessage message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            UdpMessage.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes an UdpMessage message from the specified reader or buffer.
             * @function decode
             * @memberof fk_app.UdpMessage
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_app.UdpMessage} UdpMessage
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            UdpMessage.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_app.UdpMessage();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.deviceId = reader.bytes();
                        break;
                    case 2:
                        message.status = reader.int32();
                        break;
                    case 3:
                        message.counter = reader.uint32();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes an UdpMessage message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_app.UdpMessage
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_app.UdpMessage} UdpMessage
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            UdpMessage.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies an UdpMessage message.
             * @function verify
             * @memberof fk_app.UdpMessage
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            UdpMessage.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.deviceId != null && message.hasOwnProperty("deviceId"))
                    if (!(message.deviceId && typeof message.deviceId.length === "number" || $util.isString(message.deviceId)))
                        return "deviceId: buffer expected";
                if (message.status != null && message.hasOwnProperty("status"))
                    switch (message.status) {
                    default:
                        return "status: enum value expected";
                    case 0:
                    case 1:
                        break;
                    }
                if (message.counter != null && message.hasOwnProperty("counter"))
                    if (!$util.isInteger(message.counter))
                        return "counter: integer expected";
                return null;
            };
    
            /**
             * Creates an UdpMessage message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_app.UdpMessage
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_app.UdpMessage} UdpMessage
             */
            UdpMessage.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_app.UdpMessage)
                    return object;
                var message = new $root.fk_app.UdpMessage();
                if (object.deviceId != null)
                    if (typeof object.deviceId === "string")
                        $util.base64.decode(object.deviceId, message.deviceId = $util.newBuffer($util.base64.length(object.deviceId)), 0);
                    else if (object.deviceId.length)
                        message.deviceId = object.deviceId;
                switch (object.status) {
                case "UDP_STATUS_ONLINE":
                case 0:
                    message.status = 0;
                    break;
                case "UDP_STATUS_BYE":
                case 1:
                    message.status = 1;
                    break;
                }
                if (object.counter != null)
                    message.counter = object.counter >>> 0;
                return message;
            };
    
            /**
             * Creates a plain object from an UdpMessage message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_app.UdpMessage
             * @static
             * @param {fk_app.UdpMessage} message UdpMessage
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            UdpMessage.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.deviceId = options.bytes === String ? "" : [];
                    object.status = options.enums === String ? "UDP_STATUS_ONLINE" : 0;
                    object.counter = 0;
                }
                if (message.deviceId != null && message.hasOwnProperty("deviceId"))
                    object.deviceId = options.bytes === String ? $util.base64.encode(message.deviceId, 0, message.deviceId.length) : options.bytes === Array ? Array.prototype.slice.call(message.deviceId) : message.deviceId;
                if (message.status != null && message.hasOwnProperty("status"))
                    object.status = options.enums === String ? $root.fk_app.UdpStatus[message.status] : message.status;
                if (message.counter != null && message.hasOwnProperty("counter"))
                    object.counter = message.counter;
                return object;
            };
    
            /**
             * Converts this UdpMessage to JSON.
             * @function toJSON
             * @memberof fk_app.UdpMessage
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            UdpMessage.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return UdpMessage;
        })();
    
        return fk_app;
    })();

    return $root;
});
