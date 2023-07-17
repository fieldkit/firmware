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
    
    $root.fk_data = (function() {
    
        /**
         * Namespace fk_data.
         * @exports fk_data
         * @namespace
         */
        var fk_data = {};
    
        fk_data.DeviceLocation = (function() {
    
            /**
             * Properties of a DeviceLocation.
             * @memberof fk_data
             * @interface IDeviceLocation
             * @property {number|null} [enabled] DeviceLocation enabled
             * @property {number|null} [fix] DeviceLocation fix
             * @property {number|Long|null} [time] DeviceLocation time
             * @property {number|null} [longitude] DeviceLocation longitude
             * @property {number|null} [latitude] DeviceLocation latitude
             * @property {number|null} [altitude] DeviceLocation altitude
             * @property {Array.<number>|null} [coordinates] DeviceLocation coordinates
             * @property {number|null} [satellites] DeviceLocation satellites
             * @property {number|null} [hdop] DeviceLocation hdop
             */
    
            /**
             * Constructs a new DeviceLocation.
             * @memberof fk_data
             * @classdesc Represents a DeviceLocation.
             * @implements IDeviceLocation
             * @constructor
             * @param {fk_data.IDeviceLocation=} [properties] Properties to set
             */
            function DeviceLocation(properties) {
                this.coordinates = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * DeviceLocation enabled.
             * @member {number} enabled
             * @memberof fk_data.DeviceLocation
             * @instance
             */
            DeviceLocation.prototype.enabled = 0;
    
            /**
             * DeviceLocation fix.
             * @member {number} fix
             * @memberof fk_data.DeviceLocation
             * @instance
             */
            DeviceLocation.prototype.fix = 0;
    
            /**
             * DeviceLocation time.
             * @member {number|Long} time
             * @memberof fk_data.DeviceLocation
             * @instance
             */
            DeviceLocation.prototype.time = $util.Long ? $util.Long.fromBits(0,0,false) : 0;
    
            /**
             * DeviceLocation longitude.
             * @member {number} longitude
             * @memberof fk_data.DeviceLocation
             * @instance
             */
            DeviceLocation.prototype.longitude = 0;
    
            /**
             * DeviceLocation latitude.
             * @member {number} latitude
             * @memberof fk_data.DeviceLocation
             * @instance
             */
            DeviceLocation.prototype.latitude = 0;
    
            /**
             * DeviceLocation altitude.
             * @member {number} altitude
             * @memberof fk_data.DeviceLocation
             * @instance
             */
            DeviceLocation.prototype.altitude = 0;
    
            /**
             * DeviceLocation coordinates.
             * @member {Array.<number>} coordinates
             * @memberof fk_data.DeviceLocation
             * @instance
             */
            DeviceLocation.prototype.coordinates = $util.emptyArray;
    
            /**
             * DeviceLocation satellites.
             * @member {number} satellites
             * @memberof fk_data.DeviceLocation
             * @instance
             */
            DeviceLocation.prototype.satellites = 0;
    
            /**
             * DeviceLocation hdop.
             * @member {number} hdop
             * @memberof fk_data.DeviceLocation
             * @instance
             */
            DeviceLocation.prototype.hdop = 0;
    
            /**
             * Creates a new DeviceLocation instance using the specified properties.
             * @function create
             * @memberof fk_data.DeviceLocation
             * @static
             * @param {fk_data.IDeviceLocation=} [properties] Properties to set
             * @returns {fk_data.DeviceLocation} DeviceLocation instance
             */
            DeviceLocation.create = function create(properties) {
                return new DeviceLocation(properties);
            };
    
            /**
             * Encodes the specified DeviceLocation message. Does not implicitly {@link fk_data.DeviceLocation.verify|verify} messages.
             * @function encode
             * @memberof fk_data.DeviceLocation
             * @static
             * @param {fk_data.IDeviceLocation} message DeviceLocation message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            DeviceLocation.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.fix != null && message.hasOwnProperty("fix"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.fix);
                if (message.time != null && message.hasOwnProperty("time"))
                    writer.uint32(/* id 2, wireType 0 =*/16).int64(message.time);
                if (message.longitude != null && message.hasOwnProperty("longitude"))
                    writer.uint32(/* id 3, wireType 5 =*/29).float(message.longitude);
                if (message.latitude != null && message.hasOwnProperty("latitude"))
                    writer.uint32(/* id 4, wireType 5 =*/37).float(message.latitude);
                if (message.altitude != null && message.hasOwnProperty("altitude"))
                    writer.uint32(/* id 5, wireType 5 =*/45).float(message.altitude);
                if (message.coordinates != null && message.coordinates.length) {
                    writer.uint32(/* id 6, wireType 2 =*/50).fork();
                    for (var i = 0; i < message.coordinates.length; ++i)
                        writer.float(message.coordinates[i]);
                    writer.ldelim();
                }
                if (message.enabled != null && message.hasOwnProperty("enabled"))
                    writer.uint32(/* id 7, wireType 0 =*/56).uint32(message.enabled);
                if (message.satellites != null && message.hasOwnProperty("satellites"))
                    writer.uint32(/* id 8, wireType 0 =*/64).uint32(message.satellites);
                if (message.hdop != null && message.hasOwnProperty("hdop"))
                    writer.uint32(/* id 9, wireType 0 =*/72).uint32(message.hdop);
                return writer;
            };
    
            /**
             * Encodes the specified DeviceLocation message, length delimited. Does not implicitly {@link fk_data.DeviceLocation.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.DeviceLocation
             * @static
             * @param {fk_data.IDeviceLocation} message DeviceLocation message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            DeviceLocation.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a DeviceLocation message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.DeviceLocation
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.DeviceLocation} DeviceLocation
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            DeviceLocation.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.DeviceLocation();
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
                        message.time = reader.int64();
                        break;
                    case 3:
                        message.longitude = reader.float();
                        break;
                    case 4:
                        message.latitude = reader.float();
                        break;
                    case 5:
                        message.altitude = reader.float();
                        break;
                    case 6:
                        if (!(message.coordinates && message.coordinates.length))
                            message.coordinates = [];
                        if ((tag & 7) === 2) {
                            var end2 = reader.uint32() + reader.pos;
                            while (reader.pos < end2)
                                message.coordinates.push(reader.float());
                        } else
                            message.coordinates.push(reader.float());
                        break;
                    case 8:
                        message.satellites = reader.uint32();
                        break;
                    case 9:
                        message.hdop = reader.uint32();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a DeviceLocation message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_data.DeviceLocation
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.DeviceLocation} DeviceLocation
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            DeviceLocation.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a DeviceLocation message.
             * @function verify
             * @memberof fk_data.DeviceLocation
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            DeviceLocation.verify = function verify(message) {
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
                if (message.longitude != null && message.hasOwnProperty("longitude"))
                    if (typeof message.longitude !== "number")
                        return "longitude: number expected";
                if (message.latitude != null && message.hasOwnProperty("latitude"))
                    if (typeof message.latitude !== "number")
                        return "latitude: number expected";
                if (message.altitude != null && message.hasOwnProperty("altitude"))
                    if (typeof message.altitude !== "number")
                        return "altitude: number expected";
                if (message.coordinates != null && message.hasOwnProperty("coordinates")) {
                    if (!Array.isArray(message.coordinates))
                        return "coordinates: array expected";
                    for (var i = 0; i < message.coordinates.length; ++i)
                        if (typeof message.coordinates[i] !== "number")
                            return "coordinates: number[] expected";
                }
                if (message.satellites != null && message.hasOwnProperty("satellites"))
                    if (!$util.isInteger(message.satellites))
                        return "satellites: integer expected";
                if (message.hdop != null && message.hasOwnProperty("hdop"))
                    if (!$util.isInteger(message.hdop))
                        return "hdop: integer expected";
                return null;
            };
    
            /**
             * Creates a DeviceLocation message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.DeviceLocation
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.DeviceLocation} DeviceLocation
             */
            DeviceLocation.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.DeviceLocation)
                    return object;
                var message = new $root.fk_data.DeviceLocation();
                if (object.enabled != null)
                    message.enabled = object.enabled >>> 0;
                if (object.fix != null)
                    message.fix = object.fix >>> 0;
                if (object.time != null)
                    if ($util.Long)
                        (message.time = $util.Long.fromValue(object.time)).unsigned = false;
                    else if (typeof object.time === "string")
                        message.time = parseInt(object.time, 10);
                    else if (typeof object.time === "number")
                        message.time = object.time;
                    else if (typeof object.time === "object")
                        message.time = new $util.LongBits(object.time.low >>> 0, object.time.high >>> 0).toNumber();
                if (object.longitude != null)
                    message.longitude = Number(object.longitude);
                if (object.latitude != null)
                    message.latitude = Number(object.latitude);
                if (object.altitude != null)
                    message.altitude = Number(object.altitude);
                if (object.coordinates) {
                    if (!Array.isArray(object.coordinates))
                        throw TypeError(".fk_data.DeviceLocation.coordinates: array expected");
                    message.coordinates = [];
                    for (var i = 0; i < object.coordinates.length; ++i)
                        message.coordinates[i] = Number(object.coordinates[i]);
                }
                if (object.satellites != null)
                    message.satellites = object.satellites >>> 0;
                if (object.hdop != null)
                    message.hdop = object.hdop >>> 0;
                return message;
            };
    
            /**
             * Creates a plain object from a DeviceLocation message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.DeviceLocation
             * @static
             * @param {fk_data.DeviceLocation} message DeviceLocation
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            DeviceLocation.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults)
                    object.coordinates = [];
                if (options.defaults) {
                    object.fix = 0;
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, false);
                        object.time = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.time = options.longs === String ? "0" : 0;
                    object.longitude = 0;
                    object.latitude = 0;
                    object.altitude = 0;
                    object.enabled = 0;
                    object.satellites = 0;
                    object.hdop = 0;
                }
                if (message.fix != null && message.hasOwnProperty("fix"))
                    object.fix = message.fix;
                if (message.time != null && message.hasOwnProperty("time"))
                    if (typeof message.time === "number")
                        object.time = options.longs === String ? String(message.time) : message.time;
                    else
                        object.time = options.longs === String ? $util.Long.prototype.toString.call(message.time) : options.longs === Number ? new $util.LongBits(message.time.low >>> 0, message.time.high >>> 0).toNumber() : message.time;
                if (message.longitude != null && message.hasOwnProperty("longitude"))
                    object.longitude = options.json && !isFinite(message.longitude) ? String(message.longitude) : message.longitude;
                if (message.latitude != null && message.hasOwnProperty("latitude"))
                    object.latitude = options.json && !isFinite(message.latitude) ? String(message.latitude) : message.latitude;
                if (message.altitude != null && message.hasOwnProperty("altitude"))
                    object.altitude = options.json && !isFinite(message.altitude) ? String(message.altitude) : message.altitude;
                if (message.coordinates && message.coordinates.length) {
                    object.coordinates = [];
                    for (var j = 0; j < message.coordinates.length; ++j)
                        object.coordinates[j] = options.json && !isFinite(message.coordinates[j]) ? String(message.coordinates[j]) : message.coordinates[j];
                }
                if (message.enabled != null && message.hasOwnProperty("enabled"))
                    object.enabled = message.enabled;
                if (message.satellites != null && message.hasOwnProperty("satellites"))
                    object.satellites = message.satellites;
                if (message.hdop != null && message.hasOwnProperty("hdop"))
                    object.hdop = message.hdop;
                return object;
            };
    
            /**
             * Converts this DeviceLocation to JSON.
             * @function toJSON
             * @memberof fk_data.DeviceLocation
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            DeviceLocation.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return DeviceLocation;
        })();
    
        fk_data.SensorReading = (function() {
    
            /**
             * Properties of a SensorReading.
             * @memberof fk_data
             * @interface ISensorReading
             * @property {number|Long|null} [reading] SensorReading reading
             * @property {number|Long|null} [time] SensorReading time
             * @property {number|null} [sensor] SensorReading sensor
             * @property {number|null} [value] SensorReading value
             */
    
            /**
             * Constructs a new SensorReading.
             * @memberof fk_data
             * @classdesc Represents a SensorReading.
             * @implements ISensorReading
             * @constructor
             * @param {fk_data.ISensorReading=} [properties] Properties to set
             */
            function SensorReading(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * SensorReading reading.
             * @member {number|Long} reading
             * @memberof fk_data.SensorReading
             * @instance
             */
            SensorReading.prototype.reading = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * SensorReading time.
             * @member {number|Long} time
             * @memberof fk_data.SensorReading
             * @instance
             */
            SensorReading.prototype.time = $util.Long ? $util.Long.fromBits(0,0,false) : 0;
    
            /**
             * SensorReading sensor.
             * @member {number} sensor
             * @memberof fk_data.SensorReading
             * @instance
             */
            SensorReading.prototype.sensor = 0;
    
            /**
             * SensorReading value.
             * @member {number} value
             * @memberof fk_data.SensorReading
             * @instance
             */
            SensorReading.prototype.value = 0;
    
            /**
             * Creates a new SensorReading instance using the specified properties.
             * @function create
             * @memberof fk_data.SensorReading
             * @static
             * @param {fk_data.ISensorReading=} [properties] Properties to set
             * @returns {fk_data.SensorReading} SensorReading instance
             */
            SensorReading.create = function create(properties) {
                return new SensorReading(properties);
            };
    
            /**
             * Encodes the specified SensorReading message. Does not implicitly {@link fk_data.SensorReading.verify|verify} messages.
             * @function encode
             * @memberof fk_data.SensorReading
             * @static
             * @param {fk_data.ISensorReading} message SensorReading message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            SensorReading.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.reading != null && message.hasOwnProperty("reading"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint64(message.reading);
                if (message.time != null && message.hasOwnProperty("time"))
                    writer.uint32(/* id 2, wireType 0 =*/16).int64(message.time);
                if (message.sensor != null && message.hasOwnProperty("sensor"))
                    writer.uint32(/* id 3, wireType 0 =*/24).uint32(message.sensor);
                if (message.value != null && message.hasOwnProperty("value"))
                    writer.uint32(/* id 4, wireType 5 =*/37).float(message.value);
                return writer;
            };
    
            /**
             * Encodes the specified SensorReading message, length delimited. Does not implicitly {@link fk_data.SensorReading.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.SensorReading
             * @static
             * @param {fk_data.ISensorReading} message SensorReading message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            SensorReading.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a SensorReading message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.SensorReading
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.SensorReading} SensorReading
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            SensorReading.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.SensorReading();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.reading = reader.uint64();
                        break;
                    case 2:
                        message.time = reader.int64();
                        break;
                    case 3:
                        message.sensor = reader.uint32();
                        break;
                    case 4:
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
             * Decodes a SensorReading message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_data.SensorReading
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.SensorReading} SensorReading
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            SensorReading.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a SensorReading message.
             * @function verify
             * @memberof fk_data.SensorReading
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            SensorReading.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.reading != null && message.hasOwnProperty("reading"))
                    if (!$util.isInteger(message.reading) && !(message.reading && $util.isInteger(message.reading.low) && $util.isInteger(message.reading.high)))
                        return "reading: integer|Long expected";
                if (message.time != null && message.hasOwnProperty("time"))
                    if (!$util.isInteger(message.time) && !(message.time && $util.isInteger(message.time.low) && $util.isInteger(message.time.high)))
                        return "time: integer|Long expected";
                if (message.sensor != null && message.hasOwnProperty("sensor"))
                    if (!$util.isInteger(message.sensor))
                        return "sensor: integer expected";
                if (message.value != null && message.hasOwnProperty("value"))
                    if (typeof message.value !== "number")
                        return "value: number expected";
                return null;
            };
    
            /**
             * Creates a SensorReading message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.SensorReading
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.SensorReading} SensorReading
             */
            SensorReading.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.SensorReading)
                    return object;
                var message = new $root.fk_data.SensorReading();
                if (object.reading != null)
                    if ($util.Long)
                        (message.reading = $util.Long.fromValue(object.reading)).unsigned = true;
                    else if (typeof object.reading === "string")
                        message.reading = parseInt(object.reading, 10);
                    else if (typeof object.reading === "number")
                        message.reading = object.reading;
                    else if (typeof object.reading === "object")
                        message.reading = new $util.LongBits(object.reading.low >>> 0, object.reading.high >>> 0).toNumber(true);
                if (object.time != null)
                    if ($util.Long)
                        (message.time = $util.Long.fromValue(object.time)).unsigned = false;
                    else if (typeof object.time === "string")
                        message.time = parseInt(object.time, 10);
                    else if (typeof object.time === "number")
                        message.time = object.time;
                    else if (typeof object.time === "object")
                        message.time = new $util.LongBits(object.time.low >>> 0, object.time.high >>> 0).toNumber();
                if (object.sensor != null)
                    message.sensor = object.sensor >>> 0;
                if (object.value != null)
                    message.value = Number(object.value);
                return message;
            };
    
            /**
             * Creates a plain object from a SensorReading message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.SensorReading
             * @static
             * @param {fk_data.SensorReading} message SensorReading
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            SensorReading.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.reading = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.reading = options.longs === String ? "0" : 0;
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, false);
                        object.time = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.time = options.longs === String ? "0" : 0;
                    object.sensor = 0;
                    object.value = 0;
                }
                if (message.reading != null && message.hasOwnProperty("reading"))
                    if (typeof message.reading === "number")
                        object.reading = options.longs === String ? String(message.reading) : message.reading;
                    else
                        object.reading = options.longs === String ? $util.Long.prototype.toString.call(message.reading) : options.longs === Number ? new $util.LongBits(message.reading.low >>> 0, message.reading.high >>> 0).toNumber(true) : message.reading;
                if (message.time != null && message.hasOwnProperty("time"))
                    if (typeof message.time === "number")
                        object.time = options.longs === String ? String(message.time) : message.time;
                    else
                        object.time = options.longs === String ? $util.Long.prototype.toString.call(message.time) : options.longs === Number ? new $util.LongBits(message.time.low >>> 0, message.time.high >>> 0).toNumber() : message.time;
                if (message.sensor != null && message.hasOwnProperty("sensor"))
                    object.sensor = message.sensor;
                if (message.value != null && message.hasOwnProperty("value"))
                    object.value = options.json && !isFinite(message.value) ? String(message.value) : message.value;
                return object;
            };
    
            /**
             * Converts this SensorReading to JSON.
             * @function toJSON
             * @memberof fk_data.SensorReading
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            SensorReading.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return SensorReading;
        })();
    
        fk_data.LoggedReading = (function() {
    
            /**
             * Properties of a LoggedReading.
             * @memberof fk_data
             * @interface ILoggedReading
             * @property {number|null} [version] LoggedReading version
             * @property {fk_data.IDeviceLocation|null} [location] LoggedReading location
             * @property {fk_data.ISensorReading|null} [reading] LoggedReading reading
             */
    
            /**
             * Constructs a new LoggedReading.
             * @memberof fk_data
             * @classdesc Represents a LoggedReading.
             * @implements ILoggedReading
             * @constructor
             * @param {fk_data.ILoggedReading=} [properties] Properties to set
             */
            function LoggedReading(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * LoggedReading version.
             * @member {number} version
             * @memberof fk_data.LoggedReading
             * @instance
             */
            LoggedReading.prototype.version = 0;
    
            /**
             * LoggedReading location.
             * @member {fk_data.IDeviceLocation|null|undefined} location
             * @memberof fk_data.LoggedReading
             * @instance
             */
            LoggedReading.prototype.location = null;
    
            /**
             * LoggedReading reading.
             * @member {fk_data.ISensorReading|null|undefined} reading
             * @memberof fk_data.LoggedReading
             * @instance
             */
            LoggedReading.prototype.reading = null;
    
            /**
             * Creates a new LoggedReading instance using the specified properties.
             * @function create
             * @memberof fk_data.LoggedReading
             * @static
             * @param {fk_data.ILoggedReading=} [properties] Properties to set
             * @returns {fk_data.LoggedReading} LoggedReading instance
             */
            LoggedReading.create = function create(properties) {
                return new LoggedReading(properties);
            };
    
            /**
             * Encodes the specified LoggedReading message. Does not implicitly {@link fk_data.LoggedReading.verify|verify} messages.
             * @function encode
             * @memberof fk_data.LoggedReading
             * @static
             * @param {fk_data.ILoggedReading} message LoggedReading message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            LoggedReading.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.version != null && message.hasOwnProperty("version"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.version);
                if (message.location != null && message.hasOwnProperty("location"))
                    $root.fk_data.DeviceLocation.encode(message.location, writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
                if (message.reading != null && message.hasOwnProperty("reading"))
                    $root.fk_data.SensorReading.encode(message.reading, writer.uint32(/* id 3, wireType 2 =*/26).fork()).ldelim();
                return writer;
            };
    
            /**
             * Encodes the specified LoggedReading message, length delimited. Does not implicitly {@link fk_data.LoggedReading.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.LoggedReading
             * @static
             * @param {fk_data.ILoggedReading} message LoggedReading message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            LoggedReading.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a LoggedReading message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.LoggedReading
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.LoggedReading} LoggedReading
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            LoggedReading.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.LoggedReading();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.version = reader.uint32();
                        break;
                    case 2:
                        message.location = $root.fk_data.DeviceLocation.decode(reader, reader.uint32());
                        break;
                    case 3:
                        message.reading = $root.fk_data.SensorReading.decode(reader, reader.uint32());
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a LoggedReading message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_data.LoggedReading
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.LoggedReading} LoggedReading
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            LoggedReading.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a LoggedReading message.
             * @function verify
             * @memberof fk_data.LoggedReading
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            LoggedReading.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.version != null && message.hasOwnProperty("version"))
                    if (!$util.isInteger(message.version))
                        return "version: integer expected";
                if (message.location != null && message.hasOwnProperty("location")) {
                    var error = $root.fk_data.DeviceLocation.verify(message.location);
                    if (error)
                        return "location." + error;
                }
                if (message.reading != null && message.hasOwnProperty("reading")) {
                    var error = $root.fk_data.SensorReading.verify(message.reading);
                    if (error)
                        return "reading." + error;
                }
                return null;
            };
    
            /**
             * Creates a LoggedReading message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.LoggedReading
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.LoggedReading} LoggedReading
             */
            LoggedReading.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.LoggedReading)
                    return object;
                var message = new $root.fk_data.LoggedReading();
                if (object.version != null)
                    message.version = object.version >>> 0;
                if (object.location != null) {
                    if (typeof object.location !== "object")
                        throw TypeError(".fk_data.LoggedReading.location: object expected");
                    message.location = $root.fk_data.DeviceLocation.fromObject(object.location);
                }
                if (object.reading != null) {
                    if (typeof object.reading !== "object")
                        throw TypeError(".fk_data.LoggedReading.reading: object expected");
                    message.reading = $root.fk_data.SensorReading.fromObject(object.reading);
                }
                return message;
            };
    
            /**
             * Creates a plain object from a LoggedReading message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.LoggedReading
             * @static
             * @param {fk_data.LoggedReading} message LoggedReading
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            LoggedReading.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.version = 0;
                    object.location = null;
                    object.reading = null;
                }
                if (message.version != null && message.hasOwnProperty("version"))
                    object.version = message.version;
                if (message.location != null && message.hasOwnProperty("location"))
                    object.location = $root.fk_data.DeviceLocation.toObject(message.location, options);
                if (message.reading != null && message.hasOwnProperty("reading"))
                    object.reading = $root.fk_data.SensorReading.toObject(message.reading, options);
                return object;
            };
    
            /**
             * Converts this LoggedReading to JSON.
             * @function toJSON
             * @memberof fk_data.LoggedReading
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            LoggedReading.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return LoggedReading;
        })();
    
        fk_data.SensorAndValue = (function() {
    
            /**
             * Properties of a SensorAndValue.
             * @memberof fk_data
             * @interface ISensorAndValue
             * @property {number|null} [sensor] SensorAndValue sensor
             * @property {boolean|null} [calibratedNull] SensorAndValue calibratedNull
             * @property {number|null} [calibratedValue] SensorAndValue calibratedValue
             * @property {boolean|null} [uncalibratedNull] SensorAndValue uncalibratedNull
             * @property {number|null} [uncalibratedValue] SensorAndValue uncalibratedValue
             */
    
            /**
             * Constructs a new SensorAndValue.
             * @memberof fk_data
             * @classdesc Represents a SensorAndValue.
             * @implements ISensorAndValue
             * @constructor
             * @param {fk_data.ISensorAndValue=} [properties] Properties to set
             */
            function SensorAndValue(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * SensorAndValue sensor.
             * @member {number} sensor
             * @memberof fk_data.SensorAndValue
             * @instance
             */
            SensorAndValue.prototype.sensor = 0;
    
            /**
             * SensorAndValue calibratedNull.
             * @member {boolean} calibratedNull
             * @memberof fk_data.SensorAndValue
             * @instance
             */
            SensorAndValue.prototype.calibratedNull = false;
    
            /**
             * SensorAndValue calibratedValue.
             * @member {number} calibratedValue
             * @memberof fk_data.SensorAndValue
             * @instance
             */
            SensorAndValue.prototype.calibratedValue = 0;
    
            /**
             * SensorAndValue uncalibratedNull.
             * @member {boolean} uncalibratedNull
             * @memberof fk_data.SensorAndValue
             * @instance
             */
            SensorAndValue.prototype.uncalibratedNull = false;
    
            /**
             * SensorAndValue uncalibratedValue.
             * @member {number} uncalibratedValue
             * @memberof fk_data.SensorAndValue
             * @instance
             */
            SensorAndValue.prototype.uncalibratedValue = 0;
    
            // OneOf field names bound to virtual getters and setters
            var $oneOfFields;
    
            /**
             * SensorAndValue calibrated.
             * @member {"calibratedNull"|"calibratedValue"|undefined} calibrated
             * @memberof fk_data.SensorAndValue
             * @instance
             */
            Object.defineProperty(SensorAndValue.prototype, "calibrated", {
                get: $util.oneOfGetter($oneOfFields = ["calibratedNull", "calibratedValue"]),
                set: $util.oneOfSetter($oneOfFields)
            });
    
            /**
             * SensorAndValue uncalibrated.
             * @member {"uncalibratedNull"|"uncalibratedValue"|undefined} uncalibrated
             * @memberof fk_data.SensorAndValue
             * @instance
             */
            Object.defineProperty(SensorAndValue.prototype, "uncalibrated", {
                get: $util.oneOfGetter($oneOfFields = ["uncalibratedNull", "uncalibratedValue"]),
                set: $util.oneOfSetter($oneOfFields)
            });
    
            /**
             * Creates a new SensorAndValue instance using the specified properties.
             * @function create
             * @memberof fk_data.SensorAndValue
             * @static
             * @param {fk_data.ISensorAndValue=} [properties] Properties to set
             * @returns {fk_data.SensorAndValue} SensorAndValue instance
             */
            SensorAndValue.create = function create(properties) {
                return new SensorAndValue(properties);
            };
    
            /**
             * Encodes the specified SensorAndValue message. Does not implicitly {@link fk_data.SensorAndValue.verify|verify} messages.
             * @function encode
             * @memberof fk_data.SensorAndValue
             * @static
             * @param {fk_data.ISensorAndValue} message SensorAndValue message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            SensorAndValue.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.sensor != null && message.hasOwnProperty("sensor"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.sensor);
                if (message.calibratedValue != null && message.hasOwnProperty("calibratedValue"))
                    writer.uint32(/* id 2, wireType 5 =*/21).float(message.calibratedValue);
                if (message.uncalibratedValue != null && message.hasOwnProperty("uncalibratedValue"))
                    writer.uint32(/* id 3, wireType 5 =*/29).float(message.uncalibratedValue);
                if (message.calibratedNull != null && message.hasOwnProperty("calibratedNull"))
                    writer.uint32(/* id 4, wireType 0 =*/32).bool(message.calibratedNull);
                if (message.uncalibratedNull != null && message.hasOwnProperty("uncalibratedNull"))
                    writer.uint32(/* id 5, wireType 0 =*/40).bool(message.uncalibratedNull);
                return writer;
            };
    
            /**
             * Encodes the specified SensorAndValue message, length delimited. Does not implicitly {@link fk_data.SensorAndValue.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.SensorAndValue
             * @static
             * @param {fk_data.ISensorAndValue} message SensorAndValue message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            SensorAndValue.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a SensorAndValue message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.SensorAndValue
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.SensorAndValue} SensorAndValue
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            SensorAndValue.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.SensorAndValue();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.sensor = reader.uint32();
                        break;
                    case 4:
                        message.calibratedNull = reader.bool();
                        break;
                    case 2:
                        message.calibratedValue = reader.float();
                        break;
                    case 5:
                        message.uncalibratedNull = reader.bool();
                        break;
                    case 3:
                        message.uncalibratedValue = reader.float();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a SensorAndValue message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_data.SensorAndValue
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.SensorAndValue} SensorAndValue
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            SensorAndValue.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a SensorAndValue message.
             * @function verify
             * @memberof fk_data.SensorAndValue
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            SensorAndValue.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                var properties = {};
                if (message.sensor != null && message.hasOwnProperty("sensor"))
                    if (!$util.isInteger(message.sensor))
                        return "sensor: integer expected";
                if (message.calibratedNull != null && message.hasOwnProperty("calibratedNull")) {
                    properties.calibrated = 1;
                    if (typeof message.calibratedNull !== "boolean")
                        return "calibratedNull: boolean expected";
                }
                if (message.calibratedValue != null && message.hasOwnProperty("calibratedValue")) {
                    if (properties.calibrated === 1)
                        return "calibrated: multiple values";
                    properties.calibrated = 1;
                    if (typeof message.calibratedValue !== "number")
                        return "calibratedValue: number expected";
                }
                if (message.uncalibratedNull != null && message.hasOwnProperty("uncalibratedNull")) {
                    properties.uncalibrated = 1;
                    if (typeof message.uncalibratedNull !== "boolean")
                        return "uncalibratedNull: boolean expected";
                }
                if (message.uncalibratedValue != null && message.hasOwnProperty("uncalibratedValue")) {
                    if (properties.uncalibrated === 1)
                        return "uncalibrated: multiple values";
                    properties.uncalibrated = 1;
                    if (typeof message.uncalibratedValue !== "number")
                        return "uncalibratedValue: number expected";
                }
                return null;
            };
    
            /**
             * Creates a SensorAndValue message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.SensorAndValue
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.SensorAndValue} SensorAndValue
             */
            SensorAndValue.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.SensorAndValue)
                    return object;
                var message = new $root.fk_data.SensorAndValue();
                if (object.sensor != null)
                    message.sensor = object.sensor >>> 0;
                if (object.calibratedNull != null)
                    message.calibratedNull = Boolean(object.calibratedNull);
                if (object.calibratedValue != null)
                    message.calibratedValue = Number(object.calibratedValue);
                if (object.uncalibratedNull != null)
                    message.uncalibratedNull = Boolean(object.uncalibratedNull);
                if (object.uncalibratedValue != null)
                    message.uncalibratedValue = Number(object.uncalibratedValue);
                return message;
            };
    
            /**
             * Creates a plain object from a SensorAndValue message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.SensorAndValue
             * @static
             * @param {fk_data.SensorAndValue} message SensorAndValue
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            SensorAndValue.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults)
                    object.sensor = 0;
                if (message.sensor != null && message.hasOwnProperty("sensor"))
                    object.sensor = message.sensor;
                if (message.calibratedValue != null && message.hasOwnProperty("calibratedValue")) {
                    object.calibratedValue = options.json && !isFinite(message.calibratedValue) ? String(message.calibratedValue) : message.calibratedValue;
                    if (options.oneofs)
                        object.calibrated = "calibratedValue";
                }
                if (message.uncalibratedValue != null && message.hasOwnProperty("uncalibratedValue")) {
                    object.uncalibratedValue = options.json && !isFinite(message.uncalibratedValue) ? String(message.uncalibratedValue) : message.uncalibratedValue;
                    if (options.oneofs)
                        object.uncalibrated = "uncalibratedValue";
                }
                if (message.calibratedNull != null && message.hasOwnProperty("calibratedNull")) {
                    object.calibratedNull = message.calibratedNull;
                    if (options.oneofs)
                        object.calibrated = "calibratedNull";
                }
                if (message.uncalibratedNull != null && message.hasOwnProperty("uncalibratedNull")) {
                    object.uncalibratedNull = message.uncalibratedNull;
                    if (options.oneofs)
                        object.uncalibrated = "uncalibratedNull";
                }
                return object;
            };
    
            /**
             * Converts this SensorAndValue to JSON.
             * @function toJSON
             * @memberof fk_data.SensorAndValue
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            SensorAndValue.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return SensorAndValue;
        })();
    
        fk_data.ModuleHeader = (function() {
    
            /**
             * Properties of a ModuleHeader.
             * @memberof fk_data
             * @interface IModuleHeader
             * @property {number|null} [manufacturer] ModuleHeader manufacturer
             * @property {number|null} [kind] ModuleHeader kind
             * @property {number|null} [version] ModuleHeader version
             */
    
            /**
             * Constructs a new ModuleHeader.
             * @memberof fk_data
             * @classdesc Represents a ModuleHeader.
             * @implements IModuleHeader
             * @constructor
             * @param {fk_data.IModuleHeader=} [properties] Properties to set
             */
            function ModuleHeader(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * ModuleHeader manufacturer.
             * @member {number} manufacturer
             * @memberof fk_data.ModuleHeader
             * @instance
             */
            ModuleHeader.prototype.manufacturer = 0;
    
            /**
             * ModuleHeader kind.
             * @member {number} kind
             * @memberof fk_data.ModuleHeader
             * @instance
             */
            ModuleHeader.prototype.kind = 0;
    
            /**
             * ModuleHeader version.
             * @member {number} version
             * @memberof fk_data.ModuleHeader
             * @instance
             */
            ModuleHeader.prototype.version = 0;
    
            /**
             * Creates a new ModuleHeader instance using the specified properties.
             * @function create
             * @memberof fk_data.ModuleHeader
             * @static
             * @param {fk_data.IModuleHeader=} [properties] Properties to set
             * @returns {fk_data.ModuleHeader} ModuleHeader instance
             */
            ModuleHeader.create = function create(properties) {
                return new ModuleHeader(properties);
            };
    
            /**
             * Encodes the specified ModuleHeader message. Does not implicitly {@link fk_data.ModuleHeader.verify|verify} messages.
             * @function encode
             * @memberof fk_data.ModuleHeader
             * @static
             * @param {fk_data.IModuleHeader} message ModuleHeader message or plain object to encode
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
             * Encodes the specified ModuleHeader message, length delimited. Does not implicitly {@link fk_data.ModuleHeader.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.ModuleHeader
             * @static
             * @param {fk_data.IModuleHeader} message ModuleHeader message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            ModuleHeader.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a ModuleHeader message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.ModuleHeader
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.ModuleHeader} ModuleHeader
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            ModuleHeader.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.ModuleHeader();
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
             * @memberof fk_data.ModuleHeader
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.ModuleHeader} ModuleHeader
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
             * @memberof fk_data.ModuleHeader
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
             * @memberof fk_data.ModuleHeader
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.ModuleHeader} ModuleHeader
             */
            ModuleHeader.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.ModuleHeader)
                    return object;
                var message = new $root.fk_data.ModuleHeader();
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
             * @memberof fk_data.ModuleHeader
             * @static
             * @param {fk_data.ModuleHeader} message ModuleHeader
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
             * @memberof fk_data.ModuleHeader
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            ModuleHeader.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return ModuleHeader;
        })();
    
        fk_data.ModuleInfo = (function() {
    
            /**
             * Properties of a ModuleInfo.
             * @memberof fk_data
             * @interface IModuleInfo
             * @property {number|null} [position] ModuleInfo position
             * @property {number|null} [address] ModuleInfo address
             * @property {string|null} [name] ModuleInfo name
             * @property {fk_data.IModuleHeader|null} [header] ModuleInfo header
             * @property {fk_data.IFirmware|null} [firmware] ModuleInfo firmware
             * @property {Array.<fk_data.ISensorInfo>|null} [sensors] ModuleInfo sensors
             * @property {Uint8Array|null} [id] ModuleInfo id
             * @property {number|null} [flags] ModuleInfo flags
             * @property {Uint8Array|null} [configuration] ModuleInfo configuration
             */
    
            /**
             * Constructs a new ModuleInfo.
             * @memberof fk_data
             * @classdesc Represents a ModuleInfo.
             * @implements IModuleInfo
             * @constructor
             * @param {fk_data.IModuleInfo=} [properties] Properties to set
             */
            function ModuleInfo(properties) {
                this.sensors = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * ModuleInfo position.
             * @member {number} position
             * @memberof fk_data.ModuleInfo
             * @instance
             */
            ModuleInfo.prototype.position = 0;
    
            /**
             * ModuleInfo address.
             * @member {number} address
             * @memberof fk_data.ModuleInfo
             * @instance
             */
            ModuleInfo.prototype.address = 0;
    
            /**
             * ModuleInfo name.
             * @member {string} name
             * @memberof fk_data.ModuleInfo
             * @instance
             */
            ModuleInfo.prototype.name = "";
    
            /**
             * ModuleInfo header.
             * @member {fk_data.IModuleHeader|null|undefined} header
             * @memberof fk_data.ModuleInfo
             * @instance
             */
            ModuleInfo.prototype.header = null;
    
            /**
             * ModuleInfo firmware.
             * @member {fk_data.IFirmware|null|undefined} firmware
             * @memberof fk_data.ModuleInfo
             * @instance
             */
            ModuleInfo.prototype.firmware = null;
    
            /**
             * ModuleInfo sensors.
             * @member {Array.<fk_data.ISensorInfo>} sensors
             * @memberof fk_data.ModuleInfo
             * @instance
             */
            ModuleInfo.prototype.sensors = $util.emptyArray;
    
            /**
             * ModuleInfo id.
             * @member {Uint8Array} id
             * @memberof fk_data.ModuleInfo
             * @instance
             */
            ModuleInfo.prototype.id = $util.newBuffer([]);
    
            /**
             * ModuleInfo flags.
             * @member {number} flags
             * @memberof fk_data.ModuleInfo
             * @instance
             */
            ModuleInfo.prototype.flags = 0;
    
            /**
             * ModuleInfo configuration.
             * @member {Uint8Array} configuration
             * @memberof fk_data.ModuleInfo
             * @instance
             */
            ModuleInfo.prototype.configuration = $util.newBuffer([]);
    
            /**
             * Creates a new ModuleInfo instance using the specified properties.
             * @function create
             * @memberof fk_data.ModuleInfo
             * @static
             * @param {fk_data.IModuleInfo=} [properties] Properties to set
             * @returns {fk_data.ModuleInfo} ModuleInfo instance
             */
            ModuleInfo.create = function create(properties) {
                return new ModuleInfo(properties);
            };
    
            /**
             * Encodes the specified ModuleInfo message. Does not implicitly {@link fk_data.ModuleInfo.verify|verify} messages.
             * @function encode
             * @memberof fk_data.ModuleInfo
             * @static
             * @param {fk_data.IModuleInfo} message ModuleInfo message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            ModuleInfo.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.position != null && message.hasOwnProperty("position"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.position);
                if (message.address != null && message.hasOwnProperty("address"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint32(message.address);
                if (message.name != null && message.hasOwnProperty("name"))
                    writer.uint32(/* id 3, wireType 2 =*/26).string(message.name);
                if (message.header != null && message.hasOwnProperty("header"))
                    $root.fk_data.ModuleHeader.encode(message.header, writer.uint32(/* id 4, wireType 2 =*/34).fork()).ldelim();
                if (message.firmware != null && message.hasOwnProperty("firmware"))
                    $root.fk_data.Firmware.encode(message.firmware, writer.uint32(/* id 5, wireType 2 =*/42).fork()).ldelim();
                if (message.sensors != null && message.sensors.length)
                    for (var i = 0; i < message.sensors.length; ++i)
                        $root.fk_data.SensorInfo.encode(message.sensors[i], writer.uint32(/* id 6, wireType 2 =*/50).fork()).ldelim();
                if (message.id != null && message.hasOwnProperty("id"))
                    writer.uint32(/* id 7, wireType 2 =*/58).bytes(message.id);
                if (message.flags != null && message.hasOwnProperty("flags"))
                    writer.uint32(/* id 8, wireType 0 =*/64).uint32(message.flags);
                if (message.configuration != null && message.hasOwnProperty("configuration"))
                    writer.uint32(/* id 9, wireType 2 =*/74).bytes(message.configuration);
                return writer;
            };
    
            /**
             * Encodes the specified ModuleInfo message, length delimited. Does not implicitly {@link fk_data.ModuleInfo.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.ModuleInfo
             * @static
             * @param {fk_data.IModuleInfo} message ModuleInfo message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            ModuleInfo.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a ModuleInfo message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.ModuleInfo
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.ModuleInfo} ModuleInfo
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            ModuleInfo.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.ModuleInfo();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.position = reader.uint32();
                        break;
                    case 2:
                        message.address = reader.uint32();
                        break;
                    case 3:
                        message.name = reader.string();
                        break;
                    case 4:
                        message.header = $root.fk_data.ModuleHeader.decode(reader, reader.uint32());
                        break;
                    case 5:
                        message.firmware = $root.fk_data.Firmware.decode(reader, reader.uint32());
                        break;
                    case 6:
                        if (!(message.sensors && message.sensors.length))
                            message.sensors = [];
                        message.sensors.push($root.fk_data.SensorInfo.decode(reader, reader.uint32()));
                        break;
                    case 7:
                        message.id = reader.bytes();
                        break;
                    case 8:
                        message.flags = reader.uint32();
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
             * Decodes a ModuleInfo message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_data.ModuleInfo
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.ModuleInfo} ModuleInfo
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            ModuleInfo.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a ModuleInfo message.
             * @function verify
             * @memberof fk_data.ModuleInfo
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            ModuleInfo.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.position != null && message.hasOwnProperty("position"))
                    if (!$util.isInteger(message.position))
                        return "position: integer expected";
                if (message.address != null && message.hasOwnProperty("address"))
                    if (!$util.isInteger(message.address))
                        return "address: integer expected";
                if (message.name != null && message.hasOwnProperty("name"))
                    if (!$util.isString(message.name))
                        return "name: string expected";
                if (message.header != null && message.hasOwnProperty("header")) {
                    var error = $root.fk_data.ModuleHeader.verify(message.header);
                    if (error)
                        return "header." + error;
                }
                if (message.firmware != null && message.hasOwnProperty("firmware")) {
                    var error = $root.fk_data.Firmware.verify(message.firmware);
                    if (error)
                        return "firmware." + error;
                }
                if (message.sensors != null && message.hasOwnProperty("sensors")) {
                    if (!Array.isArray(message.sensors))
                        return "sensors: array expected";
                    for (var i = 0; i < message.sensors.length; ++i) {
                        var error = $root.fk_data.SensorInfo.verify(message.sensors[i]);
                        if (error)
                            return "sensors." + error;
                    }
                }
                if (message.id != null && message.hasOwnProperty("id"))
                    if (!(message.id && typeof message.id.length === "number" || $util.isString(message.id)))
                        return "id: buffer expected";
                if (message.flags != null && message.hasOwnProperty("flags"))
                    if (!$util.isInteger(message.flags))
                        return "flags: integer expected";
                if (message.configuration != null && message.hasOwnProperty("configuration"))
                    if (!(message.configuration && typeof message.configuration.length === "number" || $util.isString(message.configuration)))
                        return "configuration: buffer expected";
                return null;
            };
    
            /**
             * Creates a ModuleInfo message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.ModuleInfo
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.ModuleInfo} ModuleInfo
             */
            ModuleInfo.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.ModuleInfo)
                    return object;
                var message = new $root.fk_data.ModuleInfo();
                if (object.position != null)
                    message.position = object.position >>> 0;
                if (object.address != null)
                    message.address = object.address >>> 0;
                if (object.name != null)
                    message.name = String(object.name);
                if (object.header != null) {
                    if (typeof object.header !== "object")
                        throw TypeError(".fk_data.ModuleInfo.header: object expected");
                    message.header = $root.fk_data.ModuleHeader.fromObject(object.header);
                }
                if (object.firmware != null) {
                    if (typeof object.firmware !== "object")
                        throw TypeError(".fk_data.ModuleInfo.firmware: object expected");
                    message.firmware = $root.fk_data.Firmware.fromObject(object.firmware);
                }
                if (object.sensors) {
                    if (!Array.isArray(object.sensors))
                        throw TypeError(".fk_data.ModuleInfo.sensors: array expected");
                    message.sensors = [];
                    for (var i = 0; i < object.sensors.length; ++i) {
                        if (typeof object.sensors[i] !== "object")
                            throw TypeError(".fk_data.ModuleInfo.sensors: object expected");
                        message.sensors[i] = $root.fk_data.SensorInfo.fromObject(object.sensors[i]);
                    }
                }
                if (object.id != null)
                    if (typeof object.id === "string")
                        $util.base64.decode(object.id, message.id = $util.newBuffer($util.base64.length(object.id)), 0);
                    else if (object.id.length)
                        message.id = object.id;
                if (object.flags != null)
                    message.flags = object.flags >>> 0;
                if (object.configuration != null)
                    if (typeof object.configuration === "string")
                        $util.base64.decode(object.configuration, message.configuration = $util.newBuffer($util.base64.length(object.configuration)), 0);
                    else if (object.configuration.length)
                        message.configuration = object.configuration;
                return message;
            };
    
            /**
             * Creates a plain object from a ModuleInfo message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.ModuleInfo
             * @static
             * @param {fk_data.ModuleInfo} message ModuleInfo
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            ModuleInfo.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults)
                    object.sensors = [];
                if (options.defaults) {
                    object.position = 0;
                    object.address = 0;
                    object.name = "";
                    object.header = null;
                    object.firmware = null;
                    object.id = options.bytes === String ? "" : [];
                    object.flags = 0;
                    object.configuration = options.bytes === String ? "" : [];
                }
                if (message.position != null && message.hasOwnProperty("position"))
                    object.position = message.position;
                if (message.address != null && message.hasOwnProperty("address"))
                    object.address = message.address;
                if (message.name != null && message.hasOwnProperty("name"))
                    object.name = message.name;
                if (message.header != null && message.hasOwnProperty("header"))
                    object.header = $root.fk_data.ModuleHeader.toObject(message.header, options);
                if (message.firmware != null && message.hasOwnProperty("firmware"))
                    object.firmware = $root.fk_data.Firmware.toObject(message.firmware, options);
                if (message.sensors && message.sensors.length) {
                    object.sensors = [];
                    for (var j = 0; j < message.sensors.length; ++j)
                        object.sensors[j] = $root.fk_data.SensorInfo.toObject(message.sensors[j], options);
                }
                if (message.id != null && message.hasOwnProperty("id"))
                    object.id = options.bytes === String ? $util.base64.encode(message.id, 0, message.id.length) : options.bytes === Array ? Array.prototype.slice.call(message.id) : message.id;
                if (message.flags != null && message.hasOwnProperty("flags"))
                    object.flags = message.flags;
                if (message.configuration != null && message.hasOwnProperty("configuration"))
                    object.configuration = options.bytes === String ? $util.base64.encode(message.configuration, 0, message.configuration.length) : options.bytes === Array ? Array.prototype.slice.call(message.configuration) : message.configuration;
                return object;
            };
    
            /**
             * Converts this ModuleInfo to JSON.
             * @function toJSON
             * @memberof fk_data.ModuleInfo
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            ModuleInfo.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return ModuleInfo;
        })();
    
        fk_data.SensorInfo = (function() {
    
            /**
             * Properties of a SensorInfo.
             * @memberof fk_data
             * @interface ISensorInfo
             * @property {number|null} [number] SensorInfo number
             * @property {string|null} [name] SensorInfo name
             * @property {string|null} [unitOfMeasure] SensorInfo unitOfMeasure
             * @property {string|null} [uncalibratedUnitOfMeasure] SensorInfo uncalibratedUnitOfMeasure
             * @property {number|null} [flags] SensorInfo flags
             */
    
            /**
             * Constructs a new SensorInfo.
             * @memberof fk_data
             * @classdesc Represents a SensorInfo.
             * @implements ISensorInfo
             * @constructor
             * @param {fk_data.ISensorInfo=} [properties] Properties to set
             */
            function SensorInfo(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * SensorInfo number.
             * @member {number} number
             * @memberof fk_data.SensorInfo
             * @instance
             */
            SensorInfo.prototype.number = 0;
    
            /**
             * SensorInfo name.
             * @member {string} name
             * @memberof fk_data.SensorInfo
             * @instance
             */
            SensorInfo.prototype.name = "";
    
            /**
             * SensorInfo unitOfMeasure.
             * @member {string} unitOfMeasure
             * @memberof fk_data.SensorInfo
             * @instance
             */
            SensorInfo.prototype.unitOfMeasure = "";
    
            /**
             * SensorInfo uncalibratedUnitOfMeasure.
             * @member {string} uncalibratedUnitOfMeasure
             * @memberof fk_data.SensorInfo
             * @instance
             */
            SensorInfo.prototype.uncalibratedUnitOfMeasure = "";
    
            /**
             * SensorInfo flags.
             * @member {number} flags
             * @memberof fk_data.SensorInfo
             * @instance
             */
            SensorInfo.prototype.flags = 0;
    
            /**
             * Creates a new SensorInfo instance using the specified properties.
             * @function create
             * @memberof fk_data.SensorInfo
             * @static
             * @param {fk_data.ISensorInfo=} [properties] Properties to set
             * @returns {fk_data.SensorInfo} SensorInfo instance
             */
            SensorInfo.create = function create(properties) {
                return new SensorInfo(properties);
            };
    
            /**
             * Encodes the specified SensorInfo message. Does not implicitly {@link fk_data.SensorInfo.verify|verify} messages.
             * @function encode
             * @memberof fk_data.SensorInfo
             * @static
             * @param {fk_data.ISensorInfo} message SensorInfo message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            SensorInfo.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.number != null && message.hasOwnProperty("number"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.number);
                if (message.name != null && message.hasOwnProperty("name"))
                    writer.uint32(/* id 2, wireType 2 =*/18).string(message.name);
                if (message.unitOfMeasure != null && message.hasOwnProperty("unitOfMeasure"))
                    writer.uint32(/* id 3, wireType 2 =*/26).string(message.unitOfMeasure);
                if (message.flags != null && message.hasOwnProperty("flags"))
                    writer.uint32(/* id 4, wireType 0 =*/32).uint32(message.flags);
                if (message.uncalibratedUnitOfMeasure != null && message.hasOwnProperty("uncalibratedUnitOfMeasure"))
                    writer.uint32(/* id 5, wireType 2 =*/42).string(message.uncalibratedUnitOfMeasure);
                return writer;
            };
    
            /**
             * Encodes the specified SensorInfo message, length delimited. Does not implicitly {@link fk_data.SensorInfo.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.SensorInfo
             * @static
             * @param {fk_data.ISensorInfo} message SensorInfo message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            SensorInfo.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a SensorInfo message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.SensorInfo
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.SensorInfo} SensorInfo
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            SensorInfo.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.SensorInfo();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.number = reader.uint32();
                        break;
                    case 2:
                        message.name = reader.string();
                        break;
                    case 3:
                        message.unitOfMeasure = reader.string();
                        break;
                    case 5:
                        message.uncalibratedUnitOfMeasure = reader.string();
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
             * Decodes a SensorInfo message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_data.SensorInfo
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.SensorInfo} SensorInfo
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            SensorInfo.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a SensorInfo message.
             * @function verify
             * @memberof fk_data.SensorInfo
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            SensorInfo.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.number != null && message.hasOwnProperty("number"))
                    if (!$util.isInteger(message.number))
                        return "number: integer expected";
                if (message.name != null && message.hasOwnProperty("name"))
                    if (!$util.isString(message.name))
                        return "name: string expected";
                if (message.unitOfMeasure != null && message.hasOwnProperty("unitOfMeasure"))
                    if (!$util.isString(message.unitOfMeasure))
                        return "unitOfMeasure: string expected";
                if (message.uncalibratedUnitOfMeasure != null && message.hasOwnProperty("uncalibratedUnitOfMeasure"))
                    if (!$util.isString(message.uncalibratedUnitOfMeasure))
                        return "uncalibratedUnitOfMeasure: string expected";
                if (message.flags != null && message.hasOwnProperty("flags"))
                    if (!$util.isInteger(message.flags))
                        return "flags: integer expected";
                return null;
            };
    
            /**
             * Creates a SensorInfo message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.SensorInfo
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.SensorInfo} SensorInfo
             */
            SensorInfo.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.SensorInfo)
                    return object;
                var message = new $root.fk_data.SensorInfo();
                if (object.number != null)
                    message.number = object.number >>> 0;
                if (object.name != null)
                    message.name = String(object.name);
                if (object.unitOfMeasure != null)
                    message.unitOfMeasure = String(object.unitOfMeasure);
                if (object.uncalibratedUnitOfMeasure != null)
                    message.uncalibratedUnitOfMeasure = String(object.uncalibratedUnitOfMeasure);
                if (object.flags != null)
                    message.flags = object.flags >>> 0;
                return message;
            };
    
            /**
             * Creates a plain object from a SensorInfo message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.SensorInfo
             * @static
             * @param {fk_data.SensorInfo} message SensorInfo
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            SensorInfo.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.number = 0;
                    object.name = "";
                    object.unitOfMeasure = "";
                    object.flags = 0;
                    object.uncalibratedUnitOfMeasure = "";
                }
                if (message.number != null && message.hasOwnProperty("number"))
                    object.number = message.number;
                if (message.name != null && message.hasOwnProperty("name"))
                    object.name = message.name;
                if (message.unitOfMeasure != null && message.hasOwnProperty("unitOfMeasure"))
                    object.unitOfMeasure = message.unitOfMeasure;
                if (message.flags != null && message.hasOwnProperty("flags"))
                    object.flags = message.flags;
                if (message.uncalibratedUnitOfMeasure != null && message.hasOwnProperty("uncalibratedUnitOfMeasure"))
                    object.uncalibratedUnitOfMeasure = message.uncalibratedUnitOfMeasure;
                return object;
            };
    
            /**
             * Converts this SensorInfo to JSON.
             * @function toJSON
             * @memberof fk_data.SensorInfo
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            SensorInfo.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return SensorInfo;
        })();
    
        fk_data.Firmware = (function() {
    
            /**
             * Properties of a Firmware.
             * @memberof fk_data
             * @interface IFirmware
             * @property {string|null} [version] Firmware version
             * @property {string|null} [build] Firmware build
             * @property {string|null} [number] Firmware number
             * @property {number|Long|null} [timestamp] Firmware timestamp
             * @property {string|null} [hash] Firmware hash
             */
    
            /**
             * Constructs a new Firmware.
             * @memberof fk_data
             * @classdesc Represents a Firmware.
             * @implements IFirmware
             * @constructor
             * @param {fk_data.IFirmware=} [properties] Properties to set
             */
            function Firmware(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Firmware version.
             * @member {string} version
             * @memberof fk_data.Firmware
             * @instance
             */
            Firmware.prototype.version = "";
    
            /**
             * Firmware build.
             * @member {string} build
             * @memberof fk_data.Firmware
             * @instance
             */
            Firmware.prototype.build = "";
    
            /**
             * Firmware number.
             * @member {string} number
             * @memberof fk_data.Firmware
             * @instance
             */
            Firmware.prototype.number = "";
    
            /**
             * Firmware timestamp.
             * @member {number|Long} timestamp
             * @memberof fk_data.Firmware
             * @instance
             */
            Firmware.prototype.timestamp = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * Firmware hash.
             * @member {string} hash
             * @memberof fk_data.Firmware
             * @instance
             */
            Firmware.prototype.hash = "";
    
            /**
             * Creates a new Firmware instance using the specified properties.
             * @function create
             * @memberof fk_data.Firmware
             * @static
             * @param {fk_data.IFirmware=} [properties] Properties to set
             * @returns {fk_data.Firmware} Firmware instance
             */
            Firmware.create = function create(properties) {
                return new Firmware(properties);
            };
    
            /**
             * Encodes the specified Firmware message. Does not implicitly {@link fk_data.Firmware.verify|verify} messages.
             * @function encode
             * @memberof fk_data.Firmware
             * @static
             * @param {fk_data.IFirmware} message Firmware message or plain object to encode
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
                return writer;
            };
    
            /**
             * Encodes the specified Firmware message, length delimited. Does not implicitly {@link fk_data.Firmware.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.Firmware
             * @static
             * @param {fk_data.IFirmware} message Firmware message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Firmware.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a Firmware message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.Firmware
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.Firmware} Firmware
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Firmware.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.Firmware();
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
             * @memberof fk_data.Firmware
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.Firmware} Firmware
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
             * @memberof fk_data.Firmware
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
                return null;
            };
    
            /**
             * Creates a Firmware message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.Firmware
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.Firmware} Firmware
             */
            Firmware.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.Firmware)
                    return object;
                var message = new $root.fk_data.Firmware();
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
                return message;
            };
    
            /**
             * Creates a plain object from a Firmware message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.Firmware
             * @static
             * @param {fk_data.Firmware} message Firmware
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
                return object;
            };
    
            /**
             * Converts this Firmware to JSON.
             * @function toJSON
             * @memberof fk_data.Firmware
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Firmware.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Firmware;
        })();
    
        fk_data.Metadata = (function() {
    
            /**
             * Properties of a Metadata.
             * @memberof fk_data
             * @interface IMetadata
             * @property {Uint8Array|null} [deviceId] Metadata deviceId
             * @property {number|Long|null} [time] Metadata time
             * @property {string|null} [git] Metadata git
             * @property {string|null} [build] Metadata build
             * @property {number|null} [resetCause] Metadata resetCause
             * @property {Array.<fk_data.ISensorInfo>|null} [sensors] Metadata sensors
             * @property {Array.<fk_data.IModuleInfo>|null} [modules] Metadata modules
             * @property {fk_data.IFirmware|null} [firmware] Metadata firmware
             * @property {Uint8Array|null} [generation] Metadata generation
             * @property {number|Long|null} [record] Metadata record
             */
    
            /**
             * Constructs a new Metadata.
             * @memberof fk_data
             * @classdesc Represents a Metadata.
             * @implements IMetadata
             * @constructor
             * @param {fk_data.IMetadata=} [properties] Properties to set
             */
            function Metadata(properties) {
                this.sensors = [];
                this.modules = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Metadata deviceId.
             * @member {Uint8Array} deviceId
             * @memberof fk_data.Metadata
             * @instance
             */
            Metadata.prototype.deviceId = $util.newBuffer([]);
    
            /**
             * Metadata time.
             * @member {number|Long} time
             * @memberof fk_data.Metadata
             * @instance
             */
            Metadata.prototype.time = $util.Long ? $util.Long.fromBits(0,0,false) : 0;
    
            /**
             * Metadata git.
             * @member {string} git
             * @memberof fk_data.Metadata
             * @instance
             */
            Metadata.prototype.git = "";
    
            /**
             * Metadata build.
             * @member {string} build
             * @memberof fk_data.Metadata
             * @instance
             */
            Metadata.prototype.build = "";
    
            /**
             * Metadata resetCause.
             * @member {number} resetCause
             * @memberof fk_data.Metadata
             * @instance
             */
            Metadata.prototype.resetCause = 0;
    
            /**
             * Metadata sensors.
             * @member {Array.<fk_data.ISensorInfo>} sensors
             * @memberof fk_data.Metadata
             * @instance
             */
            Metadata.prototype.sensors = $util.emptyArray;
    
            /**
             * Metadata modules.
             * @member {Array.<fk_data.IModuleInfo>} modules
             * @memberof fk_data.Metadata
             * @instance
             */
            Metadata.prototype.modules = $util.emptyArray;
    
            /**
             * Metadata firmware.
             * @member {fk_data.IFirmware|null|undefined} firmware
             * @memberof fk_data.Metadata
             * @instance
             */
            Metadata.prototype.firmware = null;
    
            /**
             * Metadata generation.
             * @member {Uint8Array} generation
             * @memberof fk_data.Metadata
             * @instance
             */
            Metadata.prototype.generation = $util.newBuffer([]);
    
            /**
             * Metadata record.
             * @member {number|Long} record
             * @memberof fk_data.Metadata
             * @instance
             */
            Metadata.prototype.record = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * Creates a new Metadata instance using the specified properties.
             * @function create
             * @memberof fk_data.Metadata
             * @static
             * @param {fk_data.IMetadata=} [properties] Properties to set
             * @returns {fk_data.Metadata} Metadata instance
             */
            Metadata.create = function create(properties) {
                return new Metadata(properties);
            };
    
            /**
             * Encodes the specified Metadata message. Does not implicitly {@link fk_data.Metadata.verify|verify} messages.
             * @function encode
             * @memberof fk_data.Metadata
             * @static
             * @param {fk_data.IMetadata} message Metadata message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Metadata.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.deviceId != null && message.hasOwnProperty("deviceId"))
                    writer.uint32(/* id 1, wireType 2 =*/10).bytes(message.deviceId);
                if (message.time != null && message.hasOwnProperty("time"))
                    writer.uint32(/* id 2, wireType 0 =*/16).int64(message.time);
                if (message.git != null && message.hasOwnProperty("git"))
                    writer.uint32(/* id 3, wireType 2 =*/26).string(message.git);
                if (message.resetCause != null && message.hasOwnProperty("resetCause"))
                    writer.uint32(/* id 4, wireType 0 =*/32).uint32(message.resetCause);
                if (message.sensors != null && message.sensors.length)
                    for (var i = 0; i < message.sensors.length; ++i)
                        $root.fk_data.SensorInfo.encode(message.sensors[i], writer.uint32(/* id 5, wireType 2 =*/42).fork()).ldelim();
                if (message.modules != null && message.modules.length)
                    for (var i = 0; i < message.modules.length; ++i)
                        $root.fk_data.ModuleInfo.encode(message.modules[i], writer.uint32(/* id 6, wireType 2 =*/50).fork()).ldelim();
                if (message.build != null && message.hasOwnProperty("build"))
                    writer.uint32(/* id 7, wireType 2 =*/58).string(message.build);
                if (message.firmware != null && message.hasOwnProperty("firmware"))
                    $root.fk_data.Firmware.encode(message.firmware, writer.uint32(/* id 8, wireType 2 =*/66).fork()).ldelim();
                if (message.generation != null && message.hasOwnProperty("generation"))
                    writer.uint32(/* id 9, wireType 2 =*/74).bytes(message.generation);
                if (message.record != null && message.hasOwnProperty("record"))
                    writer.uint32(/* id 10, wireType 0 =*/80).uint64(message.record);
                return writer;
            };
    
            /**
             * Encodes the specified Metadata message, length delimited. Does not implicitly {@link fk_data.Metadata.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.Metadata
             * @static
             * @param {fk_data.IMetadata} message Metadata message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Metadata.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a Metadata message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.Metadata
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.Metadata} Metadata
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Metadata.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.Metadata();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.deviceId = reader.bytes();
                        break;
                    case 2:
                        message.time = reader.int64();
                        break;
                    case 3:
                        message.git = reader.string();
                        break;
                    case 7:
                        message.build = reader.string();
                        break;
                    case 4:
                        message.resetCause = reader.uint32();
                        break;
                    case 5:
                        if (!(message.sensors && message.sensors.length))
                            message.sensors = [];
                        message.sensors.push($root.fk_data.SensorInfo.decode(reader, reader.uint32()));
                        break;
                    case 6:
                        if (!(message.modules && message.modules.length))
                            message.modules = [];
                        message.modules.push($root.fk_data.ModuleInfo.decode(reader, reader.uint32()));
                        break;
                    case 8:
                        message.firmware = $root.fk_data.Firmware.decode(reader, reader.uint32());
                        break;
                    case 9:
                        message.generation = reader.bytes();
                        break;
                    case 10:
                        message.record = reader.uint64();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a Metadata message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_data.Metadata
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.Metadata} Metadata
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Metadata.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a Metadata message.
             * @function verify
             * @memberof fk_data.Metadata
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            Metadata.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.deviceId != null && message.hasOwnProperty("deviceId"))
                    if (!(message.deviceId && typeof message.deviceId.length === "number" || $util.isString(message.deviceId)))
                        return "deviceId: buffer expected";
                if (message.time != null && message.hasOwnProperty("time"))
                    if (!$util.isInteger(message.time) && !(message.time && $util.isInteger(message.time.low) && $util.isInteger(message.time.high)))
                        return "time: integer|Long expected";
                if (message.git != null && message.hasOwnProperty("git"))
                    if (!$util.isString(message.git))
                        return "git: string expected";
                if (message.build != null && message.hasOwnProperty("build"))
                    if (!$util.isString(message.build))
                        return "build: string expected";
                if (message.resetCause != null && message.hasOwnProperty("resetCause"))
                    if (!$util.isInteger(message.resetCause))
                        return "resetCause: integer expected";
                if (message.sensors != null && message.hasOwnProperty("sensors")) {
                    if (!Array.isArray(message.sensors))
                        return "sensors: array expected";
                    for (var i = 0; i < message.sensors.length; ++i) {
                        var error = $root.fk_data.SensorInfo.verify(message.sensors[i]);
                        if (error)
                            return "sensors." + error;
                    }
                }
                if (message.modules != null && message.hasOwnProperty("modules")) {
                    if (!Array.isArray(message.modules))
                        return "modules: array expected";
                    for (var i = 0; i < message.modules.length; ++i) {
                        var error = $root.fk_data.ModuleInfo.verify(message.modules[i]);
                        if (error)
                            return "modules." + error;
                    }
                }
                if (message.firmware != null && message.hasOwnProperty("firmware")) {
                    var error = $root.fk_data.Firmware.verify(message.firmware);
                    if (error)
                        return "firmware." + error;
                }
                if (message.generation != null && message.hasOwnProperty("generation"))
                    if (!(message.generation && typeof message.generation.length === "number" || $util.isString(message.generation)))
                        return "generation: buffer expected";
                if (message.record != null && message.hasOwnProperty("record"))
                    if (!$util.isInteger(message.record) && !(message.record && $util.isInteger(message.record.low) && $util.isInteger(message.record.high)))
                        return "record: integer|Long expected";
                return null;
            };
    
            /**
             * Creates a Metadata message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.Metadata
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.Metadata} Metadata
             */
            Metadata.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.Metadata)
                    return object;
                var message = new $root.fk_data.Metadata();
                if (object.deviceId != null)
                    if (typeof object.deviceId === "string")
                        $util.base64.decode(object.deviceId, message.deviceId = $util.newBuffer($util.base64.length(object.deviceId)), 0);
                    else if (object.deviceId.length)
                        message.deviceId = object.deviceId;
                if (object.time != null)
                    if ($util.Long)
                        (message.time = $util.Long.fromValue(object.time)).unsigned = false;
                    else if (typeof object.time === "string")
                        message.time = parseInt(object.time, 10);
                    else if (typeof object.time === "number")
                        message.time = object.time;
                    else if (typeof object.time === "object")
                        message.time = new $util.LongBits(object.time.low >>> 0, object.time.high >>> 0).toNumber();
                if (object.git != null)
                    message.git = String(object.git);
                if (object.build != null)
                    message.build = String(object.build);
                if (object.resetCause != null)
                    message.resetCause = object.resetCause >>> 0;
                if (object.sensors) {
                    if (!Array.isArray(object.sensors))
                        throw TypeError(".fk_data.Metadata.sensors: array expected");
                    message.sensors = [];
                    for (var i = 0; i < object.sensors.length; ++i) {
                        if (typeof object.sensors[i] !== "object")
                            throw TypeError(".fk_data.Metadata.sensors: object expected");
                        message.sensors[i] = $root.fk_data.SensorInfo.fromObject(object.sensors[i]);
                    }
                }
                if (object.modules) {
                    if (!Array.isArray(object.modules))
                        throw TypeError(".fk_data.Metadata.modules: array expected");
                    message.modules = [];
                    for (var i = 0; i < object.modules.length; ++i) {
                        if (typeof object.modules[i] !== "object")
                            throw TypeError(".fk_data.Metadata.modules: object expected");
                        message.modules[i] = $root.fk_data.ModuleInfo.fromObject(object.modules[i]);
                    }
                }
                if (object.firmware != null) {
                    if (typeof object.firmware !== "object")
                        throw TypeError(".fk_data.Metadata.firmware: object expected");
                    message.firmware = $root.fk_data.Firmware.fromObject(object.firmware);
                }
                if (object.generation != null)
                    if (typeof object.generation === "string")
                        $util.base64.decode(object.generation, message.generation = $util.newBuffer($util.base64.length(object.generation)), 0);
                    else if (object.generation.length)
                        message.generation = object.generation;
                if (object.record != null)
                    if ($util.Long)
                        (message.record = $util.Long.fromValue(object.record)).unsigned = true;
                    else if (typeof object.record === "string")
                        message.record = parseInt(object.record, 10);
                    else if (typeof object.record === "number")
                        message.record = object.record;
                    else if (typeof object.record === "object")
                        message.record = new $util.LongBits(object.record.low >>> 0, object.record.high >>> 0).toNumber(true);
                return message;
            };
    
            /**
             * Creates a plain object from a Metadata message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.Metadata
             * @static
             * @param {fk_data.Metadata} message Metadata
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            Metadata.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults) {
                    object.sensors = [];
                    object.modules = [];
                }
                if (options.defaults) {
                    object.deviceId = options.bytes === String ? "" : [];
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, false);
                        object.time = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.time = options.longs === String ? "0" : 0;
                    object.git = "";
                    object.resetCause = 0;
                    object.build = "";
                    object.firmware = null;
                    object.generation = options.bytes === String ? "" : [];
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.record = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.record = options.longs === String ? "0" : 0;
                }
                if (message.deviceId != null && message.hasOwnProperty("deviceId"))
                    object.deviceId = options.bytes === String ? $util.base64.encode(message.deviceId, 0, message.deviceId.length) : options.bytes === Array ? Array.prototype.slice.call(message.deviceId) : message.deviceId;
                if (message.time != null && message.hasOwnProperty("time"))
                    if (typeof message.time === "number")
                        object.time = options.longs === String ? String(message.time) : message.time;
                    else
                        object.time = options.longs === String ? $util.Long.prototype.toString.call(message.time) : options.longs === Number ? new $util.LongBits(message.time.low >>> 0, message.time.high >>> 0).toNumber() : message.time;
                if (message.git != null && message.hasOwnProperty("git"))
                    object.git = message.git;
                if (message.resetCause != null && message.hasOwnProperty("resetCause"))
                    object.resetCause = message.resetCause;
                if (message.sensors && message.sensors.length) {
                    object.sensors = [];
                    for (var j = 0; j < message.sensors.length; ++j)
                        object.sensors[j] = $root.fk_data.SensorInfo.toObject(message.sensors[j], options);
                }
                if (message.modules && message.modules.length) {
                    object.modules = [];
                    for (var j = 0; j < message.modules.length; ++j)
                        object.modules[j] = $root.fk_data.ModuleInfo.toObject(message.modules[j], options);
                }
                if (message.build != null && message.hasOwnProperty("build"))
                    object.build = message.build;
                if (message.firmware != null && message.hasOwnProperty("firmware"))
                    object.firmware = $root.fk_data.Firmware.toObject(message.firmware, options);
                if (message.generation != null && message.hasOwnProperty("generation"))
                    object.generation = options.bytes === String ? $util.base64.encode(message.generation, 0, message.generation.length) : options.bytes === Array ? Array.prototype.slice.call(message.generation) : message.generation;
                if (message.record != null && message.hasOwnProperty("record"))
                    if (typeof message.record === "number")
                        object.record = options.longs === String ? String(message.record) : message.record;
                    else
                        object.record = options.longs === String ? $util.Long.prototype.toString.call(message.record) : options.longs === Number ? new $util.LongBits(message.record.low >>> 0, message.record.high >>> 0).toNumber(true) : message.record;
                return object;
            };
    
            /**
             * Converts this Metadata to JSON.
             * @function toJSON
             * @memberof fk_data.Metadata
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Metadata.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Metadata;
        })();
    
        fk_data.Status = (function() {
    
            /**
             * Properties of a Status.
             * @memberof fk_data
             * @interface IStatus
             * @property {number|Long|null} [time] Status time
             * @property {number|null} [uptime] Status uptime
             * @property {number|null} [battery] Status battery
             * @property {number|null} [memory] Status memory
             * @property {number|Long|null} [busy] Status busy
             */
    
            /**
             * Constructs a new Status.
             * @memberof fk_data
             * @classdesc Represents a Status.
             * @implements IStatus
             * @constructor
             * @param {fk_data.IStatus=} [properties] Properties to set
             */
            function Status(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Status time.
             * @member {number|Long} time
             * @memberof fk_data.Status
             * @instance
             */
            Status.prototype.time = $util.Long ? $util.Long.fromBits(0,0,false) : 0;
    
            /**
             * Status uptime.
             * @member {number} uptime
             * @memberof fk_data.Status
             * @instance
             */
            Status.prototype.uptime = 0;
    
            /**
             * Status battery.
             * @member {number} battery
             * @memberof fk_data.Status
             * @instance
             */
            Status.prototype.battery = 0;
    
            /**
             * Status memory.
             * @member {number} memory
             * @memberof fk_data.Status
             * @instance
             */
            Status.prototype.memory = 0;
    
            /**
             * Status busy.
             * @member {number|Long} busy
             * @memberof fk_data.Status
             * @instance
             */
            Status.prototype.busy = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * Creates a new Status instance using the specified properties.
             * @function create
             * @memberof fk_data.Status
             * @static
             * @param {fk_data.IStatus=} [properties] Properties to set
             * @returns {fk_data.Status} Status instance
             */
            Status.create = function create(properties) {
                return new Status(properties);
            };
    
            /**
             * Encodes the specified Status message. Does not implicitly {@link fk_data.Status.verify|verify} messages.
             * @function encode
             * @memberof fk_data.Status
             * @static
             * @param {fk_data.IStatus} message Status message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Status.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.time != null && message.hasOwnProperty("time"))
                    writer.uint32(/* id 1, wireType 0 =*/8).int64(message.time);
                if (message.uptime != null && message.hasOwnProperty("uptime"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint32(message.uptime);
                if (message.battery != null && message.hasOwnProperty("battery"))
                    writer.uint32(/* id 3, wireType 5 =*/29).float(message.battery);
                if (message.memory != null && message.hasOwnProperty("memory"))
                    writer.uint32(/* id 4, wireType 0 =*/32).uint32(message.memory);
                if (message.busy != null && message.hasOwnProperty("busy"))
                    writer.uint32(/* id 5, wireType 0 =*/40).uint64(message.busy);
                return writer;
            };
    
            /**
             * Encodes the specified Status message, length delimited. Does not implicitly {@link fk_data.Status.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.Status
             * @static
             * @param {fk_data.IStatus} message Status message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Status.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a Status message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.Status
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.Status} Status
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Status.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.Status();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.time = reader.int64();
                        break;
                    case 2:
                        message.uptime = reader.uint32();
                        break;
                    case 3:
                        message.battery = reader.float();
                        break;
                    case 4:
                        message.memory = reader.uint32();
                        break;
                    case 5:
                        message.busy = reader.uint64();
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
             * @memberof fk_data.Status
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.Status} Status
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
             * @memberof fk_data.Status
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            Status.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.time != null && message.hasOwnProperty("time"))
                    if (!$util.isInteger(message.time) && !(message.time && $util.isInteger(message.time.low) && $util.isInteger(message.time.high)))
                        return "time: integer|Long expected";
                if (message.uptime != null && message.hasOwnProperty("uptime"))
                    if (!$util.isInteger(message.uptime))
                        return "uptime: integer expected";
                if (message.battery != null && message.hasOwnProperty("battery"))
                    if (typeof message.battery !== "number")
                        return "battery: number expected";
                if (message.memory != null && message.hasOwnProperty("memory"))
                    if (!$util.isInteger(message.memory))
                        return "memory: integer expected";
                if (message.busy != null && message.hasOwnProperty("busy"))
                    if (!$util.isInteger(message.busy) && !(message.busy && $util.isInteger(message.busy.low) && $util.isInteger(message.busy.high)))
                        return "busy: integer|Long expected";
                return null;
            };
    
            /**
             * Creates a Status message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.Status
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.Status} Status
             */
            Status.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.Status)
                    return object;
                var message = new $root.fk_data.Status();
                if (object.time != null)
                    if ($util.Long)
                        (message.time = $util.Long.fromValue(object.time)).unsigned = false;
                    else if (typeof object.time === "string")
                        message.time = parseInt(object.time, 10);
                    else if (typeof object.time === "number")
                        message.time = object.time;
                    else if (typeof object.time === "object")
                        message.time = new $util.LongBits(object.time.low >>> 0, object.time.high >>> 0).toNumber();
                if (object.uptime != null)
                    message.uptime = object.uptime >>> 0;
                if (object.battery != null)
                    message.battery = Number(object.battery);
                if (object.memory != null)
                    message.memory = object.memory >>> 0;
                if (object.busy != null)
                    if ($util.Long)
                        (message.busy = $util.Long.fromValue(object.busy)).unsigned = true;
                    else if (typeof object.busy === "string")
                        message.busy = parseInt(object.busy, 10);
                    else if (typeof object.busy === "number")
                        message.busy = object.busy;
                    else if (typeof object.busy === "object")
                        message.busy = new $util.LongBits(object.busy.low >>> 0, object.busy.high >>> 0).toNumber(true);
                return message;
            };
    
            /**
             * Creates a plain object from a Status message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.Status
             * @static
             * @param {fk_data.Status} message Status
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            Status.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, false);
                        object.time = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.time = options.longs === String ? "0" : 0;
                    object.uptime = 0;
                    object.battery = 0;
                    object.memory = 0;
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.busy = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.busy = options.longs === String ? "0" : 0;
                }
                if (message.time != null && message.hasOwnProperty("time"))
                    if (typeof message.time === "number")
                        object.time = options.longs === String ? String(message.time) : message.time;
                    else
                        object.time = options.longs === String ? $util.Long.prototype.toString.call(message.time) : options.longs === Number ? new $util.LongBits(message.time.low >>> 0, message.time.high >>> 0).toNumber() : message.time;
                if (message.uptime != null && message.hasOwnProperty("uptime"))
                    object.uptime = message.uptime;
                if (message.battery != null && message.hasOwnProperty("battery"))
                    object.battery = options.json && !isFinite(message.battery) ? String(message.battery) : message.battery;
                if (message.memory != null && message.hasOwnProperty("memory"))
                    object.memory = message.memory;
                if (message.busy != null && message.hasOwnProperty("busy"))
                    if (typeof message.busy === "number")
                        object.busy = options.longs === String ? String(message.busy) : message.busy;
                    else
                        object.busy = options.longs === String ? $util.Long.prototype.toString.call(message.busy) : options.longs === Number ? new $util.LongBits(message.busy.low >>> 0, message.busy.high >>> 0).toNumber(true) : message.busy;
                return object;
            };
    
            /**
             * Converts this Status to JSON.
             * @function toJSON
             * @memberof fk_data.Status
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Status.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Status;
        })();
    
        fk_data.LogMessage = (function() {
    
            /**
             * Properties of a LogMessage.
             * @memberof fk_data
             * @interface ILogMessage
             * @property {number|Long|null} [time] LogMessage time
             * @property {number|null} [uptime] LogMessage uptime
             * @property {number|null} [level] LogMessage level
             * @property {string|null} [facility] LogMessage facility
             * @property {string|null} [message] LogMessage message
             */
    
            /**
             * Constructs a new LogMessage.
             * @memberof fk_data
             * @classdesc Represents a LogMessage.
             * @implements ILogMessage
             * @constructor
             * @param {fk_data.ILogMessage=} [properties] Properties to set
             */
            function LogMessage(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * LogMessage time.
             * @member {number|Long} time
             * @memberof fk_data.LogMessage
             * @instance
             */
            LogMessage.prototype.time = $util.Long ? $util.Long.fromBits(0,0,false) : 0;
    
            /**
             * LogMessage uptime.
             * @member {number} uptime
             * @memberof fk_data.LogMessage
             * @instance
             */
            LogMessage.prototype.uptime = 0;
    
            /**
             * LogMessage level.
             * @member {number} level
             * @memberof fk_data.LogMessage
             * @instance
             */
            LogMessage.prototype.level = 0;
    
            /**
             * LogMessage facility.
             * @member {string} facility
             * @memberof fk_data.LogMessage
             * @instance
             */
            LogMessage.prototype.facility = "";
    
            /**
             * LogMessage message.
             * @member {string} message
             * @memberof fk_data.LogMessage
             * @instance
             */
            LogMessage.prototype.message = "";
    
            /**
             * Creates a new LogMessage instance using the specified properties.
             * @function create
             * @memberof fk_data.LogMessage
             * @static
             * @param {fk_data.ILogMessage=} [properties] Properties to set
             * @returns {fk_data.LogMessage} LogMessage instance
             */
            LogMessage.create = function create(properties) {
                return new LogMessage(properties);
            };
    
            /**
             * Encodes the specified LogMessage message. Does not implicitly {@link fk_data.LogMessage.verify|verify} messages.
             * @function encode
             * @memberof fk_data.LogMessage
             * @static
             * @param {fk_data.ILogMessage} message LogMessage message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            LogMessage.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.time != null && message.hasOwnProperty("time"))
                    writer.uint32(/* id 1, wireType 0 =*/8).int64(message.time);
                if (message.uptime != null && message.hasOwnProperty("uptime"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint32(message.uptime);
                if (message.level != null && message.hasOwnProperty("level"))
                    writer.uint32(/* id 3, wireType 0 =*/24).uint32(message.level);
                if (message.facility != null && message.hasOwnProperty("facility"))
                    writer.uint32(/* id 4, wireType 2 =*/34).string(message.facility);
                if (message.message != null && message.hasOwnProperty("message"))
                    writer.uint32(/* id 5, wireType 2 =*/42).string(message.message);
                return writer;
            };
    
            /**
             * Encodes the specified LogMessage message, length delimited. Does not implicitly {@link fk_data.LogMessage.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.LogMessage
             * @static
             * @param {fk_data.ILogMessage} message LogMessage message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            LogMessage.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a LogMessage message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.LogMessage
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.LogMessage} LogMessage
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            LogMessage.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.LogMessage();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.time = reader.int64();
                        break;
                    case 2:
                        message.uptime = reader.uint32();
                        break;
                    case 3:
                        message.level = reader.uint32();
                        break;
                    case 4:
                        message.facility = reader.string();
                        break;
                    case 5:
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
             * Decodes a LogMessage message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_data.LogMessage
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.LogMessage} LogMessage
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            LogMessage.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a LogMessage message.
             * @function verify
             * @memberof fk_data.LogMessage
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            LogMessage.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.time != null && message.hasOwnProperty("time"))
                    if (!$util.isInteger(message.time) && !(message.time && $util.isInteger(message.time.low) && $util.isInteger(message.time.high)))
                        return "time: integer|Long expected";
                if (message.uptime != null && message.hasOwnProperty("uptime"))
                    if (!$util.isInteger(message.uptime))
                        return "uptime: integer expected";
                if (message.level != null && message.hasOwnProperty("level"))
                    if (!$util.isInteger(message.level))
                        return "level: integer expected";
                if (message.facility != null && message.hasOwnProperty("facility"))
                    if (!$util.isString(message.facility))
                        return "facility: string expected";
                if (message.message != null && message.hasOwnProperty("message"))
                    if (!$util.isString(message.message))
                        return "message: string expected";
                return null;
            };
    
            /**
             * Creates a LogMessage message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.LogMessage
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.LogMessage} LogMessage
             */
            LogMessage.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.LogMessage)
                    return object;
                var message = new $root.fk_data.LogMessage();
                if (object.time != null)
                    if ($util.Long)
                        (message.time = $util.Long.fromValue(object.time)).unsigned = false;
                    else if (typeof object.time === "string")
                        message.time = parseInt(object.time, 10);
                    else if (typeof object.time === "number")
                        message.time = object.time;
                    else if (typeof object.time === "object")
                        message.time = new $util.LongBits(object.time.low >>> 0, object.time.high >>> 0).toNumber();
                if (object.uptime != null)
                    message.uptime = object.uptime >>> 0;
                if (object.level != null)
                    message.level = object.level >>> 0;
                if (object.facility != null)
                    message.facility = String(object.facility);
                if (object.message != null)
                    message.message = String(object.message);
                return message;
            };
    
            /**
             * Creates a plain object from a LogMessage message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.LogMessage
             * @static
             * @param {fk_data.LogMessage} message LogMessage
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            LogMessage.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, false);
                        object.time = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.time = options.longs === String ? "0" : 0;
                    object.uptime = 0;
                    object.level = 0;
                    object.facility = "";
                    object.message = "";
                }
                if (message.time != null && message.hasOwnProperty("time"))
                    if (typeof message.time === "number")
                        object.time = options.longs === String ? String(message.time) : message.time;
                    else
                        object.time = options.longs === String ? $util.Long.prototype.toString.call(message.time) : options.longs === Number ? new $util.LongBits(message.time.low >>> 0, message.time.high >>> 0).toNumber() : message.time;
                if (message.uptime != null && message.hasOwnProperty("uptime"))
                    object.uptime = message.uptime;
                if (message.level != null && message.hasOwnProperty("level"))
                    object.level = message.level;
                if (message.facility != null && message.hasOwnProperty("facility"))
                    object.facility = message.facility;
                if (message.message != null && message.hasOwnProperty("message"))
                    object.message = message.message;
                return object;
            };
    
            /**
             * Converts this LogMessage to JSON.
             * @function toJSON
             * @memberof fk_data.LogMessage
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            LogMessage.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return LogMessage;
        })();
    
        /**
         * DownloadFlags enum.
         * @name fk_data.DownloadFlags
         * @enum {string}
         * @property {number} READING_FLAGS_NONE=0 READING_FLAGS_NONE value
         * @property {number} READING_FLAGS_NOT_RECORDING=1 READING_FLAGS_NOT_RECORDING value
         * @property {number} READING_FLAGS_MANUAL=2 READING_FLAGS_MANUAL value
         */
        fk_data.DownloadFlags = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "READING_FLAGS_NONE"] = 0;
            values[valuesById[1] = "READING_FLAGS_NOT_RECORDING"] = 1;
            values[valuesById[2] = "READING_FLAGS_MANUAL"] = 2;
            return values;
        })();
    
        fk_data.SensorGroup = (function() {
    
            /**
             * Properties of a SensorGroup.
             * @memberof fk_data
             * @interface ISensorGroup
             * @property {number|null} [module] SensorGroup module
             * @property {number|Long|null} [time] SensorGroup time
             * @property {Array.<fk_data.ISensorAndValue>|null} [readings] SensorGroup readings
             */
    
            /**
             * Constructs a new SensorGroup.
             * @memberof fk_data
             * @classdesc Represents a SensorGroup.
             * @implements ISensorGroup
             * @constructor
             * @param {fk_data.ISensorGroup=} [properties] Properties to set
             */
            function SensorGroup(properties) {
                this.readings = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * SensorGroup module.
             * @member {number} module
             * @memberof fk_data.SensorGroup
             * @instance
             */
            SensorGroup.prototype.module = 0;
    
            /**
             * SensorGroup time.
             * @member {number|Long} time
             * @memberof fk_data.SensorGroup
             * @instance
             */
            SensorGroup.prototype.time = $util.Long ? $util.Long.fromBits(0,0,false) : 0;
    
            /**
             * SensorGroup readings.
             * @member {Array.<fk_data.ISensorAndValue>} readings
             * @memberof fk_data.SensorGroup
             * @instance
             */
            SensorGroup.prototype.readings = $util.emptyArray;
    
            /**
             * Creates a new SensorGroup instance using the specified properties.
             * @function create
             * @memberof fk_data.SensorGroup
             * @static
             * @param {fk_data.ISensorGroup=} [properties] Properties to set
             * @returns {fk_data.SensorGroup} SensorGroup instance
             */
            SensorGroup.create = function create(properties) {
                return new SensorGroup(properties);
            };
    
            /**
             * Encodes the specified SensorGroup message. Does not implicitly {@link fk_data.SensorGroup.verify|verify} messages.
             * @function encode
             * @memberof fk_data.SensorGroup
             * @static
             * @param {fk_data.ISensorGroup} message SensorGroup message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            SensorGroup.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.module != null && message.hasOwnProperty("module"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.module);
                if (message.readings != null && message.readings.length)
                    for (var i = 0; i < message.readings.length; ++i)
                        $root.fk_data.SensorAndValue.encode(message.readings[i], writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
                if (message.time != null && message.hasOwnProperty("time"))
                    writer.uint32(/* id 3, wireType 0 =*/24).int64(message.time);
                return writer;
            };
    
            /**
             * Encodes the specified SensorGroup message, length delimited. Does not implicitly {@link fk_data.SensorGroup.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.SensorGroup
             * @static
             * @param {fk_data.ISensorGroup} message SensorGroup message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            SensorGroup.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a SensorGroup message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.SensorGroup
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.SensorGroup} SensorGroup
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            SensorGroup.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.SensorGroup();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.module = reader.uint32();
                        break;
                    case 3:
                        message.time = reader.int64();
                        break;
                    case 2:
                        if (!(message.readings && message.readings.length))
                            message.readings = [];
                        message.readings.push($root.fk_data.SensorAndValue.decode(reader, reader.uint32()));
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a SensorGroup message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_data.SensorGroup
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.SensorGroup} SensorGroup
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            SensorGroup.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a SensorGroup message.
             * @function verify
             * @memberof fk_data.SensorGroup
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            SensorGroup.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.module != null && message.hasOwnProperty("module"))
                    if (!$util.isInteger(message.module))
                        return "module: integer expected";
                if (message.time != null && message.hasOwnProperty("time"))
                    if (!$util.isInteger(message.time) && !(message.time && $util.isInteger(message.time.low) && $util.isInteger(message.time.high)))
                        return "time: integer|Long expected";
                if (message.readings != null && message.hasOwnProperty("readings")) {
                    if (!Array.isArray(message.readings))
                        return "readings: array expected";
                    for (var i = 0; i < message.readings.length; ++i) {
                        var error = $root.fk_data.SensorAndValue.verify(message.readings[i]);
                        if (error)
                            return "readings." + error;
                    }
                }
                return null;
            };
    
            /**
             * Creates a SensorGroup message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.SensorGroup
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.SensorGroup} SensorGroup
             */
            SensorGroup.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.SensorGroup)
                    return object;
                var message = new $root.fk_data.SensorGroup();
                if (object.module != null)
                    message.module = object.module >>> 0;
                if (object.time != null)
                    if ($util.Long)
                        (message.time = $util.Long.fromValue(object.time)).unsigned = false;
                    else if (typeof object.time === "string")
                        message.time = parseInt(object.time, 10);
                    else if (typeof object.time === "number")
                        message.time = object.time;
                    else if (typeof object.time === "object")
                        message.time = new $util.LongBits(object.time.low >>> 0, object.time.high >>> 0).toNumber();
                if (object.readings) {
                    if (!Array.isArray(object.readings))
                        throw TypeError(".fk_data.SensorGroup.readings: array expected");
                    message.readings = [];
                    for (var i = 0; i < object.readings.length; ++i) {
                        if (typeof object.readings[i] !== "object")
                            throw TypeError(".fk_data.SensorGroup.readings: object expected");
                        message.readings[i] = $root.fk_data.SensorAndValue.fromObject(object.readings[i]);
                    }
                }
                return message;
            };
    
            /**
             * Creates a plain object from a SensorGroup message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.SensorGroup
             * @static
             * @param {fk_data.SensorGroup} message SensorGroup
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            SensorGroup.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults)
                    object.readings = [];
                if (options.defaults) {
                    object.module = 0;
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, false);
                        object.time = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.time = options.longs === String ? "0" : 0;
                }
                if (message.module != null && message.hasOwnProperty("module"))
                    object.module = message.module;
                if (message.readings && message.readings.length) {
                    object.readings = [];
                    for (var j = 0; j < message.readings.length; ++j)
                        object.readings[j] = $root.fk_data.SensorAndValue.toObject(message.readings[j], options);
                }
                if (message.time != null && message.hasOwnProperty("time"))
                    if (typeof message.time === "number")
                        object.time = options.longs === String ? String(message.time) : message.time;
                    else
                        object.time = options.longs === String ? $util.Long.prototype.toString.call(message.time) : options.longs === Number ? new $util.LongBits(message.time.low >>> 0, message.time.high >>> 0).toNumber() : message.time;
                return object;
            };
    
            /**
             * Converts this SensorGroup to JSON.
             * @function toJSON
             * @memberof fk_data.SensorGroup
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            SensorGroup.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return SensorGroup;
        })();
    
        fk_data.Readings = (function() {
    
            /**
             * Properties of a Readings.
             * @memberof fk_data
             * @interface IReadings
             * @property {number|Long|null} [time] Readings time
             * @property {number|Long|null} [reading] Readings reading
             * @property {number|null} [flags] Readings flags
             * @property {number|Long|null} [meta] Readings meta
             * @property {number|null} [uptime] Readings uptime
             * @property {fk_data.IDeviceLocation|null} [location] Readings location
             * @property {Array.<fk_data.ISensorGroup>|null} [sensorGroups] Readings sensorGroups
             */
    
            /**
             * Constructs a new Readings.
             * @memberof fk_data
             * @classdesc Represents a Readings.
             * @implements IReadings
             * @constructor
             * @param {fk_data.IReadings=} [properties] Properties to set
             */
            function Readings(properties) {
                this.sensorGroups = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Readings time.
             * @member {number|Long} time
             * @memberof fk_data.Readings
             * @instance
             */
            Readings.prototype.time = $util.Long ? $util.Long.fromBits(0,0,false) : 0;
    
            /**
             * Readings reading.
             * @member {number|Long} reading
             * @memberof fk_data.Readings
             * @instance
             */
            Readings.prototype.reading = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * Readings flags.
             * @member {number} flags
             * @memberof fk_data.Readings
             * @instance
             */
            Readings.prototype.flags = 0;
    
            /**
             * Readings meta.
             * @member {number|Long} meta
             * @memberof fk_data.Readings
             * @instance
             */
            Readings.prototype.meta = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * Readings uptime.
             * @member {number} uptime
             * @memberof fk_data.Readings
             * @instance
             */
            Readings.prototype.uptime = 0;
    
            /**
             * Readings location.
             * @member {fk_data.IDeviceLocation|null|undefined} location
             * @memberof fk_data.Readings
             * @instance
             */
            Readings.prototype.location = null;
    
            /**
             * Readings sensorGroups.
             * @member {Array.<fk_data.ISensorGroup>} sensorGroups
             * @memberof fk_data.Readings
             * @instance
             */
            Readings.prototype.sensorGroups = $util.emptyArray;
    
            /**
             * Creates a new Readings instance using the specified properties.
             * @function create
             * @memberof fk_data.Readings
             * @static
             * @param {fk_data.IReadings=} [properties] Properties to set
             * @returns {fk_data.Readings} Readings instance
             */
            Readings.create = function create(properties) {
                return new Readings(properties);
            };
    
            /**
             * Encodes the specified Readings message. Does not implicitly {@link fk_data.Readings.verify|verify} messages.
             * @function encode
             * @memberof fk_data.Readings
             * @static
             * @param {fk_data.IReadings} message Readings message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Readings.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.time != null && message.hasOwnProperty("time"))
                    writer.uint32(/* id 1, wireType 0 =*/8).int64(message.time);
                if (message.reading != null && message.hasOwnProperty("reading"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint64(message.reading);
                if (message.flags != null && message.hasOwnProperty("flags"))
                    writer.uint32(/* id 3, wireType 0 =*/24).uint32(message.flags);
                if (message.location != null && message.hasOwnProperty("location"))
                    $root.fk_data.DeviceLocation.encode(message.location, writer.uint32(/* id 4, wireType 2 =*/34).fork()).ldelim();
                if (message.sensorGroups != null && message.sensorGroups.length)
                    for (var i = 0; i < message.sensorGroups.length; ++i)
                        $root.fk_data.SensorGroup.encode(message.sensorGroups[i], writer.uint32(/* id 5, wireType 2 =*/42).fork()).ldelim();
                if (message.meta != null && message.hasOwnProperty("meta"))
                    writer.uint32(/* id 6, wireType 0 =*/48).uint64(message.meta);
                if (message.uptime != null && message.hasOwnProperty("uptime"))
                    writer.uint32(/* id 7, wireType 0 =*/56).uint32(message.uptime);
                return writer;
            };
    
            /**
             * Encodes the specified Readings message, length delimited. Does not implicitly {@link fk_data.Readings.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.Readings
             * @static
             * @param {fk_data.IReadings} message Readings message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Readings.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a Readings message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.Readings
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.Readings} Readings
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Readings.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.Readings();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.time = reader.int64();
                        break;
                    case 2:
                        message.reading = reader.uint64();
                        break;
                    case 3:
                        message.flags = reader.uint32();
                        break;
                    case 6:
                        message.meta = reader.uint64();
                        break;
                    case 7:
                        message.uptime = reader.uint32();
                        break;
                    case 4:
                        message.location = $root.fk_data.DeviceLocation.decode(reader, reader.uint32());
                        break;
                    case 5:
                        if (!(message.sensorGroups && message.sensorGroups.length))
                            message.sensorGroups = [];
                        message.sensorGroups.push($root.fk_data.SensorGroup.decode(reader, reader.uint32()));
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a Readings message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_data.Readings
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.Readings} Readings
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Readings.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a Readings message.
             * @function verify
             * @memberof fk_data.Readings
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            Readings.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.time != null && message.hasOwnProperty("time"))
                    if (!$util.isInteger(message.time) && !(message.time && $util.isInteger(message.time.low) && $util.isInteger(message.time.high)))
                        return "time: integer|Long expected";
                if (message.reading != null && message.hasOwnProperty("reading"))
                    if (!$util.isInteger(message.reading) && !(message.reading && $util.isInteger(message.reading.low) && $util.isInteger(message.reading.high)))
                        return "reading: integer|Long expected";
                if (message.flags != null && message.hasOwnProperty("flags"))
                    if (!$util.isInteger(message.flags))
                        return "flags: integer expected";
                if (message.meta != null && message.hasOwnProperty("meta"))
                    if (!$util.isInteger(message.meta) && !(message.meta && $util.isInteger(message.meta.low) && $util.isInteger(message.meta.high)))
                        return "meta: integer|Long expected";
                if (message.uptime != null && message.hasOwnProperty("uptime"))
                    if (!$util.isInteger(message.uptime))
                        return "uptime: integer expected";
                if (message.location != null && message.hasOwnProperty("location")) {
                    var error = $root.fk_data.DeviceLocation.verify(message.location);
                    if (error)
                        return "location." + error;
                }
                if (message.sensorGroups != null && message.hasOwnProperty("sensorGroups")) {
                    if (!Array.isArray(message.sensorGroups))
                        return "sensorGroups: array expected";
                    for (var i = 0; i < message.sensorGroups.length; ++i) {
                        var error = $root.fk_data.SensorGroup.verify(message.sensorGroups[i]);
                        if (error)
                            return "sensorGroups." + error;
                    }
                }
                return null;
            };
    
            /**
             * Creates a Readings message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.Readings
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.Readings} Readings
             */
            Readings.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.Readings)
                    return object;
                var message = new $root.fk_data.Readings();
                if (object.time != null)
                    if ($util.Long)
                        (message.time = $util.Long.fromValue(object.time)).unsigned = false;
                    else if (typeof object.time === "string")
                        message.time = parseInt(object.time, 10);
                    else if (typeof object.time === "number")
                        message.time = object.time;
                    else if (typeof object.time === "object")
                        message.time = new $util.LongBits(object.time.low >>> 0, object.time.high >>> 0).toNumber();
                if (object.reading != null)
                    if ($util.Long)
                        (message.reading = $util.Long.fromValue(object.reading)).unsigned = true;
                    else if (typeof object.reading === "string")
                        message.reading = parseInt(object.reading, 10);
                    else if (typeof object.reading === "number")
                        message.reading = object.reading;
                    else if (typeof object.reading === "object")
                        message.reading = new $util.LongBits(object.reading.low >>> 0, object.reading.high >>> 0).toNumber(true);
                if (object.flags != null)
                    message.flags = object.flags >>> 0;
                if (object.meta != null)
                    if ($util.Long)
                        (message.meta = $util.Long.fromValue(object.meta)).unsigned = true;
                    else if (typeof object.meta === "string")
                        message.meta = parseInt(object.meta, 10);
                    else if (typeof object.meta === "number")
                        message.meta = object.meta;
                    else if (typeof object.meta === "object")
                        message.meta = new $util.LongBits(object.meta.low >>> 0, object.meta.high >>> 0).toNumber(true);
                if (object.uptime != null)
                    message.uptime = object.uptime >>> 0;
                if (object.location != null) {
                    if (typeof object.location !== "object")
                        throw TypeError(".fk_data.Readings.location: object expected");
                    message.location = $root.fk_data.DeviceLocation.fromObject(object.location);
                }
                if (object.sensorGroups) {
                    if (!Array.isArray(object.sensorGroups))
                        throw TypeError(".fk_data.Readings.sensorGroups: array expected");
                    message.sensorGroups = [];
                    for (var i = 0; i < object.sensorGroups.length; ++i) {
                        if (typeof object.sensorGroups[i] !== "object")
                            throw TypeError(".fk_data.Readings.sensorGroups: object expected");
                        message.sensorGroups[i] = $root.fk_data.SensorGroup.fromObject(object.sensorGroups[i]);
                    }
                }
                return message;
            };
    
            /**
             * Creates a plain object from a Readings message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.Readings
             * @static
             * @param {fk_data.Readings} message Readings
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            Readings.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults)
                    object.sensorGroups = [];
                if (options.defaults) {
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, false);
                        object.time = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.time = options.longs === String ? "0" : 0;
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.reading = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.reading = options.longs === String ? "0" : 0;
                    object.flags = 0;
                    object.location = null;
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.meta = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.meta = options.longs === String ? "0" : 0;
                    object.uptime = 0;
                }
                if (message.time != null && message.hasOwnProperty("time"))
                    if (typeof message.time === "number")
                        object.time = options.longs === String ? String(message.time) : message.time;
                    else
                        object.time = options.longs === String ? $util.Long.prototype.toString.call(message.time) : options.longs === Number ? new $util.LongBits(message.time.low >>> 0, message.time.high >>> 0).toNumber() : message.time;
                if (message.reading != null && message.hasOwnProperty("reading"))
                    if (typeof message.reading === "number")
                        object.reading = options.longs === String ? String(message.reading) : message.reading;
                    else
                        object.reading = options.longs === String ? $util.Long.prototype.toString.call(message.reading) : options.longs === Number ? new $util.LongBits(message.reading.low >>> 0, message.reading.high >>> 0).toNumber(true) : message.reading;
                if (message.flags != null && message.hasOwnProperty("flags"))
                    object.flags = message.flags;
                if (message.location != null && message.hasOwnProperty("location"))
                    object.location = $root.fk_data.DeviceLocation.toObject(message.location, options);
                if (message.sensorGroups && message.sensorGroups.length) {
                    object.sensorGroups = [];
                    for (var j = 0; j < message.sensorGroups.length; ++j)
                        object.sensorGroups[j] = $root.fk_data.SensorGroup.toObject(message.sensorGroups[j], options);
                }
                if (message.meta != null && message.hasOwnProperty("meta"))
                    if (typeof message.meta === "number")
                        object.meta = options.longs === String ? String(message.meta) : message.meta;
                    else
                        object.meta = options.longs === String ? $util.Long.prototype.toString.call(message.meta) : options.longs === Number ? new $util.LongBits(message.meta.low >>> 0, message.meta.high >>> 0).toNumber(true) : message.meta;
                if (message.uptime != null && message.hasOwnProperty("uptime"))
                    object.uptime = message.uptime;
                return object;
            };
    
            /**
             * Converts this Readings to JSON.
             * @function toJSON
             * @memberof fk_data.Readings
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Readings.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Readings;
        })();
    
        fk_data.Interval = (function() {
    
            /**
             * Properties of an Interval.
             * @memberof fk_data
             * @interface IInterval
             * @property {number|Long|null} [start] Interval start
             * @property {number|Long|null} [end] Interval end
             * @property {number|null} [interval] Interval interval
             */
    
            /**
             * Constructs a new Interval.
             * @memberof fk_data
             * @classdesc Represents an Interval.
             * @implements IInterval
             * @constructor
             * @param {fk_data.IInterval=} [properties] Properties to set
             */
            function Interval(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Interval start.
             * @member {number|Long} start
             * @memberof fk_data.Interval
             * @instance
             */
            Interval.prototype.start = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * Interval end.
             * @member {number|Long} end
             * @memberof fk_data.Interval
             * @instance
             */
            Interval.prototype.end = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * Interval interval.
             * @member {number} interval
             * @memberof fk_data.Interval
             * @instance
             */
            Interval.prototype.interval = 0;
    
            /**
             * Creates a new Interval instance using the specified properties.
             * @function create
             * @memberof fk_data.Interval
             * @static
             * @param {fk_data.IInterval=} [properties] Properties to set
             * @returns {fk_data.Interval} Interval instance
             */
            Interval.create = function create(properties) {
                return new Interval(properties);
            };
    
            /**
             * Encodes the specified Interval message. Does not implicitly {@link fk_data.Interval.verify|verify} messages.
             * @function encode
             * @memberof fk_data.Interval
             * @static
             * @param {fk_data.IInterval} message Interval message or plain object to encode
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
             * Encodes the specified Interval message, length delimited. Does not implicitly {@link fk_data.Interval.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.Interval
             * @static
             * @param {fk_data.IInterval} message Interval message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Interval.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes an Interval message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.Interval
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.Interval} Interval
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Interval.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.Interval();
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
             * @memberof fk_data.Interval
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.Interval} Interval
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
             * @memberof fk_data.Interval
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
             * @memberof fk_data.Interval
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.Interval} Interval
             */
            Interval.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.Interval)
                    return object;
                var message = new $root.fk_data.Interval();
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
             * @memberof fk_data.Interval
             * @static
             * @param {fk_data.Interval} message Interval
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
             * @memberof fk_data.Interval
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Interval.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Interval;
        })();
    
        fk_data.JobSchedule = (function() {
    
            /**
             * Properties of a JobSchedule.
             * @memberof fk_data
             * @interface IJobSchedule
             * @property {Uint8Array|null} [cron] JobSchedule cron
             * @property {number|null} [interval] JobSchedule interval
             * @property {number|null} [repeated] JobSchedule repeated
             * @property {number|null} [duration] JobSchedule duration
             * @property {number|null} [jitter] JobSchedule jitter
             * @property {Array.<fk_data.IInterval>|null} [intervals] JobSchedule intervals
             */
    
            /**
             * Constructs a new JobSchedule.
             * @memberof fk_data
             * @classdesc Represents a JobSchedule.
             * @implements IJobSchedule
             * @constructor
             * @param {fk_data.IJobSchedule=} [properties] Properties to set
             */
            function JobSchedule(properties) {
                this.intervals = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * JobSchedule cron.
             * @member {Uint8Array} cron
             * @memberof fk_data.JobSchedule
             * @instance
             */
            JobSchedule.prototype.cron = $util.newBuffer([]);
    
            /**
             * JobSchedule interval.
             * @member {number} interval
             * @memberof fk_data.JobSchedule
             * @instance
             */
            JobSchedule.prototype.interval = 0;
    
            /**
             * JobSchedule repeated.
             * @member {number} repeated
             * @memberof fk_data.JobSchedule
             * @instance
             */
            JobSchedule.prototype.repeated = 0;
    
            /**
             * JobSchedule duration.
             * @member {number} duration
             * @memberof fk_data.JobSchedule
             * @instance
             */
            JobSchedule.prototype.duration = 0;
    
            /**
             * JobSchedule jitter.
             * @member {number} jitter
             * @memberof fk_data.JobSchedule
             * @instance
             */
            JobSchedule.prototype.jitter = 0;
    
            /**
             * JobSchedule intervals.
             * @member {Array.<fk_data.IInterval>} intervals
             * @memberof fk_data.JobSchedule
             * @instance
             */
            JobSchedule.prototype.intervals = $util.emptyArray;
    
            /**
             * Creates a new JobSchedule instance using the specified properties.
             * @function create
             * @memberof fk_data.JobSchedule
             * @static
             * @param {fk_data.IJobSchedule=} [properties] Properties to set
             * @returns {fk_data.JobSchedule} JobSchedule instance
             */
            JobSchedule.create = function create(properties) {
                return new JobSchedule(properties);
            };
    
            /**
             * Encodes the specified JobSchedule message. Does not implicitly {@link fk_data.JobSchedule.verify|verify} messages.
             * @function encode
             * @memberof fk_data.JobSchedule
             * @static
             * @param {fk_data.IJobSchedule} message JobSchedule message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            JobSchedule.encode = function encode(message, writer) {
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
                        $root.fk_data.Interval.encode(message.intervals[i], writer.uint32(/* id 6, wireType 2 =*/50).fork()).ldelim();
                return writer;
            };
    
            /**
             * Encodes the specified JobSchedule message, length delimited. Does not implicitly {@link fk_data.JobSchedule.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.JobSchedule
             * @static
             * @param {fk_data.IJobSchedule} message JobSchedule message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            JobSchedule.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a JobSchedule message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.JobSchedule
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.JobSchedule} JobSchedule
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            JobSchedule.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.JobSchedule();
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
                        message.intervals.push($root.fk_data.Interval.decode(reader, reader.uint32()));
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a JobSchedule message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_data.JobSchedule
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.JobSchedule} JobSchedule
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            JobSchedule.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a JobSchedule message.
             * @function verify
             * @memberof fk_data.JobSchedule
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            JobSchedule.verify = function verify(message) {
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
                        var error = $root.fk_data.Interval.verify(message.intervals[i]);
                        if (error)
                            return "intervals." + error;
                    }
                }
                return null;
            };
    
            /**
             * Creates a JobSchedule message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.JobSchedule
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.JobSchedule} JobSchedule
             */
            JobSchedule.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.JobSchedule)
                    return object;
                var message = new $root.fk_data.JobSchedule();
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
                        throw TypeError(".fk_data.JobSchedule.intervals: array expected");
                    message.intervals = [];
                    for (var i = 0; i < object.intervals.length; ++i) {
                        if (typeof object.intervals[i] !== "object")
                            throw TypeError(".fk_data.JobSchedule.intervals: object expected");
                        message.intervals[i] = $root.fk_data.Interval.fromObject(object.intervals[i]);
                    }
                }
                return message;
            };
    
            /**
             * Creates a plain object from a JobSchedule message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.JobSchedule
             * @static
             * @param {fk_data.JobSchedule} message JobSchedule
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            JobSchedule.toObject = function toObject(message, options) {
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
                        object.intervals[j] = $root.fk_data.Interval.toObject(message.intervals[j], options);
                }
                return object;
            };
    
            /**
             * Converts this JobSchedule to JSON.
             * @function toJSON
             * @memberof fk_data.JobSchedule
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            JobSchedule.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return JobSchedule;
        })();
    
        fk_data.Schedule = (function() {
    
            /**
             * Properties of a Schedule.
             * @memberof fk_data
             * @interface ISchedule
             * @property {fk_data.IJobSchedule|null} [readings] Schedule readings
             * @property {fk_data.IJobSchedule|null} [network] Schedule network
             * @property {fk_data.IJobSchedule|null} [lora] Schedule lora
             * @property {fk_data.IJobSchedule|null} [gps] Schedule gps
             */
    
            /**
             * Constructs a new Schedule.
             * @memberof fk_data
             * @classdesc Represents a Schedule.
             * @implements ISchedule
             * @constructor
             * @param {fk_data.ISchedule=} [properties] Properties to set
             */
            function Schedule(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Schedule readings.
             * @member {fk_data.IJobSchedule|null|undefined} readings
             * @memberof fk_data.Schedule
             * @instance
             */
            Schedule.prototype.readings = null;
    
            /**
             * Schedule network.
             * @member {fk_data.IJobSchedule|null|undefined} network
             * @memberof fk_data.Schedule
             * @instance
             */
            Schedule.prototype.network = null;
    
            /**
             * Schedule lora.
             * @member {fk_data.IJobSchedule|null|undefined} lora
             * @memberof fk_data.Schedule
             * @instance
             */
            Schedule.prototype.lora = null;
    
            /**
             * Schedule gps.
             * @member {fk_data.IJobSchedule|null|undefined} gps
             * @memberof fk_data.Schedule
             * @instance
             */
            Schedule.prototype.gps = null;
    
            /**
             * Creates a new Schedule instance using the specified properties.
             * @function create
             * @memberof fk_data.Schedule
             * @static
             * @param {fk_data.ISchedule=} [properties] Properties to set
             * @returns {fk_data.Schedule} Schedule instance
             */
            Schedule.create = function create(properties) {
                return new Schedule(properties);
            };
    
            /**
             * Encodes the specified Schedule message. Does not implicitly {@link fk_data.Schedule.verify|verify} messages.
             * @function encode
             * @memberof fk_data.Schedule
             * @static
             * @param {fk_data.ISchedule} message Schedule message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Schedule.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.readings != null && message.hasOwnProperty("readings"))
                    $root.fk_data.JobSchedule.encode(message.readings, writer.uint32(/* id 1, wireType 2 =*/10).fork()).ldelim();
                if (message.network != null && message.hasOwnProperty("network"))
                    $root.fk_data.JobSchedule.encode(message.network, writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
                if (message.lora != null && message.hasOwnProperty("lora"))
                    $root.fk_data.JobSchedule.encode(message.lora, writer.uint32(/* id 3, wireType 2 =*/26).fork()).ldelim();
                if (message.gps != null && message.hasOwnProperty("gps"))
                    $root.fk_data.JobSchedule.encode(message.gps, writer.uint32(/* id 4, wireType 2 =*/34).fork()).ldelim();
                return writer;
            };
    
            /**
             * Encodes the specified Schedule message, length delimited. Does not implicitly {@link fk_data.Schedule.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.Schedule
             * @static
             * @param {fk_data.ISchedule} message Schedule message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Schedule.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a Schedule message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.Schedule
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.Schedule} Schedule
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Schedule.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.Schedule();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.readings = $root.fk_data.JobSchedule.decode(reader, reader.uint32());
                        break;
                    case 2:
                        message.network = $root.fk_data.JobSchedule.decode(reader, reader.uint32());
                        break;
                    case 3:
                        message.lora = $root.fk_data.JobSchedule.decode(reader, reader.uint32());
                        break;
                    case 4:
                        message.gps = $root.fk_data.JobSchedule.decode(reader, reader.uint32());
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
             * @memberof fk_data.Schedule
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.Schedule} Schedule
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
             * @memberof fk_data.Schedule
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            Schedule.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.readings != null && message.hasOwnProperty("readings")) {
                    var error = $root.fk_data.JobSchedule.verify(message.readings);
                    if (error)
                        return "readings." + error;
                }
                if (message.network != null && message.hasOwnProperty("network")) {
                    var error = $root.fk_data.JobSchedule.verify(message.network);
                    if (error)
                        return "network." + error;
                }
                if (message.lora != null && message.hasOwnProperty("lora")) {
                    var error = $root.fk_data.JobSchedule.verify(message.lora);
                    if (error)
                        return "lora." + error;
                }
                if (message.gps != null && message.hasOwnProperty("gps")) {
                    var error = $root.fk_data.JobSchedule.verify(message.gps);
                    if (error)
                        return "gps." + error;
                }
                return null;
            };
    
            /**
             * Creates a Schedule message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.Schedule
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.Schedule} Schedule
             */
            Schedule.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.Schedule)
                    return object;
                var message = new $root.fk_data.Schedule();
                if (object.readings != null) {
                    if (typeof object.readings !== "object")
                        throw TypeError(".fk_data.Schedule.readings: object expected");
                    message.readings = $root.fk_data.JobSchedule.fromObject(object.readings);
                }
                if (object.network != null) {
                    if (typeof object.network !== "object")
                        throw TypeError(".fk_data.Schedule.network: object expected");
                    message.network = $root.fk_data.JobSchedule.fromObject(object.network);
                }
                if (object.lora != null) {
                    if (typeof object.lora !== "object")
                        throw TypeError(".fk_data.Schedule.lora: object expected");
                    message.lora = $root.fk_data.JobSchedule.fromObject(object.lora);
                }
                if (object.gps != null) {
                    if (typeof object.gps !== "object")
                        throw TypeError(".fk_data.Schedule.gps: object expected");
                    message.gps = $root.fk_data.JobSchedule.fromObject(object.gps);
                }
                return message;
            };
    
            /**
             * Creates a plain object from a Schedule message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.Schedule
             * @static
             * @param {fk_data.Schedule} message Schedule
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            Schedule.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.readings = null;
                    object.network = null;
                    object.lora = null;
                    object.gps = null;
                }
                if (message.readings != null && message.hasOwnProperty("readings"))
                    object.readings = $root.fk_data.JobSchedule.toObject(message.readings, options);
                if (message.network != null && message.hasOwnProperty("network"))
                    object.network = $root.fk_data.JobSchedule.toObject(message.network, options);
                if (message.lora != null && message.hasOwnProperty("lora"))
                    object.lora = $root.fk_data.JobSchedule.toObject(message.lora, options);
                if (message.gps != null && message.hasOwnProperty("gps"))
                    object.gps = $root.fk_data.JobSchedule.toObject(message.gps, options);
                return object;
            };
    
            /**
             * Converts this Schedule to JSON.
             * @function toJSON
             * @memberof fk_data.Schedule
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Schedule.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Schedule;
        })();
    
        fk_data.Identity = (function() {
    
            /**
             * Properties of an Identity.
             * @memberof fk_data
             * @interface IIdentity
             * @property {string|null} [name] Identity name
             */
    
            /**
             * Constructs a new Identity.
             * @memberof fk_data
             * @classdesc Represents an Identity.
             * @implements IIdentity
             * @constructor
             * @param {fk_data.IIdentity=} [properties] Properties to set
             */
            function Identity(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Identity name.
             * @member {string} name
             * @memberof fk_data.Identity
             * @instance
             */
            Identity.prototype.name = "";
    
            /**
             * Creates a new Identity instance using the specified properties.
             * @function create
             * @memberof fk_data.Identity
             * @static
             * @param {fk_data.IIdentity=} [properties] Properties to set
             * @returns {fk_data.Identity} Identity instance
             */
            Identity.create = function create(properties) {
                return new Identity(properties);
            };
    
            /**
             * Encodes the specified Identity message. Does not implicitly {@link fk_data.Identity.verify|verify} messages.
             * @function encode
             * @memberof fk_data.Identity
             * @static
             * @param {fk_data.IIdentity} message Identity message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Identity.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.name != null && message.hasOwnProperty("name"))
                    writer.uint32(/* id 1, wireType 2 =*/10).string(message.name);
                return writer;
            };
    
            /**
             * Encodes the specified Identity message, length delimited. Does not implicitly {@link fk_data.Identity.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.Identity
             * @static
             * @param {fk_data.IIdentity} message Identity message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Identity.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes an Identity message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.Identity
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.Identity} Identity
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Identity.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.Identity();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
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
             * Decodes an Identity message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_data.Identity
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.Identity} Identity
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
             * @memberof fk_data.Identity
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            Identity.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.name != null && message.hasOwnProperty("name"))
                    if (!$util.isString(message.name))
                        return "name: string expected";
                return null;
            };
    
            /**
             * Creates an Identity message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.Identity
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.Identity} Identity
             */
            Identity.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.Identity)
                    return object;
                var message = new $root.fk_data.Identity();
                if (object.name != null)
                    message.name = String(object.name);
                return message;
            };
    
            /**
             * Creates a plain object from an Identity message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.Identity
             * @static
             * @param {fk_data.Identity} message Identity
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            Identity.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults)
                    object.name = "";
                if (message.name != null && message.hasOwnProperty("name"))
                    object.name = message.name;
                return object;
            };
    
            /**
             * Converts this Identity to JSON.
             * @function toJSON
             * @memberof fk_data.Identity
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Identity.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Identity;
        })();
    
        /**
         * ConditionFlags enum.
         * @name fk_data.ConditionFlags
         * @enum {string}
         * @property {number} CONDITION_FLAGS_NONE=0 CONDITION_FLAGS_NONE value
         * @property {number} CONDITION_FLAGS_RECORDING=1 CONDITION_FLAGS_RECORDING value
         */
        fk_data.ConditionFlags = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "CONDITION_FLAGS_NONE"] = 0;
            values[valuesById[1] = "CONDITION_FLAGS_RECORDING"] = 1;
            return values;
        })();
    
        fk_data.Condition = (function() {
    
            /**
             * Properties of a Condition.
             * @memberof fk_data
             * @interface ICondition
             * @property {number|null} [flags] Condition flags
             * @property {number|null} [recording] Condition recording
             */
    
            /**
             * Constructs a new Condition.
             * @memberof fk_data
             * @classdesc Represents a Condition.
             * @implements ICondition
             * @constructor
             * @param {fk_data.ICondition=} [properties] Properties to set
             */
            function Condition(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Condition flags.
             * @member {number} flags
             * @memberof fk_data.Condition
             * @instance
             */
            Condition.prototype.flags = 0;
    
            /**
             * Condition recording.
             * @member {number} recording
             * @memberof fk_data.Condition
             * @instance
             */
            Condition.prototype.recording = 0;
    
            /**
             * Creates a new Condition instance using the specified properties.
             * @function create
             * @memberof fk_data.Condition
             * @static
             * @param {fk_data.ICondition=} [properties] Properties to set
             * @returns {fk_data.Condition} Condition instance
             */
            Condition.create = function create(properties) {
                return new Condition(properties);
            };
    
            /**
             * Encodes the specified Condition message. Does not implicitly {@link fk_data.Condition.verify|verify} messages.
             * @function encode
             * @memberof fk_data.Condition
             * @static
             * @param {fk_data.ICondition} message Condition message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Condition.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.flags != null && message.hasOwnProperty("flags"))
                    writer.uint32(/* id 1, wireType 0 =*/8).uint32(message.flags);
                if (message.recording != null && message.hasOwnProperty("recording"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint32(message.recording);
                return writer;
            };
    
            /**
             * Encodes the specified Condition message, length delimited. Does not implicitly {@link fk_data.Condition.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.Condition
             * @static
             * @param {fk_data.ICondition} message Condition message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Condition.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a Condition message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.Condition
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.Condition} Condition
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Condition.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.Condition();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.flags = reader.uint32();
                        break;
                    case 2:
                        message.recording = reader.uint32();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a Condition message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_data.Condition
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.Condition} Condition
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Condition.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a Condition message.
             * @function verify
             * @memberof fk_data.Condition
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            Condition.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.flags != null && message.hasOwnProperty("flags"))
                    if (!$util.isInteger(message.flags))
                        return "flags: integer expected";
                if (message.recording != null && message.hasOwnProperty("recording"))
                    if (!$util.isInteger(message.recording))
                        return "recording: integer expected";
                return null;
            };
    
            /**
             * Creates a Condition message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.Condition
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.Condition} Condition
             */
            Condition.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.Condition)
                    return object;
                var message = new $root.fk_data.Condition();
                if (object.flags != null)
                    message.flags = object.flags >>> 0;
                if (object.recording != null)
                    message.recording = object.recording >>> 0;
                return message;
            };
    
            /**
             * Creates a plain object from a Condition message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.Condition
             * @static
             * @param {fk_data.Condition} message Condition
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            Condition.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.flags = 0;
                    object.recording = 0;
                }
                if (message.flags != null && message.hasOwnProperty("flags"))
                    object.flags = message.flags;
                if (message.recording != null && message.hasOwnProperty("recording"))
                    object.recording = message.recording;
                return object;
            };
    
            /**
             * Converts this Condition to JSON.
             * @function toJSON
             * @memberof fk_data.Condition
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Condition.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Condition;
        })();
    
        fk_data.NetworkInfo = (function() {
    
            /**
             * Properties of a NetworkInfo.
             * @memberof fk_data
             * @interface INetworkInfo
             * @property {string|null} [ssid] NetworkInfo ssid
             * @property {string|null} [password] NetworkInfo password
             * @property {boolean|null} [create] NetworkInfo create
             * @property {boolean|null} [preferred] NetworkInfo preferred
             */
    
            /**
             * Constructs a new NetworkInfo.
             * @memberof fk_data
             * @classdesc Represents a NetworkInfo.
             * @implements INetworkInfo
             * @constructor
             * @param {fk_data.INetworkInfo=} [properties] Properties to set
             */
            function NetworkInfo(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * NetworkInfo ssid.
             * @member {string} ssid
             * @memberof fk_data.NetworkInfo
             * @instance
             */
            NetworkInfo.prototype.ssid = "";
    
            /**
             * NetworkInfo password.
             * @member {string} password
             * @memberof fk_data.NetworkInfo
             * @instance
             */
            NetworkInfo.prototype.password = "";
    
            /**
             * NetworkInfo create.
             * @member {boolean} create
             * @memberof fk_data.NetworkInfo
             * @instance
             */
            NetworkInfo.prototype.create = false;
    
            /**
             * NetworkInfo preferred.
             * @member {boolean} preferred
             * @memberof fk_data.NetworkInfo
             * @instance
             */
            NetworkInfo.prototype.preferred = false;
    
            /**
             * Creates a new NetworkInfo instance using the specified properties.
             * @function create
             * @memberof fk_data.NetworkInfo
             * @static
             * @param {fk_data.INetworkInfo=} [properties] Properties to set
             * @returns {fk_data.NetworkInfo} NetworkInfo instance
             */
            NetworkInfo.create = function create(properties) {
                return new NetworkInfo(properties);
            };
    
            /**
             * Encodes the specified NetworkInfo message. Does not implicitly {@link fk_data.NetworkInfo.verify|verify} messages.
             * @function encode
             * @memberof fk_data.NetworkInfo
             * @static
             * @param {fk_data.INetworkInfo} message NetworkInfo message or plain object to encode
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
                return writer;
            };
    
            /**
             * Encodes the specified NetworkInfo message, length delimited. Does not implicitly {@link fk_data.NetworkInfo.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.NetworkInfo
             * @static
             * @param {fk_data.INetworkInfo} message NetworkInfo message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            NetworkInfo.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a NetworkInfo message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.NetworkInfo
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.NetworkInfo} NetworkInfo
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            NetworkInfo.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.NetworkInfo();
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
             * @memberof fk_data.NetworkInfo
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.NetworkInfo} NetworkInfo
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
             * @memberof fk_data.NetworkInfo
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
                return null;
            };
    
            /**
             * Creates a NetworkInfo message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.NetworkInfo
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.NetworkInfo} NetworkInfo
             */
            NetworkInfo.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.NetworkInfo)
                    return object;
                var message = new $root.fk_data.NetworkInfo();
                if (object.ssid != null)
                    message.ssid = String(object.ssid);
                if (object.password != null)
                    message.password = String(object.password);
                if (object.create != null)
                    message.create = Boolean(object.create);
                if (object.preferred != null)
                    message.preferred = Boolean(object.preferred);
                return message;
            };
    
            /**
             * Creates a plain object from a NetworkInfo message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.NetworkInfo
             * @static
             * @param {fk_data.NetworkInfo} message NetworkInfo
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
                }
                if (message.ssid != null && message.hasOwnProperty("ssid"))
                    object.ssid = message.ssid;
                if (message.password != null && message.hasOwnProperty("password"))
                    object.password = message.password;
                if (message.create != null && message.hasOwnProperty("create"))
                    object.create = message.create;
                if (message.preferred != null && message.hasOwnProperty("preferred"))
                    object.preferred = message.preferred;
                return object;
            };
    
            /**
             * Converts this NetworkInfo to JSON.
             * @function toJSON
             * @memberof fk_data.NetworkInfo
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            NetworkInfo.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return NetworkInfo;
        })();
    
        fk_data.WifiTransmission = (function() {
    
            /**
             * Properties of a WifiTransmission.
             * @memberof fk_data
             * @interface IWifiTransmission
             * @property {string|null} [url] WifiTransmission url
             * @property {string|null} [token] WifiTransmission token
             * @property {boolean|null} [enabled] WifiTransmission enabled
             */
    
            /**
             * Constructs a new WifiTransmission.
             * @memberof fk_data
             * @classdesc Represents a WifiTransmission.
             * @implements IWifiTransmission
             * @constructor
             * @param {fk_data.IWifiTransmission=} [properties] Properties to set
             */
            function WifiTransmission(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * WifiTransmission url.
             * @member {string} url
             * @memberof fk_data.WifiTransmission
             * @instance
             */
            WifiTransmission.prototype.url = "";
    
            /**
             * WifiTransmission token.
             * @member {string} token
             * @memberof fk_data.WifiTransmission
             * @instance
             */
            WifiTransmission.prototype.token = "";
    
            /**
             * WifiTransmission enabled.
             * @member {boolean} enabled
             * @memberof fk_data.WifiTransmission
             * @instance
             */
            WifiTransmission.prototype.enabled = false;
    
            /**
             * Creates a new WifiTransmission instance using the specified properties.
             * @function create
             * @memberof fk_data.WifiTransmission
             * @static
             * @param {fk_data.IWifiTransmission=} [properties] Properties to set
             * @returns {fk_data.WifiTransmission} WifiTransmission instance
             */
            WifiTransmission.create = function create(properties) {
                return new WifiTransmission(properties);
            };
    
            /**
             * Encodes the specified WifiTransmission message. Does not implicitly {@link fk_data.WifiTransmission.verify|verify} messages.
             * @function encode
             * @memberof fk_data.WifiTransmission
             * @static
             * @param {fk_data.IWifiTransmission} message WifiTransmission message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            WifiTransmission.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.url != null && message.hasOwnProperty("url"))
                    writer.uint32(/* id 1, wireType 2 =*/10).string(message.url);
                if (message.token != null && message.hasOwnProperty("token"))
                    writer.uint32(/* id 2, wireType 2 =*/18).string(message.token);
                if (message.enabled != null && message.hasOwnProperty("enabled"))
                    writer.uint32(/* id 3, wireType 0 =*/24).bool(message.enabled);
                return writer;
            };
    
            /**
             * Encodes the specified WifiTransmission message, length delimited. Does not implicitly {@link fk_data.WifiTransmission.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.WifiTransmission
             * @static
             * @param {fk_data.IWifiTransmission} message WifiTransmission message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            WifiTransmission.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a WifiTransmission message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.WifiTransmission
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.WifiTransmission} WifiTransmission
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            WifiTransmission.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.WifiTransmission();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.url = reader.string();
                        break;
                    case 2:
                        message.token = reader.string();
                        break;
                    case 3:
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
             * @memberof fk_data.WifiTransmission
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.WifiTransmission} WifiTransmission
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
             * @memberof fk_data.WifiTransmission
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            WifiTransmission.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
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
             * @memberof fk_data.WifiTransmission
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.WifiTransmission} WifiTransmission
             */
            WifiTransmission.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.WifiTransmission)
                    return object;
                var message = new $root.fk_data.WifiTransmission();
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
             * @memberof fk_data.WifiTransmission
             * @static
             * @param {fk_data.WifiTransmission} message WifiTransmission
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            WifiTransmission.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.url = "";
                    object.token = "";
                    object.enabled = false;
                }
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
             * @memberof fk_data.WifiTransmission
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            WifiTransmission.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return WifiTransmission;
        })();
    
        fk_data.TransmissionSettings = (function() {
    
            /**
             * Properties of a TransmissionSettings.
             * @memberof fk_data
             * @interface ITransmissionSettings
             * @property {fk_data.IWifiTransmission|null} [wifi] TransmissionSettings wifi
             */
    
            /**
             * Constructs a new TransmissionSettings.
             * @memberof fk_data
             * @classdesc Represents a TransmissionSettings.
             * @implements ITransmissionSettings
             * @constructor
             * @param {fk_data.ITransmissionSettings=} [properties] Properties to set
             */
            function TransmissionSettings(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * TransmissionSettings wifi.
             * @member {fk_data.IWifiTransmission|null|undefined} wifi
             * @memberof fk_data.TransmissionSettings
             * @instance
             */
            TransmissionSettings.prototype.wifi = null;
    
            /**
             * Creates a new TransmissionSettings instance using the specified properties.
             * @function create
             * @memberof fk_data.TransmissionSettings
             * @static
             * @param {fk_data.ITransmissionSettings=} [properties] Properties to set
             * @returns {fk_data.TransmissionSettings} TransmissionSettings instance
             */
            TransmissionSettings.create = function create(properties) {
                return new TransmissionSettings(properties);
            };
    
            /**
             * Encodes the specified TransmissionSettings message. Does not implicitly {@link fk_data.TransmissionSettings.verify|verify} messages.
             * @function encode
             * @memberof fk_data.TransmissionSettings
             * @static
             * @param {fk_data.ITransmissionSettings} message TransmissionSettings message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            TransmissionSettings.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.wifi != null && message.hasOwnProperty("wifi"))
                    $root.fk_data.WifiTransmission.encode(message.wifi, writer.uint32(/* id 1, wireType 2 =*/10).fork()).ldelim();
                return writer;
            };
    
            /**
             * Encodes the specified TransmissionSettings message, length delimited. Does not implicitly {@link fk_data.TransmissionSettings.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.TransmissionSettings
             * @static
             * @param {fk_data.ITransmissionSettings} message TransmissionSettings message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            TransmissionSettings.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a TransmissionSettings message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.TransmissionSettings
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.TransmissionSettings} TransmissionSettings
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            TransmissionSettings.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.TransmissionSettings();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.wifi = $root.fk_data.WifiTransmission.decode(reader, reader.uint32());
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a TransmissionSettings message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_data.TransmissionSettings
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.TransmissionSettings} TransmissionSettings
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            TransmissionSettings.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a TransmissionSettings message.
             * @function verify
             * @memberof fk_data.TransmissionSettings
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            TransmissionSettings.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.wifi != null && message.hasOwnProperty("wifi")) {
                    var error = $root.fk_data.WifiTransmission.verify(message.wifi);
                    if (error)
                        return "wifi." + error;
                }
                return null;
            };
    
            /**
             * Creates a TransmissionSettings message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.TransmissionSettings
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.TransmissionSettings} TransmissionSettings
             */
            TransmissionSettings.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.TransmissionSettings)
                    return object;
                var message = new $root.fk_data.TransmissionSettings();
                if (object.wifi != null) {
                    if (typeof object.wifi !== "object")
                        throw TypeError(".fk_data.TransmissionSettings.wifi: object expected");
                    message.wifi = $root.fk_data.WifiTransmission.fromObject(object.wifi);
                }
                return message;
            };
    
            /**
             * Creates a plain object from a TransmissionSettings message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.TransmissionSettings
             * @static
             * @param {fk_data.TransmissionSettings} message TransmissionSettings
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            TransmissionSettings.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults)
                    object.wifi = null;
                if (message.wifi != null && message.hasOwnProperty("wifi"))
                    object.wifi = $root.fk_data.WifiTransmission.toObject(message.wifi, options);
                return object;
            };
    
            /**
             * Converts this TransmissionSettings to JSON.
             * @function toJSON
             * @memberof fk_data.TransmissionSettings
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            TransmissionSettings.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return TransmissionSettings;
        })();
    
        fk_data.NetworkSettings = (function() {
    
            /**
             * Properties of a NetworkSettings.
             * @memberof fk_data
             * @interface INetworkSettings
             * @property {Array.<fk_data.INetworkInfo>|null} [networks] NetworkSettings networks
             */
    
            /**
             * Constructs a new NetworkSettings.
             * @memberof fk_data
             * @classdesc Represents a NetworkSettings.
             * @implements INetworkSettings
             * @constructor
             * @param {fk_data.INetworkSettings=} [properties] Properties to set
             */
            function NetworkSettings(properties) {
                this.networks = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * NetworkSettings networks.
             * @member {Array.<fk_data.INetworkInfo>} networks
             * @memberof fk_data.NetworkSettings
             * @instance
             */
            NetworkSettings.prototype.networks = $util.emptyArray;
    
            /**
             * Creates a new NetworkSettings instance using the specified properties.
             * @function create
             * @memberof fk_data.NetworkSettings
             * @static
             * @param {fk_data.INetworkSettings=} [properties] Properties to set
             * @returns {fk_data.NetworkSettings} NetworkSettings instance
             */
            NetworkSettings.create = function create(properties) {
                return new NetworkSettings(properties);
            };
    
            /**
             * Encodes the specified NetworkSettings message. Does not implicitly {@link fk_data.NetworkSettings.verify|verify} messages.
             * @function encode
             * @memberof fk_data.NetworkSettings
             * @static
             * @param {fk_data.INetworkSettings} message NetworkSettings message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            NetworkSettings.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.networks != null && message.networks.length)
                    for (var i = 0; i < message.networks.length; ++i)
                        $root.fk_data.NetworkInfo.encode(message.networks[i], writer.uint32(/* id 1, wireType 2 =*/10).fork()).ldelim();
                return writer;
            };
    
            /**
             * Encodes the specified NetworkSettings message, length delimited. Does not implicitly {@link fk_data.NetworkSettings.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.NetworkSettings
             * @static
             * @param {fk_data.INetworkSettings} message NetworkSettings message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            NetworkSettings.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a NetworkSettings message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.NetworkSettings
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.NetworkSettings} NetworkSettings
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            NetworkSettings.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.NetworkSettings();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        if (!(message.networks && message.networks.length))
                            message.networks = [];
                        message.networks.push($root.fk_data.NetworkInfo.decode(reader, reader.uint32()));
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
             * @memberof fk_data.NetworkSettings
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.NetworkSettings} NetworkSettings
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
             * @memberof fk_data.NetworkSettings
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            NetworkSettings.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.networks != null && message.hasOwnProperty("networks")) {
                    if (!Array.isArray(message.networks))
                        return "networks: array expected";
                    for (var i = 0; i < message.networks.length; ++i) {
                        var error = $root.fk_data.NetworkInfo.verify(message.networks[i]);
                        if (error)
                            return "networks." + error;
                    }
                }
                return null;
            };
    
            /**
             * Creates a NetworkSettings message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.NetworkSettings
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.NetworkSettings} NetworkSettings
             */
            NetworkSettings.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.NetworkSettings)
                    return object;
                var message = new $root.fk_data.NetworkSettings();
                if (object.networks) {
                    if (!Array.isArray(object.networks))
                        throw TypeError(".fk_data.NetworkSettings.networks: array expected");
                    message.networks = [];
                    for (var i = 0; i < object.networks.length; ++i) {
                        if (typeof object.networks[i] !== "object")
                            throw TypeError(".fk_data.NetworkSettings.networks: object expected");
                        message.networks[i] = $root.fk_data.NetworkInfo.fromObject(object.networks[i]);
                    }
                }
                return message;
            };
    
            /**
             * Creates a plain object from a NetworkSettings message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.NetworkSettings
             * @static
             * @param {fk_data.NetworkSettings} message NetworkSettings
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            NetworkSettings.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults)
                    object.networks = [];
                if (message.networks && message.networks.length) {
                    object.networks = [];
                    for (var j = 0; j < message.networks.length; ++j)
                        object.networks[j] = $root.fk_data.NetworkInfo.toObject(message.networks[j], options);
                }
                return object;
            };
    
            /**
             * Converts this NetworkSettings to JSON.
             * @function toJSON
             * @memberof fk_data.NetworkSettings
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            NetworkSettings.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return NetworkSettings;
        })();
    
        fk_data.LoraSettings = (function() {
    
            /**
             * Properties of a LoraSettings.
             * @memberof fk_data
             * @interface ILoraSettings
             * @property {Uint8Array|null} [deviceEui] LoraSettings deviceEui
             * @property {Uint8Array|null} [appKey] LoraSettings appKey
             * @property {Uint8Array|null} [joinEui] LoraSettings joinEui
             * @property {number|null} [frequencyBand] LoraSettings frequencyBand
             * @property {Uint8Array|null} [deviceAddress] LoraSettings deviceAddress
             * @property {Uint8Array|null} [networkSessionKey] LoraSettings networkSessionKey
             * @property {Uint8Array|null} [appSessionKey] LoraSettings appSessionKey
             * @property {number|null} [uplinkCounter] LoraSettings uplinkCounter
             * @property {number|null} [downlinkCounter] LoraSettings downlinkCounter
             * @property {number|null} [rxDelay1] LoraSettings rxDelay1
             * @property {number|null} [rxDelay2] LoraSettings rxDelay2
             */
    
            /**
             * Constructs a new LoraSettings.
             * @memberof fk_data
             * @classdesc Represents a LoraSettings.
             * @implements ILoraSettings
             * @constructor
             * @param {fk_data.ILoraSettings=} [properties] Properties to set
             */
            function LoraSettings(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * LoraSettings deviceEui.
             * @member {Uint8Array} deviceEui
             * @memberof fk_data.LoraSettings
             * @instance
             */
            LoraSettings.prototype.deviceEui = $util.newBuffer([]);
    
            /**
             * LoraSettings appKey.
             * @member {Uint8Array} appKey
             * @memberof fk_data.LoraSettings
             * @instance
             */
            LoraSettings.prototype.appKey = $util.newBuffer([]);
    
            /**
             * LoraSettings joinEui.
             * @member {Uint8Array} joinEui
             * @memberof fk_data.LoraSettings
             * @instance
             */
            LoraSettings.prototype.joinEui = $util.newBuffer([]);
    
            /**
             * LoraSettings frequencyBand.
             * @member {number} frequencyBand
             * @memberof fk_data.LoraSettings
             * @instance
             */
            LoraSettings.prototype.frequencyBand = 0;
    
            /**
             * LoraSettings deviceAddress.
             * @member {Uint8Array} deviceAddress
             * @memberof fk_data.LoraSettings
             * @instance
             */
            LoraSettings.prototype.deviceAddress = $util.newBuffer([]);
    
            /**
             * LoraSettings networkSessionKey.
             * @member {Uint8Array} networkSessionKey
             * @memberof fk_data.LoraSettings
             * @instance
             */
            LoraSettings.prototype.networkSessionKey = $util.newBuffer([]);
    
            /**
             * LoraSettings appSessionKey.
             * @member {Uint8Array} appSessionKey
             * @memberof fk_data.LoraSettings
             * @instance
             */
            LoraSettings.prototype.appSessionKey = $util.newBuffer([]);
    
            /**
             * LoraSettings uplinkCounter.
             * @member {number} uplinkCounter
             * @memberof fk_data.LoraSettings
             * @instance
             */
            LoraSettings.prototype.uplinkCounter = 0;
    
            /**
             * LoraSettings downlinkCounter.
             * @member {number} downlinkCounter
             * @memberof fk_data.LoraSettings
             * @instance
             */
            LoraSettings.prototype.downlinkCounter = 0;
    
            /**
             * LoraSettings rxDelay1.
             * @member {number} rxDelay1
             * @memberof fk_data.LoraSettings
             * @instance
             */
            LoraSettings.prototype.rxDelay1 = 0;
    
            /**
             * LoraSettings rxDelay2.
             * @member {number} rxDelay2
             * @memberof fk_data.LoraSettings
             * @instance
             */
            LoraSettings.prototype.rxDelay2 = 0;
    
            /**
             * Creates a new LoraSettings instance using the specified properties.
             * @function create
             * @memberof fk_data.LoraSettings
             * @static
             * @param {fk_data.ILoraSettings=} [properties] Properties to set
             * @returns {fk_data.LoraSettings} LoraSettings instance
             */
            LoraSettings.create = function create(properties) {
                return new LoraSettings(properties);
            };
    
            /**
             * Encodes the specified LoraSettings message. Does not implicitly {@link fk_data.LoraSettings.verify|verify} messages.
             * @function encode
             * @memberof fk_data.LoraSettings
             * @static
             * @param {fk_data.ILoraSettings} message LoraSettings message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            LoraSettings.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.deviceEui != null && message.hasOwnProperty("deviceEui"))
                    writer.uint32(/* id 1, wireType 2 =*/10).bytes(message.deviceEui);
                if (message.appKey != null && message.hasOwnProperty("appKey"))
                    writer.uint32(/* id 2, wireType 2 =*/18).bytes(message.appKey);
                if (message.joinEui != null && message.hasOwnProperty("joinEui"))
                    writer.uint32(/* id 3, wireType 2 =*/26).bytes(message.joinEui);
                if (message.frequencyBand != null && message.hasOwnProperty("frequencyBand"))
                    writer.uint32(/* id 4, wireType 0 =*/32).uint32(message.frequencyBand);
                if (message.deviceAddress != null && message.hasOwnProperty("deviceAddress"))
                    writer.uint32(/* id 5, wireType 2 =*/42).bytes(message.deviceAddress);
                if (message.networkSessionKey != null && message.hasOwnProperty("networkSessionKey"))
                    writer.uint32(/* id 6, wireType 2 =*/50).bytes(message.networkSessionKey);
                if (message.appSessionKey != null && message.hasOwnProperty("appSessionKey"))
                    writer.uint32(/* id 7, wireType 2 =*/58).bytes(message.appSessionKey);
                if (message.uplinkCounter != null && message.hasOwnProperty("uplinkCounter"))
                    writer.uint32(/* id 8, wireType 0 =*/64).uint32(message.uplinkCounter);
                if (message.downlinkCounter != null && message.hasOwnProperty("downlinkCounter"))
                    writer.uint32(/* id 9, wireType 0 =*/72).uint32(message.downlinkCounter);
                if (message.rxDelay1 != null && message.hasOwnProperty("rxDelay1"))
                    writer.uint32(/* id 10, wireType 0 =*/80).uint32(message.rxDelay1);
                if (message.rxDelay2 != null && message.hasOwnProperty("rxDelay2"))
                    writer.uint32(/* id 11, wireType 0 =*/88).uint32(message.rxDelay2);
                return writer;
            };
    
            /**
             * Encodes the specified LoraSettings message, length delimited. Does not implicitly {@link fk_data.LoraSettings.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.LoraSettings
             * @static
             * @param {fk_data.ILoraSettings} message LoraSettings message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            LoraSettings.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a LoraSettings message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.LoraSettings
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.LoraSettings} LoraSettings
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            LoraSettings.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.LoraSettings();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.deviceEui = reader.bytes();
                        break;
                    case 2:
                        message.appKey = reader.bytes();
                        break;
                    case 3:
                        message.joinEui = reader.bytes();
                        break;
                    case 4:
                        message.frequencyBand = reader.uint32();
                        break;
                    case 5:
                        message.deviceAddress = reader.bytes();
                        break;
                    case 6:
                        message.networkSessionKey = reader.bytes();
                        break;
                    case 7:
                        message.appSessionKey = reader.bytes();
                        break;
                    case 8:
                        message.uplinkCounter = reader.uint32();
                        break;
                    case 9:
                        message.downlinkCounter = reader.uint32();
                        break;
                    case 10:
                        message.rxDelay1 = reader.uint32();
                        break;
                    case 11:
                        message.rxDelay2 = reader.uint32();
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
             * @memberof fk_data.LoraSettings
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.LoraSettings} LoraSettings
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
             * @memberof fk_data.LoraSettings
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            LoraSettings.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.deviceEui != null && message.hasOwnProperty("deviceEui"))
                    if (!(message.deviceEui && typeof message.deviceEui.length === "number" || $util.isString(message.deviceEui)))
                        return "deviceEui: buffer expected";
                if (message.appKey != null && message.hasOwnProperty("appKey"))
                    if (!(message.appKey && typeof message.appKey.length === "number" || $util.isString(message.appKey)))
                        return "appKey: buffer expected";
                if (message.joinEui != null && message.hasOwnProperty("joinEui"))
                    if (!(message.joinEui && typeof message.joinEui.length === "number" || $util.isString(message.joinEui)))
                        return "joinEui: buffer expected";
                if (message.frequencyBand != null && message.hasOwnProperty("frequencyBand"))
                    if (!$util.isInteger(message.frequencyBand))
                        return "frequencyBand: integer expected";
                if (message.deviceAddress != null && message.hasOwnProperty("deviceAddress"))
                    if (!(message.deviceAddress && typeof message.deviceAddress.length === "number" || $util.isString(message.deviceAddress)))
                        return "deviceAddress: buffer expected";
                if (message.networkSessionKey != null && message.hasOwnProperty("networkSessionKey"))
                    if (!(message.networkSessionKey && typeof message.networkSessionKey.length === "number" || $util.isString(message.networkSessionKey)))
                        return "networkSessionKey: buffer expected";
                if (message.appSessionKey != null && message.hasOwnProperty("appSessionKey"))
                    if (!(message.appSessionKey && typeof message.appSessionKey.length === "number" || $util.isString(message.appSessionKey)))
                        return "appSessionKey: buffer expected";
                if (message.uplinkCounter != null && message.hasOwnProperty("uplinkCounter"))
                    if (!$util.isInteger(message.uplinkCounter))
                        return "uplinkCounter: integer expected";
                if (message.downlinkCounter != null && message.hasOwnProperty("downlinkCounter"))
                    if (!$util.isInteger(message.downlinkCounter))
                        return "downlinkCounter: integer expected";
                if (message.rxDelay1 != null && message.hasOwnProperty("rxDelay1"))
                    if (!$util.isInteger(message.rxDelay1))
                        return "rxDelay1: integer expected";
                if (message.rxDelay2 != null && message.hasOwnProperty("rxDelay2"))
                    if (!$util.isInteger(message.rxDelay2))
                        return "rxDelay2: integer expected";
                return null;
            };
    
            /**
             * Creates a LoraSettings message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.LoraSettings
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.LoraSettings} LoraSettings
             */
            LoraSettings.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.LoraSettings)
                    return object;
                var message = new $root.fk_data.LoraSettings();
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
                if (object.frequencyBand != null)
                    message.frequencyBand = object.frequencyBand >>> 0;
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
                if (object.uplinkCounter != null)
                    message.uplinkCounter = object.uplinkCounter >>> 0;
                if (object.downlinkCounter != null)
                    message.downlinkCounter = object.downlinkCounter >>> 0;
                if (object.rxDelay1 != null)
                    message.rxDelay1 = object.rxDelay1 >>> 0;
                if (object.rxDelay2 != null)
                    message.rxDelay2 = object.rxDelay2 >>> 0;
                return message;
            };
    
            /**
             * Creates a plain object from a LoraSettings message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.LoraSettings
             * @static
             * @param {fk_data.LoraSettings} message LoraSettings
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            LoraSettings.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.deviceEui = options.bytes === String ? "" : [];
                    object.appKey = options.bytes === String ? "" : [];
                    object.joinEui = options.bytes === String ? "" : [];
                    object.frequencyBand = 0;
                    object.deviceAddress = options.bytes === String ? "" : [];
                    object.networkSessionKey = options.bytes === String ? "" : [];
                    object.appSessionKey = options.bytes === String ? "" : [];
                    object.uplinkCounter = 0;
                    object.downlinkCounter = 0;
                    object.rxDelay1 = 0;
                    object.rxDelay2 = 0;
                }
                if (message.deviceEui != null && message.hasOwnProperty("deviceEui"))
                    object.deviceEui = options.bytes === String ? $util.base64.encode(message.deviceEui, 0, message.deviceEui.length) : options.bytes === Array ? Array.prototype.slice.call(message.deviceEui) : message.deviceEui;
                if (message.appKey != null && message.hasOwnProperty("appKey"))
                    object.appKey = options.bytes === String ? $util.base64.encode(message.appKey, 0, message.appKey.length) : options.bytes === Array ? Array.prototype.slice.call(message.appKey) : message.appKey;
                if (message.joinEui != null && message.hasOwnProperty("joinEui"))
                    object.joinEui = options.bytes === String ? $util.base64.encode(message.joinEui, 0, message.joinEui.length) : options.bytes === Array ? Array.prototype.slice.call(message.joinEui) : message.joinEui;
                if (message.frequencyBand != null && message.hasOwnProperty("frequencyBand"))
                    object.frequencyBand = message.frequencyBand;
                if (message.deviceAddress != null && message.hasOwnProperty("deviceAddress"))
                    object.deviceAddress = options.bytes === String ? $util.base64.encode(message.deviceAddress, 0, message.deviceAddress.length) : options.bytes === Array ? Array.prototype.slice.call(message.deviceAddress) : message.deviceAddress;
                if (message.networkSessionKey != null && message.hasOwnProperty("networkSessionKey"))
                    object.networkSessionKey = options.bytes === String ? $util.base64.encode(message.networkSessionKey, 0, message.networkSessionKey.length) : options.bytes === Array ? Array.prototype.slice.call(message.networkSessionKey) : message.networkSessionKey;
                if (message.appSessionKey != null && message.hasOwnProperty("appSessionKey"))
                    object.appSessionKey = options.bytes === String ? $util.base64.encode(message.appSessionKey, 0, message.appSessionKey.length) : options.bytes === Array ? Array.prototype.slice.call(message.appSessionKey) : message.appSessionKey;
                if (message.uplinkCounter != null && message.hasOwnProperty("uplinkCounter"))
                    object.uplinkCounter = message.uplinkCounter;
                if (message.downlinkCounter != null && message.hasOwnProperty("downlinkCounter"))
                    object.downlinkCounter = message.downlinkCounter;
                if (message.rxDelay1 != null && message.hasOwnProperty("rxDelay1"))
                    object.rxDelay1 = message.rxDelay1;
                if (message.rxDelay2 != null && message.hasOwnProperty("rxDelay2"))
                    object.rxDelay2 = message.rxDelay2;
                return object;
            };
    
            /**
             * Converts this LoraSettings to JSON.
             * @function toJSON
             * @memberof fk_data.LoraSettings
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            LoraSettings.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return LoraSettings;
        })();
    
        fk_data.Fault = (function() {
    
            /**
             * Properties of a Fault.
             * @memberof fk_data
             * @interface IFault
             * @property {number|null} [time] Fault time
             * @property {number|null} [code] Fault code
             * @property {string|null} [description] Fault description
             * @property {Uint8Array|null} [debug] Fault debug
             */
    
            /**
             * Constructs a new Fault.
             * @memberof fk_data
             * @classdesc Represents a Fault.
             * @implements IFault
             * @constructor
             * @param {fk_data.IFault=} [properties] Properties to set
             */
            function Fault(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Fault time.
             * @member {number} time
             * @memberof fk_data.Fault
             * @instance
             */
            Fault.prototype.time = 0;
    
            /**
             * Fault code.
             * @member {number} code
             * @memberof fk_data.Fault
             * @instance
             */
            Fault.prototype.code = 0;
    
            /**
             * Fault description.
             * @member {string} description
             * @memberof fk_data.Fault
             * @instance
             */
            Fault.prototype.description = "";
    
            /**
             * Fault debug.
             * @member {Uint8Array} debug
             * @memberof fk_data.Fault
             * @instance
             */
            Fault.prototype.debug = $util.newBuffer([]);
    
            /**
             * Creates a new Fault instance using the specified properties.
             * @function create
             * @memberof fk_data.Fault
             * @static
             * @param {fk_data.IFault=} [properties] Properties to set
             * @returns {fk_data.Fault} Fault instance
             */
            Fault.create = function create(properties) {
                return new Fault(properties);
            };
    
            /**
             * Encodes the specified Fault message. Does not implicitly {@link fk_data.Fault.verify|verify} messages.
             * @function encode
             * @memberof fk_data.Fault
             * @static
             * @param {fk_data.IFault} message Fault message or plain object to encode
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
             * Encodes the specified Fault message, length delimited. Does not implicitly {@link fk_data.Fault.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.Fault
             * @static
             * @param {fk_data.IFault} message Fault message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Fault.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a Fault message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.Fault
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.Fault} Fault
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Fault.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.Fault();
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
             * @memberof fk_data.Fault
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.Fault} Fault
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
             * @memberof fk_data.Fault
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
             * @memberof fk_data.Fault
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.Fault} Fault
             */
            Fault.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.Fault)
                    return object;
                var message = new $root.fk_data.Fault();
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
             * @memberof fk_data.Fault
             * @static
             * @param {fk_data.Fault} message Fault
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
             * @memberof fk_data.Fault
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Fault.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Fault;
        })();
    
        fk_data.DataRecord = (function() {
    
            /**
             * Properties of a DataRecord.
             * @memberof fk_data
             * @interface IDataRecord
             * @property {fk_data.ILoggedReading|null} [loggedReading] DataRecord loggedReading
             * @property {fk_data.IMetadata|null} [metadata] DataRecord metadata
             * @property {fk_data.ILogMessage|null} [log] DataRecord log
             * @property {Array.<fk_data.ILogMessage>|null} [logs] DataRecord logs
             * @property {fk_data.IStatus|null} [status] DataRecord status
             * @property {fk_data.IReadings|null} [readings] DataRecord readings
             * @property {Array.<fk_data.IModuleInfo>|null} [modules] DataRecord modules
             * @property {fk_data.ISchedule|null} [schedule] DataRecord schedule
             * @property {number|Long|null} [meta] DataRecord meta
             * @property {fk_data.IIdentity|null} [identity] DataRecord identity
             * @property {fk_data.ICondition|null} [condition] DataRecord condition
             * @property {fk_data.ILoraSettings|null} [lora] DataRecord lora
             * @property {fk_data.INetworkSettings|null} [network] DataRecord network
             * @property {fk_data.ITransmissionSettings|null} [transmission] DataRecord transmission
             * @property {Array.<fk_data.IFault>|null} [faults] DataRecord faults
             */
    
            /**
             * Constructs a new DataRecord.
             * @memberof fk_data
             * @classdesc I may break this into a MetaRecord.
             * @implements IDataRecord
             * @constructor
             * @param {fk_data.IDataRecord=} [properties] Properties to set
             */
            function DataRecord(properties) {
                this.logs = [];
                this.modules = [];
                this.faults = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * DataRecord loggedReading.
             * @member {fk_data.ILoggedReading|null|undefined} loggedReading
             * @memberof fk_data.DataRecord
             * @instance
             */
            DataRecord.prototype.loggedReading = null;
    
            /**
             * DataRecord metadata.
             * @member {fk_data.IMetadata|null|undefined} metadata
             * @memberof fk_data.DataRecord
             * @instance
             */
            DataRecord.prototype.metadata = null;
    
            /**
             * DataRecord log.
             * @member {fk_data.ILogMessage|null|undefined} log
             * @memberof fk_data.DataRecord
             * @instance
             */
            DataRecord.prototype.log = null;
    
            /**
             * DataRecord logs.
             * @member {Array.<fk_data.ILogMessage>} logs
             * @memberof fk_data.DataRecord
             * @instance
             */
            DataRecord.prototype.logs = $util.emptyArray;
    
            /**
             * DataRecord status.
             * @member {fk_data.IStatus|null|undefined} status
             * @memberof fk_data.DataRecord
             * @instance
             */
            DataRecord.prototype.status = null;
    
            /**
             * DataRecord readings.
             * @member {fk_data.IReadings|null|undefined} readings
             * @memberof fk_data.DataRecord
             * @instance
             */
            DataRecord.prototype.readings = null;
    
            /**
             * DataRecord modules.
             * @member {Array.<fk_data.IModuleInfo>} modules
             * @memberof fk_data.DataRecord
             * @instance
             */
            DataRecord.prototype.modules = $util.emptyArray;
    
            /**
             * DataRecord schedule.
             * @member {fk_data.ISchedule|null|undefined} schedule
             * @memberof fk_data.DataRecord
             * @instance
             */
            DataRecord.prototype.schedule = null;
    
            /**
             * DataRecord meta.
             * @member {number|Long} meta
             * @memberof fk_data.DataRecord
             * @instance
             */
            DataRecord.prototype.meta = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * DataRecord identity.
             * @member {fk_data.IIdentity|null|undefined} identity
             * @memberof fk_data.DataRecord
             * @instance
             */
            DataRecord.prototype.identity = null;
    
            /**
             * DataRecord condition.
             * @member {fk_data.ICondition|null|undefined} condition
             * @memberof fk_data.DataRecord
             * @instance
             */
            DataRecord.prototype.condition = null;
    
            /**
             * DataRecord lora.
             * @member {fk_data.ILoraSettings|null|undefined} lora
             * @memberof fk_data.DataRecord
             * @instance
             */
            DataRecord.prototype.lora = null;
    
            /**
             * DataRecord network.
             * @member {fk_data.INetworkSettings|null|undefined} network
             * @memberof fk_data.DataRecord
             * @instance
             */
            DataRecord.prototype.network = null;
    
            /**
             * DataRecord transmission.
             * @member {fk_data.ITransmissionSettings|null|undefined} transmission
             * @memberof fk_data.DataRecord
             * @instance
             */
            DataRecord.prototype.transmission = null;
    
            /**
             * DataRecord faults.
             * @member {Array.<fk_data.IFault>} faults
             * @memberof fk_data.DataRecord
             * @instance
             */
            DataRecord.prototype.faults = $util.emptyArray;
    
            /**
             * Creates a new DataRecord instance using the specified properties.
             * @function create
             * @memberof fk_data.DataRecord
             * @static
             * @param {fk_data.IDataRecord=} [properties] Properties to set
             * @returns {fk_data.DataRecord} DataRecord instance
             */
            DataRecord.create = function create(properties) {
                return new DataRecord(properties);
            };
    
            /**
             * Encodes the specified DataRecord message. Does not implicitly {@link fk_data.DataRecord.verify|verify} messages.
             * @function encode
             * @memberof fk_data.DataRecord
             * @static
             * @param {fk_data.IDataRecord} message DataRecord message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            DataRecord.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.loggedReading != null && message.hasOwnProperty("loggedReading"))
                    $root.fk_data.LoggedReading.encode(message.loggedReading, writer.uint32(/* id 1, wireType 2 =*/10).fork()).ldelim();
                if (message.metadata != null && message.hasOwnProperty("metadata"))
                    $root.fk_data.Metadata.encode(message.metadata, writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
                if (message.log != null && message.hasOwnProperty("log"))
                    $root.fk_data.LogMessage.encode(message.log, writer.uint32(/* id 3, wireType 2 =*/26).fork()).ldelim();
                if (message.status != null && message.hasOwnProperty("status"))
                    $root.fk_data.Status.encode(message.status, writer.uint32(/* id 4, wireType 2 =*/34).fork()).ldelim();
                if (message.readings != null && message.hasOwnProperty("readings"))
                    $root.fk_data.Readings.encode(message.readings, writer.uint32(/* id 5, wireType 2 =*/42).fork()).ldelim();
                if (message.modules != null && message.modules.length)
                    for (var i = 0; i < message.modules.length; ++i)
                        $root.fk_data.ModuleInfo.encode(message.modules[i], writer.uint32(/* id 6, wireType 2 =*/50).fork()).ldelim();
                if (message.schedule != null && message.hasOwnProperty("schedule"))
                    $root.fk_data.Schedule.encode(message.schedule, writer.uint32(/* id 7, wireType 2 =*/58).fork()).ldelim();
                if (message.meta != null && message.hasOwnProperty("meta"))
                    writer.uint32(/* id 8, wireType 0 =*/64).uint64(message.meta);
                if (message.identity != null && message.hasOwnProperty("identity"))
                    $root.fk_data.Identity.encode(message.identity, writer.uint32(/* id 9, wireType 2 =*/74).fork()).ldelim();
                if (message.condition != null && message.hasOwnProperty("condition"))
                    $root.fk_data.Condition.encode(message.condition, writer.uint32(/* id 10, wireType 2 =*/82).fork()).ldelim();
                if (message.lora != null && message.hasOwnProperty("lora"))
                    $root.fk_data.LoraSettings.encode(message.lora, writer.uint32(/* id 11, wireType 2 =*/90).fork()).ldelim();
                if (message.network != null && message.hasOwnProperty("network"))
                    $root.fk_data.NetworkSettings.encode(message.network, writer.uint32(/* id 12, wireType 2 =*/98).fork()).ldelim();
                if (message.logs != null && message.logs.length)
                    for (var i = 0; i < message.logs.length; ++i)
                        $root.fk_data.LogMessage.encode(message.logs[i], writer.uint32(/* id 13, wireType 2 =*/106).fork()).ldelim();
                if (message.transmission != null && message.hasOwnProperty("transmission"))
                    $root.fk_data.TransmissionSettings.encode(message.transmission, writer.uint32(/* id 14, wireType 2 =*/114).fork()).ldelim();
                if (message.faults != null && message.faults.length)
                    for (var i = 0; i < message.faults.length; ++i)
                        $root.fk_data.Fault.encode(message.faults[i], writer.uint32(/* id 15, wireType 2 =*/122).fork()).ldelim();
                return writer;
            };
    
            /**
             * Encodes the specified DataRecord message, length delimited. Does not implicitly {@link fk_data.DataRecord.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.DataRecord
             * @static
             * @param {fk_data.IDataRecord} message DataRecord message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            DataRecord.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a DataRecord message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.DataRecord
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.DataRecord} DataRecord
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            DataRecord.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.DataRecord();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.loggedReading = $root.fk_data.LoggedReading.decode(reader, reader.uint32());
                        break;
                    case 2:
                        message.metadata = $root.fk_data.Metadata.decode(reader, reader.uint32());
                        break;
                    case 3:
                        message.log = $root.fk_data.LogMessage.decode(reader, reader.uint32());
                        break;
                    case 13:
                        if (!(message.logs && message.logs.length))
                            message.logs = [];
                        message.logs.push($root.fk_data.LogMessage.decode(reader, reader.uint32()));
                        break;
                    case 4:
                        message.status = $root.fk_data.Status.decode(reader, reader.uint32());
                        break;
                    case 5:
                        message.readings = $root.fk_data.Readings.decode(reader, reader.uint32());
                        break;
                    case 6:
                        if (!(message.modules && message.modules.length))
                            message.modules = [];
                        message.modules.push($root.fk_data.ModuleInfo.decode(reader, reader.uint32()));
                        break;
                    case 7:
                        message.schedule = $root.fk_data.Schedule.decode(reader, reader.uint32());
                        break;
                    case 8:
                        message.meta = reader.uint64();
                        break;
                    case 9:
                        message.identity = $root.fk_data.Identity.decode(reader, reader.uint32());
                        break;
                    case 10:
                        message.condition = $root.fk_data.Condition.decode(reader, reader.uint32());
                        break;
                    case 11:
                        message.lora = $root.fk_data.LoraSettings.decode(reader, reader.uint32());
                        break;
                    case 12:
                        message.network = $root.fk_data.NetworkSettings.decode(reader, reader.uint32());
                        break;
                    case 14:
                        message.transmission = $root.fk_data.TransmissionSettings.decode(reader, reader.uint32());
                        break;
                    case 15:
                        if (!(message.faults && message.faults.length))
                            message.faults = [];
                        message.faults.push($root.fk_data.Fault.decode(reader, reader.uint32()));
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a DataRecord message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_data.DataRecord
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.DataRecord} DataRecord
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            DataRecord.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a DataRecord message.
             * @function verify
             * @memberof fk_data.DataRecord
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            DataRecord.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.loggedReading != null && message.hasOwnProperty("loggedReading")) {
                    var error = $root.fk_data.LoggedReading.verify(message.loggedReading);
                    if (error)
                        return "loggedReading." + error;
                }
                if (message.metadata != null && message.hasOwnProperty("metadata")) {
                    var error = $root.fk_data.Metadata.verify(message.metadata);
                    if (error)
                        return "metadata." + error;
                }
                if (message.log != null && message.hasOwnProperty("log")) {
                    var error = $root.fk_data.LogMessage.verify(message.log);
                    if (error)
                        return "log." + error;
                }
                if (message.logs != null && message.hasOwnProperty("logs")) {
                    if (!Array.isArray(message.logs))
                        return "logs: array expected";
                    for (var i = 0; i < message.logs.length; ++i) {
                        var error = $root.fk_data.LogMessage.verify(message.logs[i]);
                        if (error)
                            return "logs." + error;
                    }
                }
                if (message.status != null && message.hasOwnProperty("status")) {
                    var error = $root.fk_data.Status.verify(message.status);
                    if (error)
                        return "status." + error;
                }
                if (message.readings != null && message.hasOwnProperty("readings")) {
                    var error = $root.fk_data.Readings.verify(message.readings);
                    if (error)
                        return "readings." + error;
                }
                if (message.modules != null && message.hasOwnProperty("modules")) {
                    if (!Array.isArray(message.modules))
                        return "modules: array expected";
                    for (var i = 0; i < message.modules.length; ++i) {
                        var error = $root.fk_data.ModuleInfo.verify(message.modules[i]);
                        if (error)
                            return "modules." + error;
                    }
                }
                if (message.schedule != null && message.hasOwnProperty("schedule")) {
                    var error = $root.fk_data.Schedule.verify(message.schedule);
                    if (error)
                        return "schedule." + error;
                }
                if (message.meta != null && message.hasOwnProperty("meta"))
                    if (!$util.isInteger(message.meta) && !(message.meta && $util.isInteger(message.meta.low) && $util.isInteger(message.meta.high)))
                        return "meta: integer|Long expected";
                if (message.identity != null && message.hasOwnProperty("identity")) {
                    var error = $root.fk_data.Identity.verify(message.identity);
                    if (error)
                        return "identity." + error;
                }
                if (message.condition != null && message.hasOwnProperty("condition")) {
                    var error = $root.fk_data.Condition.verify(message.condition);
                    if (error)
                        return "condition." + error;
                }
                if (message.lora != null && message.hasOwnProperty("lora")) {
                    var error = $root.fk_data.LoraSettings.verify(message.lora);
                    if (error)
                        return "lora." + error;
                }
                if (message.network != null && message.hasOwnProperty("network")) {
                    var error = $root.fk_data.NetworkSettings.verify(message.network);
                    if (error)
                        return "network." + error;
                }
                if (message.transmission != null && message.hasOwnProperty("transmission")) {
                    var error = $root.fk_data.TransmissionSettings.verify(message.transmission);
                    if (error)
                        return "transmission." + error;
                }
                if (message.faults != null && message.hasOwnProperty("faults")) {
                    if (!Array.isArray(message.faults))
                        return "faults: array expected";
                    for (var i = 0; i < message.faults.length; ++i) {
                        var error = $root.fk_data.Fault.verify(message.faults[i]);
                        if (error)
                            return "faults." + error;
                    }
                }
                return null;
            };
    
            /**
             * Creates a DataRecord message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.DataRecord
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.DataRecord} DataRecord
             */
            DataRecord.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.DataRecord)
                    return object;
                var message = new $root.fk_data.DataRecord();
                if (object.loggedReading != null) {
                    if (typeof object.loggedReading !== "object")
                        throw TypeError(".fk_data.DataRecord.loggedReading: object expected");
                    message.loggedReading = $root.fk_data.LoggedReading.fromObject(object.loggedReading);
                }
                if (object.metadata != null) {
                    if (typeof object.metadata !== "object")
                        throw TypeError(".fk_data.DataRecord.metadata: object expected");
                    message.metadata = $root.fk_data.Metadata.fromObject(object.metadata);
                }
                if (object.log != null) {
                    if (typeof object.log !== "object")
                        throw TypeError(".fk_data.DataRecord.log: object expected");
                    message.log = $root.fk_data.LogMessage.fromObject(object.log);
                }
                if (object.logs) {
                    if (!Array.isArray(object.logs))
                        throw TypeError(".fk_data.DataRecord.logs: array expected");
                    message.logs = [];
                    for (var i = 0; i < object.logs.length; ++i) {
                        if (typeof object.logs[i] !== "object")
                            throw TypeError(".fk_data.DataRecord.logs: object expected");
                        message.logs[i] = $root.fk_data.LogMessage.fromObject(object.logs[i]);
                    }
                }
                if (object.status != null) {
                    if (typeof object.status !== "object")
                        throw TypeError(".fk_data.DataRecord.status: object expected");
                    message.status = $root.fk_data.Status.fromObject(object.status);
                }
                if (object.readings != null) {
                    if (typeof object.readings !== "object")
                        throw TypeError(".fk_data.DataRecord.readings: object expected");
                    message.readings = $root.fk_data.Readings.fromObject(object.readings);
                }
                if (object.modules) {
                    if (!Array.isArray(object.modules))
                        throw TypeError(".fk_data.DataRecord.modules: array expected");
                    message.modules = [];
                    for (var i = 0; i < object.modules.length; ++i) {
                        if (typeof object.modules[i] !== "object")
                            throw TypeError(".fk_data.DataRecord.modules: object expected");
                        message.modules[i] = $root.fk_data.ModuleInfo.fromObject(object.modules[i]);
                    }
                }
                if (object.schedule != null) {
                    if (typeof object.schedule !== "object")
                        throw TypeError(".fk_data.DataRecord.schedule: object expected");
                    message.schedule = $root.fk_data.Schedule.fromObject(object.schedule);
                }
                if (object.meta != null)
                    if ($util.Long)
                        (message.meta = $util.Long.fromValue(object.meta)).unsigned = true;
                    else if (typeof object.meta === "string")
                        message.meta = parseInt(object.meta, 10);
                    else if (typeof object.meta === "number")
                        message.meta = object.meta;
                    else if (typeof object.meta === "object")
                        message.meta = new $util.LongBits(object.meta.low >>> 0, object.meta.high >>> 0).toNumber(true);
                if (object.identity != null) {
                    if (typeof object.identity !== "object")
                        throw TypeError(".fk_data.DataRecord.identity: object expected");
                    message.identity = $root.fk_data.Identity.fromObject(object.identity);
                }
                if (object.condition != null) {
                    if (typeof object.condition !== "object")
                        throw TypeError(".fk_data.DataRecord.condition: object expected");
                    message.condition = $root.fk_data.Condition.fromObject(object.condition);
                }
                if (object.lora != null) {
                    if (typeof object.lora !== "object")
                        throw TypeError(".fk_data.DataRecord.lora: object expected");
                    message.lora = $root.fk_data.LoraSettings.fromObject(object.lora);
                }
                if (object.network != null) {
                    if (typeof object.network !== "object")
                        throw TypeError(".fk_data.DataRecord.network: object expected");
                    message.network = $root.fk_data.NetworkSettings.fromObject(object.network);
                }
                if (object.transmission != null) {
                    if (typeof object.transmission !== "object")
                        throw TypeError(".fk_data.DataRecord.transmission: object expected");
                    message.transmission = $root.fk_data.TransmissionSettings.fromObject(object.transmission);
                }
                if (object.faults) {
                    if (!Array.isArray(object.faults))
                        throw TypeError(".fk_data.DataRecord.faults: array expected");
                    message.faults = [];
                    for (var i = 0; i < object.faults.length; ++i) {
                        if (typeof object.faults[i] !== "object")
                            throw TypeError(".fk_data.DataRecord.faults: object expected");
                        message.faults[i] = $root.fk_data.Fault.fromObject(object.faults[i]);
                    }
                }
                return message;
            };
    
            /**
             * Creates a plain object from a DataRecord message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.DataRecord
             * @static
             * @param {fk_data.DataRecord} message DataRecord
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            DataRecord.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults) {
                    object.modules = [];
                    object.logs = [];
                    object.faults = [];
                }
                if (options.defaults) {
                    object.loggedReading = null;
                    object.metadata = null;
                    object.log = null;
                    object.status = null;
                    object.readings = null;
                    object.schedule = null;
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.meta = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.meta = options.longs === String ? "0" : 0;
                    object.identity = null;
                    object.condition = null;
                    object.lora = null;
                    object.network = null;
                    object.transmission = null;
                }
                if (message.loggedReading != null && message.hasOwnProperty("loggedReading"))
                    object.loggedReading = $root.fk_data.LoggedReading.toObject(message.loggedReading, options);
                if (message.metadata != null && message.hasOwnProperty("metadata"))
                    object.metadata = $root.fk_data.Metadata.toObject(message.metadata, options);
                if (message.log != null && message.hasOwnProperty("log"))
                    object.log = $root.fk_data.LogMessage.toObject(message.log, options);
                if (message.status != null && message.hasOwnProperty("status"))
                    object.status = $root.fk_data.Status.toObject(message.status, options);
                if (message.readings != null && message.hasOwnProperty("readings"))
                    object.readings = $root.fk_data.Readings.toObject(message.readings, options);
                if (message.modules && message.modules.length) {
                    object.modules = [];
                    for (var j = 0; j < message.modules.length; ++j)
                        object.modules[j] = $root.fk_data.ModuleInfo.toObject(message.modules[j], options);
                }
                if (message.schedule != null && message.hasOwnProperty("schedule"))
                    object.schedule = $root.fk_data.Schedule.toObject(message.schedule, options);
                if (message.meta != null && message.hasOwnProperty("meta"))
                    if (typeof message.meta === "number")
                        object.meta = options.longs === String ? String(message.meta) : message.meta;
                    else
                        object.meta = options.longs === String ? $util.Long.prototype.toString.call(message.meta) : options.longs === Number ? new $util.LongBits(message.meta.low >>> 0, message.meta.high >>> 0).toNumber(true) : message.meta;
                if (message.identity != null && message.hasOwnProperty("identity"))
                    object.identity = $root.fk_data.Identity.toObject(message.identity, options);
                if (message.condition != null && message.hasOwnProperty("condition"))
                    object.condition = $root.fk_data.Condition.toObject(message.condition, options);
                if (message.lora != null && message.hasOwnProperty("lora"))
                    object.lora = $root.fk_data.LoraSettings.toObject(message.lora, options);
                if (message.network != null && message.hasOwnProperty("network"))
                    object.network = $root.fk_data.NetworkSettings.toObject(message.network, options);
                if (message.logs && message.logs.length) {
                    object.logs = [];
                    for (var j = 0; j < message.logs.length; ++j)
                        object.logs[j] = $root.fk_data.LogMessage.toObject(message.logs[j], options);
                }
                if (message.transmission != null && message.hasOwnProperty("transmission"))
                    object.transmission = $root.fk_data.TransmissionSettings.toObject(message.transmission, options);
                if (message.faults && message.faults.length) {
                    object.faults = [];
                    for (var j = 0; j < message.faults.length; ++j)
                        object.faults[j] = $root.fk_data.Fault.toObject(message.faults[j], options);
                }
                return object;
            };
    
            /**
             * Converts this DataRecord to JSON.
             * @function toJSON
             * @memberof fk_data.DataRecord
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            DataRecord.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return DataRecord;
        })();
    
        /**
         * SignedRecordKind enum.
         * @name fk_data.SignedRecordKind
         * @enum {string}
         * @property {number} SIGNED_RECORD_KIND_NONE=0 SIGNED_RECORD_KIND_NONE value
         * @property {number} SIGNED_RECORD_KIND_MODULES=1 SIGNED_RECORD_KIND_MODULES value
         * @property {number} SIGNED_RECORD_KIND_SCHEDULE=2 SIGNED_RECORD_KIND_SCHEDULE value
         * @property {number} SIGNED_RECORD_KIND_STATE=3 SIGNED_RECORD_KIND_STATE value
         * @property {number} SIGNED_RECORD_KIND_RAW_STATE=4 SIGNED_RECORD_KIND_RAW_STATE value
         * @property {number} SIGNED_RECORD_KIND_FAULTS=5 SIGNED_RECORD_KIND_FAULTS value
         * @property {number} SIGNED_RECORD_KIND_OTHER=255 SIGNED_RECORD_KIND_OTHER value
         */
        fk_data.SignedRecordKind = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "SIGNED_RECORD_KIND_NONE"] = 0;
            values[valuesById[1] = "SIGNED_RECORD_KIND_MODULES"] = 1;
            values[valuesById[2] = "SIGNED_RECORD_KIND_SCHEDULE"] = 2;
            values[valuesById[3] = "SIGNED_RECORD_KIND_STATE"] = 3;
            values[valuesById[4] = "SIGNED_RECORD_KIND_RAW_STATE"] = 4;
            values[valuesById[5] = "SIGNED_RECORD_KIND_FAULTS"] = 5;
            values[valuesById[255] = "SIGNED_RECORD_KIND_OTHER"] = 255;
            return values;
        })();
    
        fk_data.SignedRecord = (function() {
    
            /**
             * Properties of a SignedRecord.
             * @memberof fk_data
             * @interface ISignedRecord
             * @property {fk_data.SignedRecordKind|null} [kind] SignedRecord kind
             * @property {number|Long|null} [time] SignedRecord time
             * @property {Uint8Array|null} [data] SignedRecord data
             * @property {Uint8Array|null} [hash] SignedRecord hash
             * @property {number|Long|null} [record] SignedRecord record
             */
    
            /**
             * Constructs a new SignedRecord.
             * @memberof fk_data
             * @classdesc Represents a SignedRecord.
             * @implements ISignedRecord
             * @constructor
             * @param {fk_data.ISignedRecord=} [properties] Properties to set
             */
            function SignedRecord(properties) {
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * SignedRecord kind.
             * @member {fk_data.SignedRecordKind} kind
             * @memberof fk_data.SignedRecord
             * @instance
             */
            SignedRecord.prototype.kind = 0;
    
            /**
             * SignedRecord time.
             * @member {number|Long} time
             * @memberof fk_data.SignedRecord
             * @instance
             */
            SignedRecord.prototype.time = $util.Long ? $util.Long.fromBits(0,0,false) : 0;
    
            /**
             * SignedRecord data.
             * @member {Uint8Array} data
             * @memberof fk_data.SignedRecord
             * @instance
             */
            SignedRecord.prototype.data = $util.newBuffer([]);
    
            /**
             * SignedRecord hash.
             * @member {Uint8Array} hash
             * @memberof fk_data.SignedRecord
             * @instance
             */
            SignedRecord.prototype.hash = $util.newBuffer([]);
    
            /**
             * SignedRecord record.
             * @member {number|Long} record
             * @memberof fk_data.SignedRecord
             * @instance
             */
            SignedRecord.prototype.record = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * Creates a new SignedRecord instance using the specified properties.
             * @function create
             * @memberof fk_data.SignedRecord
             * @static
             * @param {fk_data.ISignedRecord=} [properties] Properties to set
             * @returns {fk_data.SignedRecord} SignedRecord instance
             */
            SignedRecord.create = function create(properties) {
                return new SignedRecord(properties);
            };
    
            /**
             * Encodes the specified SignedRecord message. Does not implicitly {@link fk_data.SignedRecord.verify|verify} messages.
             * @function encode
             * @memberof fk_data.SignedRecord
             * @static
             * @param {fk_data.ISignedRecord} message SignedRecord message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            SignedRecord.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.kind != null && message.hasOwnProperty("kind"))
                    writer.uint32(/* id 1, wireType 0 =*/8).int32(message.kind);
                if (message.time != null && message.hasOwnProperty("time"))
                    writer.uint32(/* id 2, wireType 0 =*/16).int64(message.time);
                if (message.data != null && message.hasOwnProperty("data"))
                    writer.uint32(/* id 3, wireType 2 =*/26).bytes(message.data);
                if (message.hash != null && message.hasOwnProperty("hash"))
                    writer.uint32(/* id 4, wireType 2 =*/34).bytes(message.hash);
                if (message.record != null && message.hasOwnProperty("record"))
                    writer.uint32(/* id 5, wireType 0 =*/40).uint64(message.record);
                return writer;
            };
    
            /**
             * Encodes the specified SignedRecord message, length delimited. Does not implicitly {@link fk_data.SignedRecord.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.SignedRecord
             * @static
             * @param {fk_data.ISignedRecord} message SignedRecord message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            SignedRecord.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a SignedRecord message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.SignedRecord
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.SignedRecord} SignedRecord
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            SignedRecord.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.SignedRecord();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.kind = reader.int32();
                        break;
                    case 2:
                        message.time = reader.int64();
                        break;
                    case 3:
                        message.data = reader.bytes();
                        break;
                    case 4:
                        message.hash = reader.bytes();
                        break;
                    case 5:
                        message.record = reader.uint64();
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a SignedRecord message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_data.SignedRecord
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.SignedRecord} SignedRecord
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            SignedRecord.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a SignedRecord message.
             * @function verify
             * @memberof fk_data.SignedRecord
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            SignedRecord.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.kind != null && message.hasOwnProperty("kind"))
                    switch (message.kind) {
                    default:
                        return "kind: enum value expected";
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 255:
                        break;
                    }
                if (message.time != null && message.hasOwnProperty("time"))
                    if (!$util.isInteger(message.time) && !(message.time && $util.isInteger(message.time.low) && $util.isInteger(message.time.high)))
                        return "time: integer|Long expected";
                if (message.data != null && message.hasOwnProperty("data"))
                    if (!(message.data && typeof message.data.length === "number" || $util.isString(message.data)))
                        return "data: buffer expected";
                if (message.hash != null && message.hasOwnProperty("hash"))
                    if (!(message.hash && typeof message.hash.length === "number" || $util.isString(message.hash)))
                        return "hash: buffer expected";
                if (message.record != null && message.hasOwnProperty("record"))
                    if (!$util.isInteger(message.record) && !(message.record && $util.isInteger(message.record.low) && $util.isInteger(message.record.high)))
                        return "record: integer|Long expected";
                return null;
            };
    
            /**
             * Creates a SignedRecord message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.SignedRecord
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.SignedRecord} SignedRecord
             */
            SignedRecord.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.SignedRecord)
                    return object;
                var message = new $root.fk_data.SignedRecord();
                switch (object.kind) {
                case "SIGNED_RECORD_KIND_NONE":
                case 0:
                    message.kind = 0;
                    break;
                case "SIGNED_RECORD_KIND_MODULES":
                case 1:
                    message.kind = 1;
                    break;
                case "SIGNED_RECORD_KIND_SCHEDULE":
                case 2:
                    message.kind = 2;
                    break;
                case "SIGNED_RECORD_KIND_STATE":
                case 3:
                    message.kind = 3;
                    break;
                case "SIGNED_RECORD_KIND_RAW_STATE":
                case 4:
                    message.kind = 4;
                    break;
                case "SIGNED_RECORD_KIND_FAULTS":
                case 5:
                    message.kind = 5;
                    break;
                case "SIGNED_RECORD_KIND_OTHER":
                case 255:
                    message.kind = 255;
                    break;
                }
                if (object.time != null)
                    if ($util.Long)
                        (message.time = $util.Long.fromValue(object.time)).unsigned = false;
                    else if (typeof object.time === "string")
                        message.time = parseInt(object.time, 10);
                    else if (typeof object.time === "number")
                        message.time = object.time;
                    else if (typeof object.time === "object")
                        message.time = new $util.LongBits(object.time.low >>> 0, object.time.high >>> 0).toNumber();
                if (object.data != null)
                    if (typeof object.data === "string")
                        $util.base64.decode(object.data, message.data = $util.newBuffer($util.base64.length(object.data)), 0);
                    else if (object.data.length)
                        message.data = object.data;
                if (object.hash != null)
                    if (typeof object.hash === "string")
                        $util.base64.decode(object.hash, message.hash = $util.newBuffer($util.base64.length(object.hash)), 0);
                    else if (object.hash.length)
                        message.hash = object.hash;
                if (object.record != null)
                    if ($util.Long)
                        (message.record = $util.Long.fromValue(object.record)).unsigned = true;
                    else if (typeof object.record === "string")
                        message.record = parseInt(object.record, 10);
                    else if (typeof object.record === "number")
                        message.record = object.record;
                    else if (typeof object.record === "object")
                        message.record = new $util.LongBits(object.record.low >>> 0, object.record.high >>> 0).toNumber(true);
                return message;
            };
    
            /**
             * Creates a plain object from a SignedRecord message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.SignedRecord
             * @static
             * @param {fk_data.SignedRecord} message SignedRecord
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            SignedRecord.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.defaults) {
                    object.kind = options.enums === String ? "SIGNED_RECORD_KIND_NONE" : 0;
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, false);
                        object.time = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.time = options.longs === String ? "0" : 0;
                    object.data = options.bytes === String ? "" : [];
                    object.hash = options.bytes === String ? "" : [];
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.record = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.record = options.longs === String ? "0" : 0;
                }
                if (message.kind != null && message.hasOwnProperty("kind"))
                    object.kind = options.enums === String ? $root.fk_data.SignedRecordKind[message.kind] : message.kind;
                if (message.time != null && message.hasOwnProperty("time"))
                    if (typeof message.time === "number")
                        object.time = options.longs === String ? String(message.time) : message.time;
                    else
                        object.time = options.longs === String ? $util.Long.prototype.toString.call(message.time) : options.longs === Number ? new $util.LongBits(message.time.low >>> 0, message.time.high >>> 0).toNumber() : message.time;
                if (message.data != null && message.hasOwnProperty("data"))
                    object.data = options.bytes === String ? $util.base64.encode(message.data, 0, message.data.length) : options.bytes === Array ? Array.prototype.slice.call(message.data) : message.data;
                if (message.hash != null && message.hasOwnProperty("hash"))
                    object.hash = options.bytes === String ? $util.base64.encode(message.hash, 0, message.hash.length) : options.bytes === Array ? Array.prototype.slice.call(message.hash) : message.hash;
                if (message.record != null && message.hasOwnProperty("record"))
                    if (typeof message.record === "number")
                        object.record = options.longs === String ? String(message.record) : message.record;
                    else
                        object.record = options.longs === String ? $util.Long.prototype.toString.call(message.record) : options.longs === Number ? new $util.LongBits(message.record.low >>> 0, message.record.high >>> 0).toNumber(true) : message.record;
                return object;
            };
    
            /**
             * Converts this SignedRecord to JSON.
             * @function toJSON
             * @memberof fk_data.SignedRecord
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            SignedRecord.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return SignedRecord;
        })();
    
        fk_data.LoraRecord = (function() {
    
            /**
             * Properties of a LoraRecord.
             * @memberof fk_data
             * @interface ILoraRecord
             * @property {Uint8Array|null} [deviceId] LoraRecord deviceId
             * @property {number|Long|null} [time] LoraRecord time
             * @property {number|Long|null} [number] LoraRecord number
             * @property {number|null} [module] LoraRecord module
             * @property {number|Long|null} [sensor] LoraRecord sensor
             * @property {Array.<number>|null} [values] LoraRecord values
             * @property {Uint8Array|null} [data] LoraRecord data
             */
    
            /**
             * Constructs a new LoraRecord.
             * @memberof fk_data
             * @classdesc Represents a LoraRecord.
             * @implements ILoraRecord
             * @constructor
             * @param {fk_data.ILoraRecord=} [properties] Properties to set
             */
            function LoraRecord(properties) {
                this.values = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * LoraRecord deviceId.
             * @member {Uint8Array} deviceId
             * @memberof fk_data.LoraRecord
             * @instance
             */
            LoraRecord.prototype.deviceId = $util.newBuffer([]);
    
            /**
             * LoraRecord time.
             * @member {number|Long} time
             * @memberof fk_data.LoraRecord
             * @instance
             */
            LoraRecord.prototype.time = $util.Long ? $util.Long.fromBits(0,0,false) : 0;
    
            /**
             * LoraRecord number.
             * @member {number|Long} number
             * @memberof fk_data.LoraRecord
             * @instance
             */
            LoraRecord.prototype.number = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * LoraRecord module.
             * @member {number} module
             * @memberof fk_data.LoraRecord
             * @instance
             */
            LoraRecord.prototype.module = 0;
    
            /**
             * LoraRecord sensor.
             * @member {number|Long} sensor
             * @memberof fk_data.LoraRecord
             * @instance
             */
            LoraRecord.prototype.sensor = $util.Long ? $util.Long.fromBits(0,0,true) : 0;
    
            /**
             * LoraRecord values.
             * @member {Array.<number>} values
             * @memberof fk_data.LoraRecord
             * @instance
             */
            LoraRecord.prototype.values = $util.emptyArray;
    
            /**
             * LoraRecord data.
             * @member {Uint8Array} data
             * @memberof fk_data.LoraRecord
             * @instance
             */
            LoraRecord.prototype.data = $util.newBuffer([]);
    
            /**
             * Creates a new LoraRecord instance using the specified properties.
             * @function create
             * @memberof fk_data.LoraRecord
             * @static
             * @param {fk_data.ILoraRecord=} [properties] Properties to set
             * @returns {fk_data.LoraRecord} LoraRecord instance
             */
            LoraRecord.create = function create(properties) {
                return new LoraRecord(properties);
            };
    
            /**
             * Encodes the specified LoraRecord message. Does not implicitly {@link fk_data.LoraRecord.verify|verify} messages.
             * @function encode
             * @memberof fk_data.LoraRecord
             * @static
             * @param {fk_data.ILoraRecord} message LoraRecord message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            LoraRecord.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.deviceId != null && message.hasOwnProperty("deviceId"))
                    writer.uint32(/* id 1, wireType 2 =*/10).bytes(message.deviceId);
                if (message.time != null && message.hasOwnProperty("time"))
                    writer.uint32(/* id 2, wireType 0 =*/16).int64(message.time);
                if (message.number != null && message.hasOwnProperty("number"))
                    writer.uint32(/* id 3, wireType 0 =*/24).uint64(message.number);
                if (message.module != null && message.hasOwnProperty("module"))
                    writer.uint32(/* id 4, wireType 0 =*/32).uint32(message.module);
                if (message.sensor != null && message.hasOwnProperty("sensor"))
                    writer.uint32(/* id 5, wireType 0 =*/40).uint64(message.sensor);
                if (message.values != null && message.values.length) {
                    writer.uint32(/* id 6, wireType 2 =*/50).fork();
                    for (var i = 0; i < message.values.length; ++i)
                        writer.float(message.values[i]);
                    writer.ldelim();
                }
                if (message.data != null && message.hasOwnProperty("data"))
                    writer.uint32(/* id 7, wireType 2 =*/58).bytes(message.data);
                return writer;
            };
    
            /**
             * Encodes the specified LoraRecord message, length delimited. Does not implicitly {@link fk_data.LoraRecord.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.LoraRecord
             * @static
             * @param {fk_data.ILoraRecord} message LoraRecord message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            LoraRecord.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a LoraRecord message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.LoraRecord
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.LoraRecord} LoraRecord
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            LoraRecord.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.LoraRecord();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.deviceId = reader.bytes();
                        break;
                    case 2:
                        message.time = reader.int64();
                        break;
                    case 3:
                        message.number = reader.uint64();
                        break;
                    case 4:
                        message.module = reader.uint32();
                        break;
                    case 5:
                        message.sensor = reader.uint64();
                        break;
                    case 6:
                        if (!(message.values && message.values.length))
                            message.values = [];
                        if ((tag & 7) === 2) {
                            var end2 = reader.uint32() + reader.pos;
                            while (reader.pos < end2)
                                message.values.push(reader.float());
                        } else
                            message.values.push(reader.float());
                        break;
                    case 7:
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
             * Decodes a LoraRecord message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_data.LoraRecord
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.LoraRecord} LoraRecord
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            LoraRecord.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a LoraRecord message.
             * @function verify
             * @memberof fk_data.LoraRecord
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            LoraRecord.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.deviceId != null && message.hasOwnProperty("deviceId"))
                    if (!(message.deviceId && typeof message.deviceId.length === "number" || $util.isString(message.deviceId)))
                        return "deviceId: buffer expected";
                if (message.time != null && message.hasOwnProperty("time"))
                    if (!$util.isInteger(message.time) && !(message.time && $util.isInteger(message.time.low) && $util.isInteger(message.time.high)))
                        return "time: integer|Long expected";
                if (message.number != null && message.hasOwnProperty("number"))
                    if (!$util.isInteger(message.number) && !(message.number && $util.isInteger(message.number.low) && $util.isInteger(message.number.high)))
                        return "number: integer|Long expected";
                if (message.module != null && message.hasOwnProperty("module"))
                    if (!$util.isInteger(message.module))
                        return "module: integer expected";
                if (message.sensor != null && message.hasOwnProperty("sensor"))
                    if (!$util.isInteger(message.sensor) && !(message.sensor && $util.isInteger(message.sensor.low) && $util.isInteger(message.sensor.high)))
                        return "sensor: integer|Long expected";
                if (message.values != null && message.hasOwnProperty("values")) {
                    if (!Array.isArray(message.values))
                        return "values: array expected";
                    for (var i = 0; i < message.values.length; ++i)
                        if (typeof message.values[i] !== "number")
                            return "values: number[] expected";
                }
                if (message.data != null && message.hasOwnProperty("data"))
                    if (!(message.data && typeof message.data.length === "number" || $util.isString(message.data)))
                        return "data: buffer expected";
                return null;
            };
    
            /**
             * Creates a LoraRecord message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.LoraRecord
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.LoraRecord} LoraRecord
             */
            LoraRecord.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.LoraRecord)
                    return object;
                var message = new $root.fk_data.LoraRecord();
                if (object.deviceId != null)
                    if (typeof object.deviceId === "string")
                        $util.base64.decode(object.deviceId, message.deviceId = $util.newBuffer($util.base64.length(object.deviceId)), 0);
                    else if (object.deviceId.length)
                        message.deviceId = object.deviceId;
                if (object.time != null)
                    if ($util.Long)
                        (message.time = $util.Long.fromValue(object.time)).unsigned = false;
                    else if (typeof object.time === "string")
                        message.time = parseInt(object.time, 10);
                    else if (typeof object.time === "number")
                        message.time = object.time;
                    else if (typeof object.time === "object")
                        message.time = new $util.LongBits(object.time.low >>> 0, object.time.high >>> 0).toNumber();
                if (object.number != null)
                    if ($util.Long)
                        (message.number = $util.Long.fromValue(object.number)).unsigned = true;
                    else if (typeof object.number === "string")
                        message.number = parseInt(object.number, 10);
                    else if (typeof object.number === "number")
                        message.number = object.number;
                    else if (typeof object.number === "object")
                        message.number = new $util.LongBits(object.number.low >>> 0, object.number.high >>> 0).toNumber(true);
                if (object.module != null)
                    message.module = object.module >>> 0;
                if (object.sensor != null)
                    if ($util.Long)
                        (message.sensor = $util.Long.fromValue(object.sensor)).unsigned = true;
                    else if (typeof object.sensor === "string")
                        message.sensor = parseInt(object.sensor, 10);
                    else if (typeof object.sensor === "number")
                        message.sensor = object.sensor;
                    else if (typeof object.sensor === "object")
                        message.sensor = new $util.LongBits(object.sensor.low >>> 0, object.sensor.high >>> 0).toNumber(true);
                if (object.values) {
                    if (!Array.isArray(object.values))
                        throw TypeError(".fk_data.LoraRecord.values: array expected");
                    message.values = [];
                    for (var i = 0; i < object.values.length; ++i)
                        message.values[i] = Number(object.values[i]);
                }
                if (object.data != null)
                    if (typeof object.data === "string")
                        $util.base64.decode(object.data, message.data = $util.newBuffer($util.base64.length(object.data)), 0);
                    else if (object.data.length)
                        message.data = object.data;
                return message;
            };
    
            /**
             * Creates a plain object from a LoraRecord message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.LoraRecord
             * @static
             * @param {fk_data.LoraRecord} message LoraRecord
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            LoraRecord.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults)
                    object.values = [];
                if (options.defaults) {
                    object.deviceId = options.bytes === String ? "" : [];
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, false);
                        object.time = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.time = options.longs === String ? "0" : 0;
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.number = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.number = options.longs === String ? "0" : 0;
                    object.module = 0;
                    if ($util.Long) {
                        var long = new $util.Long(0, 0, true);
                        object.sensor = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                    } else
                        object.sensor = options.longs === String ? "0" : 0;
                    object.data = options.bytes === String ? "" : [];
                }
                if (message.deviceId != null && message.hasOwnProperty("deviceId"))
                    object.deviceId = options.bytes === String ? $util.base64.encode(message.deviceId, 0, message.deviceId.length) : options.bytes === Array ? Array.prototype.slice.call(message.deviceId) : message.deviceId;
                if (message.time != null && message.hasOwnProperty("time"))
                    if (typeof message.time === "number")
                        object.time = options.longs === String ? String(message.time) : message.time;
                    else
                        object.time = options.longs === String ? $util.Long.prototype.toString.call(message.time) : options.longs === Number ? new $util.LongBits(message.time.low >>> 0, message.time.high >>> 0).toNumber() : message.time;
                if (message.number != null && message.hasOwnProperty("number"))
                    if (typeof message.number === "number")
                        object.number = options.longs === String ? String(message.number) : message.number;
                    else
                        object.number = options.longs === String ? $util.Long.prototype.toString.call(message.number) : options.longs === Number ? new $util.LongBits(message.number.low >>> 0, message.number.high >>> 0).toNumber(true) : message.number;
                if (message.module != null && message.hasOwnProperty("module"))
                    object.module = message.module;
                if (message.sensor != null && message.hasOwnProperty("sensor"))
                    if (typeof message.sensor === "number")
                        object.sensor = options.longs === String ? String(message.sensor) : message.sensor;
                    else
                        object.sensor = options.longs === String ? $util.Long.prototype.toString.call(message.sensor) : options.longs === Number ? new $util.LongBits(message.sensor.low >>> 0, message.sensor.high >>> 0).toNumber(true) : message.sensor;
                if (message.values && message.values.length) {
                    object.values = [];
                    for (var j = 0; j < message.values.length; ++j)
                        object.values[j] = options.json && !isFinite(message.values[j]) ? String(message.values[j]) : message.values[j];
                }
                if (message.data != null && message.hasOwnProperty("data"))
                    object.data = options.bytes === String ? $util.base64.encode(message.data, 0, message.data.length) : options.bytes === Array ? Array.prototype.slice.call(message.data) : message.data;
                return object;
            };
    
            /**
             * Converts this LoraRecord to JSON.
             * @function toJSON
             * @memberof fk_data.LoraRecord
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            LoraRecord.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return LoraRecord;
        })();
    
        /**
         * CurveType enum.
         * @name fk_data.CurveType
         * @enum {string}
         * @property {number} CURVE_NONE=0 CURVE_NONE value
         * @property {number} CURVE_LINEAR=1 CURVE_LINEAR value
         * @property {number} CURVE_POWER=2 CURVE_POWER value
         * @property {number} CURVE_LOGARITHMIC=3 CURVE_LOGARITHMIC value
         * @property {number} CURVE_EXPONENTIAL=4 CURVE_EXPONENTIAL value
         */
        fk_data.CurveType = (function() {
            var valuesById = {}, values = Object.create(valuesById);
            values[valuesById[0] = "CURVE_NONE"] = 0;
            values[valuesById[1] = "CURVE_LINEAR"] = 1;
            values[valuesById[2] = "CURVE_POWER"] = 2;
            values[valuesById[3] = "CURVE_LOGARITHMIC"] = 3;
            values[valuesById[4] = "CURVE_EXPONENTIAL"] = 4;
            return values;
        })();
    
        fk_data.CalibrationPoint = (function() {
    
            /**
             * Properties of a CalibrationPoint.
             * @memberof fk_data
             * @interface ICalibrationPoint
             * @property {Array.<number>|null} [references] CalibrationPoint references
             * @property {Array.<number>|null} [uncalibrated] CalibrationPoint uncalibrated
             * @property {Array.<number>|null} [factory] CalibrationPoint factory
             * @property {Array.<Uint8Array>|null} [adc] CalibrationPoint adc
             */
    
            /**
             * Constructs a new CalibrationPoint.
             * @memberof fk_data
             * @classdesc Represents a CalibrationPoint.
             * @implements ICalibrationPoint
             * @constructor
             * @param {fk_data.ICalibrationPoint=} [properties] Properties to set
             */
            function CalibrationPoint(properties) {
                this.references = [];
                this.uncalibrated = [];
                this.factory = [];
                this.adc = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * CalibrationPoint references.
             * @member {Array.<number>} references
             * @memberof fk_data.CalibrationPoint
             * @instance
             */
            CalibrationPoint.prototype.references = $util.emptyArray;
    
            /**
             * CalibrationPoint uncalibrated.
             * @member {Array.<number>} uncalibrated
             * @memberof fk_data.CalibrationPoint
             * @instance
             */
            CalibrationPoint.prototype.uncalibrated = $util.emptyArray;
    
            /**
             * CalibrationPoint factory.
             * @member {Array.<number>} factory
             * @memberof fk_data.CalibrationPoint
             * @instance
             */
            CalibrationPoint.prototype.factory = $util.emptyArray;
    
            /**
             * CalibrationPoint adc.
             * @member {Array.<Uint8Array>} adc
             * @memberof fk_data.CalibrationPoint
             * @instance
             */
            CalibrationPoint.prototype.adc = $util.emptyArray;
    
            /**
             * Creates a new CalibrationPoint instance using the specified properties.
             * @function create
             * @memberof fk_data.CalibrationPoint
             * @static
             * @param {fk_data.ICalibrationPoint=} [properties] Properties to set
             * @returns {fk_data.CalibrationPoint} CalibrationPoint instance
             */
            CalibrationPoint.create = function create(properties) {
                return new CalibrationPoint(properties);
            };
    
            /**
             * Encodes the specified CalibrationPoint message. Does not implicitly {@link fk_data.CalibrationPoint.verify|verify} messages.
             * @function encode
             * @memberof fk_data.CalibrationPoint
             * @static
             * @param {fk_data.ICalibrationPoint} message CalibrationPoint message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            CalibrationPoint.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.references != null && message.references.length) {
                    writer.uint32(/* id 1, wireType 2 =*/10).fork();
                    for (var i = 0; i < message.references.length; ++i)
                        writer.float(message.references[i]);
                    writer.ldelim();
                }
                if (message.uncalibrated != null && message.uncalibrated.length) {
                    writer.uint32(/* id 2, wireType 2 =*/18).fork();
                    for (var i = 0; i < message.uncalibrated.length; ++i)
                        writer.float(message.uncalibrated[i]);
                    writer.ldelim();
                }
                if (message.factory != null && message.factory.length) {
                    writer.uint32(/* id 3, wireType 2 =*/26).fork();
                    for (var i = 0; i < message.factory.length; ++i)
                        writer.float(message.factory[i]);
                    writer.ldelim();
                }
                if (message.adc != null && message.adc.length)
                    for (var i = 0; i < message.adc.length; ++i)
                        writer.uint32(/* id 4, wireType 2 =*/34).bytes(message.adc[i]);
                return writer;
            };
    
            /**
             * Encodes the specified CalibrationPoint message, length delimited. Does not implicitly {@link fk_data.CalibrationPoint.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.CalibrationPoint
             * @static
             * @param {fk_data.ICalibrationPoint} message CalibrationPoint message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            CalibrationPoint.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a CalibrationPoint message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.CalibrationPoint
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.CalibrationPoint} CalibrationPoint
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            CalibrationPoint.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.CalibrationPoint();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        if (!(message.references && message.references.length))
                            message.references = [];
                        if ((tag & 7) === 2) {
                            var end2 = reader.uint32() + reader.pos;
                            while (reader.pos < end2)
                                message.references.push(reader.float());
                        } else
                            message.references.push(reader.float());
                        break;
                    case 2:
                        if (!(message.uncalibrated && message.uncalibrated.length))
                            message.uncalibrated = [];
                        if ((tag & 7) === 2) {
                            var end2 = reader.uint32() + reader.pos;
                            while (reader.pos < end2)
                                message.uncalibrated.push(reader.float());
                        } else
                            message.uncalibrated.push(reader.float());
                        break;
                    case 3:
                        if (!(message.factory && message.factory.length))
                            message.factory = [];
                        if ((tag & 7) === 2) {
                            var end2 = reader.uint32() + reader.pos;
                            while (reader.pos < end2)
                                message.factory.push(reader.float());
                        } else
                            message.factory.push(reader.float());
                        break;
                    case 4:
                        if (!(message.adc && message.adc.length))
                            message.adc = [];
                        message.adc.push(reader.bytes());
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a CalibrationPoint message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_data.CalibrationPoint
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.CalibrationPoint} CalibrationPoint
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            CalibrationPoint.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a CalibrationPoint message.
             * @function verify
             * @memberof fk_data.CalibrationPoint
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            CalibrationPoint.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.references != null && message.hasOwnProperty("references")) {
                    if (!Array.isArray(message.references))
                        return "references: array expected";
                    for (var i = 0; i < message.references.length; ++i)
                        if (typeof message.references[i] !== "number")
                            return "references: number[] expected";
                }
                if (message.uncalibrated != null && message.hasOwnProperty("uncalibrated")) {
                    if (!Array.isArray(message.uncalibrated))
                        return "uncalibrated: array expected";
                    for (var i = 0; i < message.uncalibrated.length; ++i)
                        if (typeof message.uncalibrated[i] !== "number")
                            return "uncalibrated: number[] expected";
                }
                if (message.factory != null && message.hasOwnProperty("factory")) {
                    if (!Array.isArray(message.factory))
                        return "factory: array expected";
                    for (var i = 0; i < message.factory.length; ++i)
                        if (typeof message.factory[i] !== "number")
                            return "factory: number[] expected";
                }
                if (message.adc != null && message.hasOwnProperty("adc")) {
                    if (!Array.isArray(message.adc))
                        return "adc: array expected";
                    for (var i = 0; i < message.adc.length; ++i)
                        if (!(message.adc[i] && typeof message.adc[i].length === "number" || $util.isString(message.adc[i])))
                            return "adc: buffer[] expected";
                }
                return null;
            };
    
            /**
             * Creates a CalibrationPoint message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.CalibrationPoint
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.CalibrationPoint} CalibrationPoint
             */
            CalibrationPoint.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.CalibrationPoint)
                    return object;
                var message = new $root.fk_data.CalibrationPoint();
                if (object.references) {
                    if (!Array.isArray(object.references))
                        throw TypeError(".fk_data.CalibrationPoint.references: array expected");
                    message.references = [];
                    for (var i = 0; i < object.references.length; ++i)
                        message.references[i] = Number(object.references[i]);
                }
                if (object.uncalibrated) {
                    if (!Array.isArray(object.uncalibrated))
                        throw TypeError(".fk_data.CalibrationPoint.uncalibrated: array expected");
                    message.uncalibrated = [];
                    for (var i = 0; i < object.uncalibrated.length; ++i)
                        message.uncalibrated[i] = Number(object.uncalibrated[i]);
                }
                if (object.factory) {
                    if (!Array.isArray(object.factory))
                        throw TypeError(".fk_data.CalibrationPoint.factory: array expected");
                    message.factory = [];
                    for (var i = 0; i < object.factory.length; ++i)
                        message.factory[i] = Number(object.factory[i]);
                }
                if (object.adc) {
                    if (!Array.isArray(object.adc))
                        throw TypeError(".fk_data.CalibrationPoint.adc: array expected");
                    message.adc = [];
                    for (var i = 0; i < object.adc.length; ++i)
                        if (typeof object.adc[i] === "string")
                            $util.base64.decode(object.adc[i], message.adc[i] = $util.newBuffer($util.base64.length(object.adc[i])), 0);
                        else if (object.adc[i].length)
                            message.adc[i] = object.adc[i];
                }
                return message;
            };
    
            /**
             * Creates a plain object from a CalibrationPoint message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.CalibrationPoint
             * @static
             * @param {fk_data.CalibrationPoint} message CalibrationPoint
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            CalibrationPoint.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults) {
                    object.references = [];
                    object.uncalibrated = [];
                    object.factory = [];
                    object.adc = [];
                }
                if (message.references && message.references.length) {
                    object.references = [];
                    for (var j = 0; j < message.references.length; ++j)
                        object.references[j] = options.json && !isFinite(message.references[j]) ? String(message.references[j]) : message.references[j];
                }
                if (message.uncalibrated && message.uncalibrated.length) {
                    object.uncalibrated = [];
                    for (var j = 0; j < message.uncalibrated.length; ++j)
                        object.uncalibrated[j] = options.json && !isFinite(message.uncalibrated[j]) ? String(message.uncalibrated[j]) : message.uncalibrated[j];
                }
                if (message.factory && message.factory.length) {
                    object.factory = [];
                    for (var j = 0; j < message.factory.length; ++j)
                        object.factory[j] = options.json && !isFinite(message.factory[j]) ? String(message.factory[j]) : message.factory[j];
                }
                if (message.adc && message.adc.length) {
                    object.adc = [];
                    for (var j = 0; j < message.adc.length; ++j)
                        object.adc[j] = options.bytes === String ? $util.base64.encode(message.adc[j], 0, message.adc[j].length) : options.bytes === Array ? Array.prototype.slice.call(message.adc[j]) : message.adc[j];
                }
                return object;
            };
    
            /**
             * Converts this CalibrationPoint to JSON.
             * @function toJSON
             * @memberof fk_data.CalibrationPoint
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            CalibrationPoint.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return CalibrationPoint;
        })();
    
        fk_data.CalibrationCoefficients = (function() {
    
            /**
             * Properties of a CalibrationCoefficients.
             * @memberof fk_data
             * @interface ICalibrationCoefficients
             * @property {Array.<number>|null} [values] CalibrationCoefficients values
             */
    
            /**
             * Constructs a new CalibrationCoefficients.
             * @memberof fk_data
             * @classdesc Represents a CalibrationCoefficients.
             * @implements ICalibrationCoefficients
             * @constructor
             * @param {fk_data.ICalibrationCoefficients=} [properties] Properties to set
             */
            function CalibrationCoefficients(properties) {
                this.values = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * CalibrationCoefficients values.
             * @member {Array.<number>} values
             * @memberof fk_data.CalibrationCoefficients
             * @instance
             */
            CalibrationCoefficients.prototype.values = $util.emptyArray;
    
            /**
             * Creates a new CalibrationCoefficients instance using the specified properties.
             * @function create
             * @memberof fk_data.CalibrationCoefficients
             * @static
             * @param {fk_data.ICalibrationCoefficients=} [properties] Properties to set
             * @returns {fk_data.CalibrationCoefficients} CalibrationCoefficients instance
             */
            CalibrationCoefficients.create = function create(properties) {
                return new CalibrationCoefficients(properties);
            };
    
            /**
             * Encodes the specified CalibrationCoefficients message. Does not implicitly {@link fk_data.CalibrationCoefficients.verify|verify} messages.
             * @function encode
             * @memberof fk_data.CalibrationCoefficients
             * @static
             * @param {fk_data.ICalibrationCoefficients} message CalibrationCoefficients message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            CalibrationCoefficients.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.values != null && message.values.length) {
                    writer.uint32(/* id 1, wireType 2 =*/10).fork();
                    for (var i = 0; i < message.values.length; ++i)
                        writer.float(message.values[i]);
                    writer.ldelim();
                }
                return writer;
            };
    
            /**
             * Encodes the specified CalibrationCoefficients message, length delimited. Does not implicitly {@link fk_data.CalibrationCoefficients.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.CalibrationCoefficients
             * @static
             * @param {fk_data.ICalibrationCoefficients} message CalibrationCoefficients message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            CalibrationCoefficients.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a CalibrationCoefficients message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.CalibrationCoefficients
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.CalibrationCoefficients} CalibrationCoefficients
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            CalibrationCoefficients.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.CalibrationCoefficients();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        if (!(message.values && message.values.length))
                            message.values = [];
                        if ((tag & 7) === 2) {
                            var end2 = reader.uint32() + reader.pos;
                            while (reader.pos < end2)
                                message.values.push(reader.float());
                        } else
                            message.values.push(reader.float());
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a CalibrationCoefficients message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_data.CalibrationCoefficients
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.CalibrationCoefficients} CalibrationCoefficients
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            CalibrationCoefficients.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a CalibrationCoefficients message.
             * @function verify
             * @memberof fk_data.CalibrationCoefficients
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            CalibrationCoefficients.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.values != null && message.hasOwnProperty("values")) {
                    if (!Array.isArray(message.values))
                        return "values: array expected";
                    for (var i = 0; i < message.values.length; ++i)
                        if (typeof message.values[i] !== "number")
                            return "values: number[] expected";
                }
                return null;
            };
    
            /**
             * Creates a CalibrationCoefficients message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.CalibrationCoefficients
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.CalibrationCoefficients} CalibrationCoefficients
             */
            CalibrationCoefficients.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.CalibrationCoefficients)
                    return object;
                var message = new $root.fk_data.CalibrationCoefficients();
                if (object.values) {
                    if (!Array.isArray(object.values))
                        throw TypeError(".fk_data.CalibrationCoefficients.values: array expected");
                    message.values = [];
                    for (var i = 0; i < object.values.length; ++i)
                        message.values[i] = Number(object.values[i]);
                }
                return message;
            };
    
            /**
             * Creates a plain object from a CalibrationCoefficients message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.CalibrationCoefficients
             * @static
             * @param {fk_data.CalibrationCoefficients} message CalibrationCoefficients
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            CalibrationCoefficients.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults)
                    object.values = [];
                if (message.values && message.values.length) {
                    object.values = [];
                    for (var j = 0; j < message.values.length; ++j)
                        object.values[j] = options.json && !isFinite(message.values[j]) ? String(message.values[j]) : message.values[j];
                }
                return object;
            };
    
            /**
             * Converts this CalibrationCoefficients to JSON.
             * @function toJSON
             * @memberof fk_data.CalibrationCoefficients
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            CalibrationCoefficients.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return CalibrationCoefficients;
        })();
    
        fk_data.Calibration = (function() {
    
            /**
             * Properties of a Calibration.
             * @memberof fk_data
             * @interface ICalibration
             * @property {fk_data.CurveType|null} [type] Calibration type
             * @property {number|null} [time] Calibration time
             * @property {number|null} [kind] Calibration kind
             * @property {Array.<fk_data.ICalibrationPoint>|null} [points] Calibration points
             * @property {fk_data.ICalibrationCoefficients|null} [coefficients] Calibration coefficients
             * @property {fk_data.IFirmware|null} [firmware] Calibration firmware
             */
    
            /**
             * Constructs a new Calibration.
             * @memberof fk_data
             * @classdesc Represents a Calibration.
             * @implements ICalibration
             * @constructor
             * @param {fk_data.ICalibration=} [properties] Properties to set
             */
            function Calibration(properties) {
                this.points = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * Calibration type.
             * @member {fk_data.CurveType} type
             * @memberof fk_data.Calibration
             * @instance
             */
            Calibration.prototype.type = 0;
    
            /**
             * Calibration time.
             * @member {number} time
             * @memberof fk_data.Calibration
             * @instance
             */
            Calibration.prototype.time = 0;
    
            /**
             * Calibration kind.
             * @member {number} kind
             * @memberof fk_data.Calibration
             * @instance
             */
            Calibration.prototype.kind = 0;
    
            /**
             * Calibration points.
             * @member {Array.<fk_data.ICalibrationPoint>} points
             * @memberof fk_data.Calibration
             * @instance
             */
            Calibration.prototype.points = $util.emptyArray;
    
            /**
             * Calibration coefficients.
             * @member {fk_data.ICalibrationCoefficients|null|undefined} coefficients
             * @memberof fk_data.Calibration
             * @instance
             */
            Calibration.prototype.coefficients = null;
    
            /**
             * Calibration firmware.
             * @member {fk_data.IFirmware|null|undefined} firmware
             * @memberof fk_data.Calibration
             * @instance
             */
            Calibration.prototype.firmware = null;
    
            /**
             * Creates a new Calibration instance using the specified properties.
             * @function create
             * @memberof fk_data.Calibration
             * @static
             * @param {fk_data.ICalibration=} [properties] Properties to set
             * @returns {fk_data.Calibration} Calibration instance
             */
            Calibration.create = function create(properties) {
                return new Calibration(properties);
            };
    
            /**
             * Encodes the specified Calibration message. Does not implicitly {@link fk_data.Calibration.verify|verify} messages.
             * @function encode
             * @memberof fk_data.Calibration
             * @static
             * @param {fk_data.ICalibration} message Calibration message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Calibration.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.type != null && message.hasOwnProperty("type"))
                    writer.uint32(/* id 1, wireType 0 =*/8).int32(message.type);
                if (message.time != null && message.hasOwnProperty("time"))
                    writer.uint32(/* id 2, wireType 0 =*/16).uint32(message.time);
                if (message.points != null && message.points.length)
                    for (var i = 0; i < message.points.length; ++i)
                        $root.fk_data.CalibrationPoint.encode(message.points[i], writer.uint32(/* id 3, wireType 2 =*/26).fork()).ldelim();
                if (message.coefficients != null && message.hasOwnProperty("coefficients"))
                    $root.fk_data.CalibrationCoefficients.encode(message.coefficients, writer.uint32(/* id 4, wireType 2 =*/34).fork()).ldelim();
                if (message.firmware != null && message.hasOwnProperty("firmware"))
                    $root.fk_data.Firmware.encode(message.firmware, writer.uint32(/* id 5, wireType 2 =*/42).fork()).ldelim();
                if (message.kind != null && message.hasOwnProperty("kind"))
                    writer.uint32(/* id 6, wireType 0 =*/48).uint32(message.kind);
                return writer;
            };
    
            /**
             * Encodes the specified Calibration message, length delimited. Does not implicitly {@link fk_data.Calibration.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.Calibration
             * @static
             * @param {fk_data.ICalibration} message Calibration message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            Calibration.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a Calibration message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.Calibration
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.Calibration} Calibration
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Calibration.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.Calibration();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.type = reader.int32();
                        break;
                    case 2:
                        message.time = reader.uint32();
                        break;
                    case 6:
                        message.kind = reader.uint32();
                        break;
                    case 3:
                        if (!(message.points && message.points.length))
                            message.points = [];
                        message.points.push($root.fk_data.CalibrationPoint.decode(reader, reader.uint32()));
                        break;
                    case 4:
                        message.coefficients = $root.fk_data.CalibrationCoefficients.decode(reader, reader.uint32());
                        break;
                    case 5:
                        message.firmware = $root.fk_data.Firmware.decode(reader, reader.uint32());
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a Calibration message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_data.Calibration
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.Calibration} Calibration
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            Calibration.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a Calibration message.
             * @function verify
             * @memberof fk_data.Calibration
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            Calibration.verify = function verify(message) {
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
                if (message.time != null && message.hasOwnProperty("time"))
                    if (!$util.isInteger(message.time))
                        return "time: integer expected";
                if (message.kind != null && message.hasOwnProperty("kind"))
                    if (!$util.isInteger(message.kind))
                        return "kind: integer expected";
                if (message.points != null && message.hasOwnProperty("points")) {
                    if (!Array.isArray(message.points))
                        return "points: array expected";
                    for (var i = 0; i < message.points.length; ++i) {
                        var error = $root.fk_data.CalibrationPoint.verify(message.points[i]);
                        if (error)
                            return "points." + error;
                    }
                }
                if (message.coefficients != null && message.hasOwnProperty("coefficients")) {
                    var error = $root.fk_data.CalibrationCoefficients.verify(message.coefficients);
                    if (error)
                        return "coefficients." + error;
                }
                if (message.firmware != null && message.hasOwnProperty("firmware")) {
                    var error = $root.fk_data.Firmware.verify(message.firmware);
                    if (error)
                        return "firmware." + error;
                }
                return null;
            };
    
            /**
             * Creates a Calibration message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.Calibration
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.Calibration} Calibration
             */
            Calibration.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.Calibration)
                    return object;
                var message = new $root.fk_data.Calibration();
                switch (object.type) {
                case "CURVE_NONE":
                case 0:
                    message.type = 0;
                    break;
                case "CURVE_LINEAR":
                case 1:
                    message.type = 1;
                    break;
                case "CURVE_POWER":
                case 2:
                    message.type = 2;
                    break;
                case "CURVE_LOGARITHMIC":
                case 3:
                    message.type = 3;
                    break;
                case "CURVE_EXPONENTIAL":
                case 4:
                    message.type = 4;
                    break;
                }
                if (object.time != null)
                    message.time = object.time >>> 0;
                if (object.kind != null)
                    message.kind = object.kind >>> 0;
                if (object.points) {
                    if (!Array.isArray(object.points))
                        throw TypeError(".fk_data.Calibration.points: array expected");
                    message.points = [];
                    for (var i = 0; i < object.points.length; ++i) {
                        if (typeof object.points[i] !== "object")
                            throw TypeError(".fk_data.Calibration.points: object expected");
                        message.points[i] = $root.fk_data.CalibrationPoint.fromObject(object.points[i]);
                    }
                }
                if (object.coefficients != null) {
                    if (typeof object.coefficients !== "object")
                        throw TypeError(".fk_data.Calibration.coefficients: object expected");
                    message.coefficients = $root.fk_data.CalibrationCoefficients.fromObject(object.coefficients);
                }
                if (object.firmware != null) {
                    if (typeof object.firmware !== "object")
                        throw TypeError(".fk_data.Calibration.firmware: object expected");
                    message.firmware = $root.fk_data.Firmware.fromObject(object.firmware);
                }
                return message;
            };
    
            /**
             * Creates a plain object from a Calibration message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.Calibration
             * @static
             * @param {fk_data.Calibration} message Calibration
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            Calibration.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults)
                    object.points = [];
                if (options.defaults) {
                    object.type = options.enums === String ? "CURVE_NONE" : 0;
                    object.time = 0;
                    object.coefficients = null;
                    object.firmware = null;
                    object.kind = 0;
                }
                if (message.type != null && message.hasOwnProperty("type"))
                    object.type = options.enums === String ? $root.fk_data.CurveType[message.type] : message.type;
                if (message.time != null && message.hasOwnProperty("time"))
                    object.time = message.time;
                if (message.points && message.points.length) {
                    object.points = [];
                    for (var j = 0; j < message.points.length; ++j)
                        object.points[j] = $root.fk_data.CalibrationPoint.toObject(message.points[j], options);
                }
                if (message.coefficients != null && message.hasOwnProperty("coefficients"))
                    object.coefficients = $root.fk_data.CalibrationCoefficients.toObject(message.coefficients, options);
                if (message.firmware != null && message.hasOwnProperty("firmware"))
                    object.firmware = $root.fk_data.Firmware.toObject(message.firmware, options);
                if (message.kind != null && message.hasOwnProperty("kind"))
                    object.kind = message.kind;
                return object;
            };
    
            /**
             * Converts this Calibration to JSON.
             * @function toJSON
             * @memberof fk_data.Calibration
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            Calibration.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return Calibration;
        })();
    
        fk_data.ModuleConfiguration = (function() {
    
            /**
             * Properties of a ModuleConfiguration.
             * @memberof fk_data
             * @interface IModuleConfiguration
             * @property {fk_data.ICalibration|null} [calibration] ModuleConfiguration calibration
             * @property {Array.<fk_data.ICalibration>|null} [calibrations] ModuleConfiguration calibrations
             */
    
            /**
             * Constructs a new ModuleConfiguration.
             * @memberof fk_data
             * @classdesc Represents a ModuleConfiguration.
             * @implements IModuleConfiguration
             * @constructor
             * @param {fk_data.IModuleConfiguration=} [properties] Properties to set
             */
            function ModuleConfiguration(properties) {
                this.calibrations = [];
                if (properties)
                    for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                        if (properties[keys[i]] != null)
                            this[keys[i]] = properties[keys[i]];
            }
    
            /**
             * ModuleConfiguration calibration.
             * @member {fk_data.ICalibration|null|undefined} calibration
             * @memberof fk_data.ModuleConfiguration
             * @instance
             */
            ModuleConfiguration.prototype.calibration = null;
    
            /**
             * ModuleConfiguration calibrations.
             * @member {Array.<fk_data.ICalibration>} calibrations
             * @memberof fk_data.ModuleConfiguration
             * @instance
             */
            ModuleConfiguration.prototype.calibrations = $util.emptyArray;
    
            /**
             * Creates a new ModuleConfiguration instance using the specified properties.
             * @function create
             * @memberof fk_data.ModuleConfiguration
             * @static
             * @param {fk_data.IModuleConfiguration=} [properties] Properties to set
             * @returns {fk_data.ModuleConfiguration} ModuleConfiguration instance
             */
            ModuleConfiguration.create = function create(properties) {
                return new ModuleConfiguration(properties);
            };
    
            /**
             * Encodes the specified ModuleConfiguration message. Does not implicitly {@link fk_data.ModuleConfiguration.verify|verify} messages.
             * @function encode
             * @memberof fk_data.ModuleConfiguration
             * @static
             * @param {fk_data.IModuleConfiguration} message ModuleConfiguration message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            ModuleConfiguration.encode = function encode(message, writer) {
                if (!writer)
                    writer = $Writer.create();
                if (message.calibration != null && message.hasOwnProperty("calibration"))
                    $root.fk_data.Calibration.encode(message.calibration, writer.uint32(/* id 1, wireType 2 =*/10).fork()).ldelim();
                if (message.calibrations != null && message.calibrations.length)
                    for (var i = 0; i < message.calibrations.length; ++i)
                        $root.fk_data.Calibration.encode(message.calibrations[i], writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
                return writer;
            };
    
            /**
             * Encodes the specified ModuleConfiguration message, length delimited. Does not implicitly {@link fk_data.ModuleConfiguration.verify|verify} messages.
             * @function encodeDelimited
             * @memberof fk_data.ModuleConfiguration
             * @static
             * @param {fk_data.IModuleConfiguration} message ModuleConfiguration message or plain object to encode
             * @param {$protobuf.Writer} [writer] Writer to encode to
             * @returns {$protobuf.Writer} Writer
             */
            ModuleConfiguration.encodeDelimited = function encodeDelimited(message, writer) {
                return this.encode(message, writer).ldelim();
            };
    
            /**
             * Decodes a ModuleConfiguration message from the specified reader or buffer.
             * @function decode
             * @memberof fk_data.ModuleConfiguration
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @param {number} [length] Message length if known beforehand
             * @returns {fk_data.ModuleConfiguration} ModuleConfiguration
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            ModuleConfiguration.decode = function decode(reader, length) {
                if (!(reader instanceof $Reader))
                    reader = $Reader.create(reader);
                var end = length === undefined ? reader.len : reader.pos + length, message = new $root.fk_data.ModuleConfiguration();
                while (reader.pos < end) {
                    var tag = reader.uint32();
                    switch (tag >>> 3) {
                    case 1:
                        message.calibration = $root.fk_data.Calibration.decode(reader, reader.uint32());
                        break;
                    case 2:
                        if (!(message.calibrations && message.calibrations.length))
                            message.calibrations = [];
                        message.calibrations.push($root.fk_data.Calibration.decode(reader, reader.uint32()));
                        break;
                    default:
                        reader.skipType(tag & 7);
                        break;
                    }
                }
                return message;
            };
    
            /**
             * Decodes a ModuleConfiguration message from the specified reader or buffer, length delimited.
             * @function decodeDelimited
             * @memberof fk_data.ModuleConfiguration
             * @static
             * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
             * @returns {fk_data.ModuleConfiguration} ModuleConfiguration
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            ModuleConfiguration.decodeDelimited = function decodeDelimited(reader) {
                if (!(reader instanceof $Reader))
                    reader = new $Reader(reader);
                return this.decode(reader, reader.uint32());
            };
    
            /**
             * Verifies a ModuleConfiguration message.
             * @function verify
             * @memberof fk_data.ModuleConfiguration
             * @static
             * @param {Object.<string,*>} message Plain object to verify
             * @returns {string|null} `null` if valid, otherwise the reason why it is not
             */
            ModuleConfiguration.verify = function verify(message) {
                if (typeof message !== "object" || message === null)
                    return "object expected";
                if (message.calibration != null && message.hasOwnProperty("calibration")) {
                    var error = $root.fk_data.Calibration.verify(message.calibration);
                    if (error)
                        return "calibration." + error;
                }
                if (message.calibrations != null && message.hasOwnProperty("calibrations")) {
                    if (!Array.isArray(message.calibrations))
                        return "calibrations: array expected";
                    for (var i = 0; i < message.calibrations.length; ++i) {
                        var error = $root.fk_data.Calibration.verify(message.calibrations[i]);
                        if (error)
                            return "calibrations." + error;
                    }
                }
                return null;
            };
    
            /**
             * Creates a ModuleConfiguration message from a plain object. Also converts values to their respective internal types.
             * @function fromObject
             * @memberof fk_data.ModuleConfiguration
             * @static
             * @param {Object.<string,*>} object Plain object
             * @returns {fk_data.ModuleConfiguration} ModuleConfiguration
             */
            ModuleConfiguration.fromObject = function fromObject(object) {
                if (object instanceof $root.fk_data.ModuleConfiguration)
                    return object;
                var message = new $root.fk_data.ModuleConfiguration();
                if (object.calibration != null) {
                    if (typeof object.calibration !== "object")
                        throw TypeError(".fk_data.ModuleConfiguration.calibration: object expected");
                    message.calibration = $root.fk_data.Calibration.fromObject(object.calibration);
                }
                if (object.calibrations) {
                    if (!Array.isArray(object.calibrations))
                        throw TypeError(".fk_data.ModuleConfiguration.calibrations: array expected");
                    message.calibrations = [];
                    for (var i = 0; i < object.calibrations.length; ++i) {
                        if (typeof object.calibrations[i] !== "object")
                            throw TypeError(".fk_data.ModuleConfiguration.calibrations: object expected");
                        message.calibrations[i] = $root.fk_data.Calibration.fromObject(object.calibrations[i]);
                    }
                }
                return message;
            };
    
            /**
             * Creates a plain object from a ModuleConfiguration message. Also converts values to other types if specified.
             * @function toObject
             * @memberof fk_data.ModuleConfiguration
             * @static
             * @param {fk_data.ModuleConfiguration} message ModuleConfiguration
             * @param {$protobuf.IConversionOptions} [options] Conversion options
             * @returns {Object.<string,*>} Plain object
             */
            ModuleConfiguration.toObject = function toObject(message, options) {
                if (!options)
                    options = {};
                var object = {};
                if (options.arrays || options.defaults)
                    object.calibrations = [];
                if (options.defaults)
                    object.calibration = null;
                if (message.calibration != null && message.hasOwnProperty("calibration"))
                    object.calibration = $root.fk_data.Calibration.toObject(message.calibration, options);
                if (message.calibrations && message.calibrations.length) {
                    object.calibrations = [];
                    for (var j = 0; j < message.calibrations.length; ++j)
                        object.calibrations[j] = $root.fk_data.Calibration.toObject(message.calibrations[j], options);
                }
                return object;
            };
    
            /**
             * Converts this ModuleConfiguration to JSON.
             * @function toJSON
             * @memberof fk_data.ModuleConfiguration
             * @instance
             * @returns {Object.<string,*>} JSON object
             */
            ModuleConfiguration.prototype.toJSON = function toJSON() {
                return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
            };
    
            return ModuleConfiguration;
        })();
    
        return fk_data;
    })();

    return $root;
});
