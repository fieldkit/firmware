import * as $protobuf from "protobufjs";

/** Namespace fk_data. */
export namespace fk_data {

    /** Properties of a DeviceLocation. */
    interface IDeviceLocation {

        /** DeviceLocation enabled */
        enabled?: (number|null);

        /** DeviceLocation fix */
        fix?: (number|null);

        /** DeviceLocation time */
        time?: (number|Long|null);

        /** DeviceLocation longitude */
        longitude?: (number|null);

        /** DeviceLocation latitude */
        latitude?: (number|null);

        /** DeviceLocation altitude */
        altitude?: (number|null);

        /** DeviceLocation coordinates */
        coordinates?: (number[]|null);

        /** DeviceLocation satellites */
        satellites?: (number|null);

        /** DeviceLocation hdop */
        hdop?: (number|null);
    }

    /** Represents a DeviceLocation. */
    class DeviceLocation implements IDeviceLocation {

        /**
         * Constructs a new DeviceLocation.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.IDeviceLocation);

        /** DeviceLocation enabled. */
        public enabled: number;

        /** DeviceLocation fix. */
        public fix: number;

        /** DeviceLocation time. */
        public time: (number|Long);

        /** DeviceLocation longitude. */
        public longitude: number;

        /** DeviceLocation latitude. */
        public latitude: number;

        /** DeviceLocation altitude. */
        public altitude: number;

        /** DeviceLocation coordinates. */
        public coordinates: number[];

        /** DeviceLocation satellites. */
        public satellites: number;

        /** DeviceLocation hdop. */
        public hdop: number;

        /**
         * Creates a new DeviceLocation instance using the specified properties.
         * @param [properties] Properties to set
         * @returns DeviceLocation instance
         */
        public static create(properties?: fk_data.IDeviceLocation): fk_data.DeviceLocation;

        /**
         * Encodes the specified DeviceLocation message. Does not implicitly {@link fk_data.DeviceLocation.verify|verify} messages.
         * @param message DeviceLocation message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.IDeviceLocation, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified DeviceLocation message, length delimited. Does not implicitly {@link fk_data.DeviceLocation.verify|verify} messages.
         * @param message DeviceLocation message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.IDeviceLocation, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a DeviceLocation message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns DeviceLocation
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.DeviceLocation;

        /**
         * Decodes a DeviceLocation message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns DeviceLocation
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.DeviceLocation;

        /**
         * Verifies a DeviceLocation message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a DeviceLocation message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns DeviceLocation
         */
        public static fromObject(object: { [k: string]: any }): fk_data.DeviceLocation;

        /**
         * Creates a plain object from a DeviceLocation message. Also converts values to other types if specified.
         * @param message DeviceLocation
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.DeviceLocation, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this DeviceLocation to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a SensorReading. */
    interface ISensorReading {

        /** SensorReading reading */
        reading?: (number|Long|null);

        /** SensorReading time */
        time?: (number|Long|null);

        /** SensorReading sensor */
        sensor?: (number|null);

        /** SensorReading value */
        value?: (number|null);
    }

    /** Represents a SensorReading. */
    class SensorReading implements ISensorReading {

        /**
         * Constructs a new SensorReading.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.ISensorReading);

        /** SensorReading reading. */
        public reading: (number|Long);

        /** SensorReading time. */
        public time: (number|Long);

        /** SensorReading sensor. */
        public sensor: number;

        /** SensorReading value. */
        public value: number;

        /**
         * Creates a new SensorReading instance using the specified properties.
         * @param [properties] Properties to set
         * @returns SensorReading instance
         */
        public static create(properties?: fk_data.ISensorReading): fk_data.SensorReading;

        /**
         * Encodes the specified SensorReading message. Does not implicitly {@link fk_data.SensorReading.verify|verify} messages.
         * @param message SensorReading message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.ISensorReading, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified SensorReading message, length delimited. Does not implicitly {@link fk_data.SensorReading.verify|verify} messages.
         * @param message SensorReading message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.ISensorReading, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a SensorReading message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns SensorReading
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.SensorReading;

        /**
         * Decodes a SensorReading message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns SensorReading
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.SensorReading;

        /**
         * Verifies a SensorReading message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a SensorReading message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns SensorReading
         */
        public static fromObject(object: { [k: string]: any }): fk_data.SensorReading;

        /**
         * Creates a plain object from a SensorReading message. Also converts values to other types if specified.
         * @param message SensorReading
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.SensorReading, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this SensorReading to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a LoggedReading. */
    interface ILoggedReading {

        /** LoggedReading version */
        version?: (number|null);

        /** LoggedReading location */
        location?: (fk_data.IDeviceLocation|null);

        /** LoggedReading reading */
        reading?: (fk_data.ISensorReading|null);
    }

    /** Represents a LoggedReading. */
    class LoggedReading implements ILoggedReading {

        /**
         * Constructs a new LoggedReading.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.ILoggedReading);

        /** LoggedReading version. */
        public version: number;

        /** LoggedReading location. */
        public location?: (fk_data.IDeviceLocation|null);

        /** LoggedReading reading. */
        public reading?: (fk_data.ISensorReading|null);

        /**
         * Creates a new LoggedReading instance using the specified properties.
         * @param [properties] Properties to set
         * @returns LoggedReading instance
         */
        public static create(properties?: fk_data.ILoggedReading): fk_data.LoggedReading;

        /**
         * Encodes the specified LoggedReading message. Does not implicitly {@link fk_data.LoggedReading.verify|verify} messages.
         * @param message LoggedReading message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.ILoggedReading, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified LoggedReading message, length delimited. Does not implicitly {@link fk_data.LoggedReading.verify|verify} messages.
         * @param message LoggedReading message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.ILoggedReading, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a LoggedReading message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns LoggedReading
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.LoggedReading;

        /**
         * Decodes a LoggedReading message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns LoggedReading
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.LoggedReading;

        /**
         * Verifies a LoggedReading message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a LoggedReading message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns LoggedReading
         */
        public static fromObject(object: { [k: string]: any }): fk_data.LoggedReading;

        /**
         * Creates a plain object from a LoggedReading message. Also converts values to other types if specified.
         * @param message LoggedReading
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.LoggedReading, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this LoggedReading to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a SensorAndValue. */
    interface ISensorAndValue {

        /** SensorAndValue sensor */
        sensor?: (number|null);

        /** SensorAndValue value */
        value?: (number|null);

        /** SensorAndValue uncalibrated */
        uncalibrated?: (number|null);
    }

    /** Represents a SensorAndValue. */
    class SensorAndValue implements ISensorAndValue {

        /**
         * Constructs a new SensorAndValue.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.ISensorAndValue);

        /** SensorAndValue sensor. */
        public sensor: number;

        /** SensorAndValue value. */
        public value: number;

        /** SensorAndValue uncalibrated. */
        public uncalibrated: number;

        /**
         * Creates a new SensorAndValue instance using the specified properties.
         * @param [properties] Properties to set
         * @returns SensorAndValue instance
         */
        public static create(properties?: fk_data.ISensorAndValue): fk_data.SensorAndValue;

        /**
         * Encodes the specified SensorAndValue message. Does not implicitly {@link fk_data.SensorAndValue.verify|verify} messages.
         * @param message SensorAndValue message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.ISensorAndValue, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified SensorAndValue message, length delimited. Does not implicitly {@link fk_data.SensorAndValue.verify|verify} messages.
         * @param message SensorAndValue message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.ISensorAndValue, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a SensorAndValue message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns SensorAndValue
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.SensorAndValue;

        /**
         * Decodes a SensorAndValue message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns SensorAndValue
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.SensorAndValue;

        /**
         * Verifies a SensorAndValue message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a SensorAndValue message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns SensorAndValue
         */
        public static fromObject(object: { [k: string]: any }): fk_data.SensorAndValue;

        /**
         * Creates a plain object from a SensorAndValue message. Also converts values to other types if specified.
         * @param message SensorAndValue
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.SensorAndValue, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this SensorAndValue to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a ModuleHeader. */
    interface IModuleHeader {

        /** ModuleHeader manufacturer */
        manufacturer?: (number|null);

        /** ModuleHeader kind */
        kind?: (number|null);

        /** ModuleHeader version */
        version?: (number|null);
    }

    /** Represents a ModuleHeader. */
    class ModuleHeader implements IModuleHeader {

        /**
         * Constructs a new ModuleHeader.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.IModuleHeader);

        /** ModuleHeader manufacturer. */
        public manufacturer: number;

        /** ModuleHeader kind. */
        public kind: number;

        /** ModuleHeader version. */
        public version: number;

        /**
         * Creates a new ModuleHeader instance using the specified properties.
         * @param [properties] Properties to set
         * @returns ModuleHeader instance
         */
        public static create(properties?: fk_data.IModuleHeader): fk_data.ModuleHeader;

        /**
         * Encodes the specified ModuleHeader message. Does not implicitly {@link fk_data.ModuleHeader.verify|verify} messages.
         * @param message ModuleHeader message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.IModuleHeader, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified ModuleHeader message, length delimited. Does not implicitly {@link fk_data.ModuleHeader.verify|verify} messages.
         * @param message ModuleHeader message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.IModuleHeader, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a ModuleHeader message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns ModuleHeader
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.ModuleHeader;

        /**
         * Decodes a ModuleHeader message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns ModuleHeader
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.ModuleHeader;

        /**
         * Verifies a ModuleHeader message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a ModuleHeader message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns ModuleHeader
         */
        public static fromObject(object: { [k: string]: any }): fk_data.ModuleHeader;

        /**
         * Creates a plain object from a ModuleHeader message. Also converts values to other types if specified.
         * @param message ModuleHeader
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.ModuleHeader, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this ModuleHeader to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a ModuleInfo. */
    interface IModuleInfo {

        /** ModuleInfo position */
        position?: (number|null);

        /** ModuleInfo address */
        address?: (number|null);

        /** ModuleInfo name */
        name?: (string|null);

        /** ModuleInfo header */
        header?: (fk_data.IModuleHeader|null);

        /** ModuleInfo firmware */
        firmware?: (fk_data.IFirmware|null);

        /** ModuleInfo sensors */
        sensors?: (fk_data.ISensorInfo[]|null);

        /** ModuleInfo id */
        id?: (Uint8Array|null);

        /** ModuleInfo flags */
        flags?: (number|null);

        /** ModuleInfo configuration */
        configuration?: (Uint8Array|null);
    }

    /** Represents a ModuleInfo. */
    class ModuleInfo implements IModuleInfo {

        /**
         * Constructs a new ModuleInfo.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.IModuleInfo);

        /** ModuleInfo position. */
        public position: number;

        /** ModuleInfo address. */
        public address: number;

        /** ModuleInfo name. */
        public name: string;

        /** ModuleInfo header. */
        public header?: (fk_data.IModuleHeader|null);

        /** ModuleInfo firmware. */
        public firmware?: (fk_data.IFirmware|null);

        /** ModuleInfo sensors. */
        public sensors: fk_data.ISensorInfo[];

        /** ModuleInfo id. */
        public id: Uint8Array;

        /** ModuleInfo flags. */
        public flags: number;

        /** ModuleInfo configuration. */
        public configuration: Uint8Array;

        /**
         * Creates a new ModuleInfo instance using the specified properties.
         * @param [properties] Properties to set
         * @returns ModuleInfo instance
         */
        public static create(properties?: fk_data.IModuleInfo): fk_data.ModuleInfo;

        /**
         * Encodes the specified ModuleInfo message. Does not implicitly {@link fk_data.ModuleInfo.verify|verify} messages.
         * @param message ModuleInfo message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.IModuleInfo, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified ModuleInfo message, length delimited. Does not implicitly {@link fk_data.ModuleInfo.verify|verify} messages.
         * @param message ModuleInfo message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.IModuleInfo, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a ModuleInfo message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns ModuleInfo
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.ModuleInfo;

        /**
         * Decodes a ModuleInfo message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns ModuleInfo
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.ModuleInfo;

        /**
         * Verifies a ModuleInfo message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a ModuleInfo message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns ModuleInfo
         */
        public static fromObject(object: { [k: string]: any }): fk_data.ModuleInfo;

        /**
         * Creates a plain object from a ModuleInfo message. Also converts values to other types if specified.
         * @param message ModuleInfo
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.ModuleInfo, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this ModuleInfo to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a SensorInfo. */
    interface ISensorInfo {

        /** SensorInfo number */
        number?: (number|null);

        /** SensorInfo name */
        name?: (string|null);

        /** SensorInfo unitOfMeasure */
        unitOfMeasure?: (string|null);

        /** SensorInfo flags */
        flags?: (number|null);
    }

    /** Represents a SensorInfo. */
    class SensorInfo implements ISensorInfo {

        /**
         * Constructs a new SensorInfo.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.ISensorInfo);

        /** SensorInfo number. */
        public number: number;

        /** SensorInfo name. */
        public name: string;

        /** SensorInfo unitOfMeasure. */
        public unitOfMeasure: string;

        /** SensorInfo flags. */
        public flags: number;

        /**
         * Creates a new SensorInfo instance using the specified properties.
         * @param [properties] Properties to set
         * @returns SensorInfo instance
         */
        public static create(properties?: fk_data.ISensorInfo): fk_data.SensorInfo;

        /**
         * Encodes the specified SensorInfo message. Does not implicitly {@link fk_data.SensorInfo.verify|verify} messages.
         * @param message SensorInfo message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.ISensorInfo, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified SensorInfo message, length delimited. Does not implicitly {@link fk_data.SensorInfo.verify|verify} messages.
         * @param message SensorInfo message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.ISensorInfo, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a SensorInfo message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns SensorInfo
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.SensorInfo;

        /**
         * Decodes a SensorInfo message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns SensorInfo
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.SensorInfo;

        /**
         * Verifies a SensorInfo message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a SensorInfo message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns SensorInfo
         */
        public static fromObject(object: { [k: string]: any }): fk_data.SensorInfo;

        /**
         * Creates a plain object from a SensorInfo message. Also converts values to other types if specified.
         * @param message SensorInfo
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.SensorInfo, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this SensorInfo to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a Firmware. */
    interface IFirmware {

        /** Firmware version */
        version?: (string|null);

        /** Firmware build */
        build?: (string|null);

        /** Firmware number */
        number?: (string|null);

        /** Firmware timestamp */
        timestamp?: (number|Long|null);

        /** Firmware hash */
        hash?: (string|null);
    }

    /** Represents a Firmware. */
    class Firmware implements IFirmware {

        /**
         * Constructs a new Firmware.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.IFirmware);

        /** Firmware version. */
        public version: string;

        /** Firmware build. */
        public build: string;

        /** Firmware number. */
        public number: string;

        /** Firmware timestamp. */
        public timestamp: (number|Long);

        /** Firmware hash. */
        public hash: string;

        /**
         * Creates a new Firmware instance using the specified properties.
         * @param [properties] Properties to set
         * @returns Firmware instance
         */
        public static create(properties?: fk_data.IFirmware): fk_data.Firmware;

        /**
         * Encodes the specified Firmware message. Does not implicitly {@link fk_data.Firmware.verify|verify} messages.
         * @param message Firmware message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.IFirmware, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Firmware message, length delimited. Does not implicitly {@link fk_data.Firmware.verify|verify} messages.
         * @param message Firmware message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.IFirmware, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a Firmware message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Firmware
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.Firmware;

        /**
         * Decodes a Firmware message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Firmware
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.Firmware;

        /**
         * Verifies a Firmware message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a Firmware message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns Firmware
         */
        public static fromObject(object: { [k: string]: any }): fk_data.Firmware;

        /**
         * Creates a plain object from a Firmware message. Also converts values to other types if specified.
         * @param message Firmware
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.Firmware, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Firmware to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a Metadata. */
    interface IMetadata {

        /** Metadata deviceId */
        deviceId?: (Uint8Array|null);

        /** Metadata time */
        time?: (number|Long|null);

        /** Metadata git */
        git?: (string|null);

        /** Metadata build */
        build?: (string|null);

        /** Metadata resetCause */
        resetCause?: (number|null);

        /** Metadata sensors */
        sensors?: (fk_data.ISensorInfo[]|null);

        /** Metadata modules */
        modules?: (fk_data.IModuleInfo[]|null);

        /** Metadata firmware */
        firmware?: (fk_data.IFirmware|null);

        /** Metadata generation */
        generation?: (Uint8Array|null);

        /** Metadata record */
        record?: (number|Long|null);
    }

    /** Represents a Metadata. */
    class Metadata implements IMetadata {

        /**
         * Constructs a new Metadata.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.IMetadata);

        /** Metadata deviceId. */
        public deviceId: Uint8Array;

        /** Metadata time. */
        public time: (number|Long);

        /** Metadata git. */
        public git: string;

        /** Metadata build. */
        public build: string;

        /** Metadata resetCause. */
        public resetCause: number;

        /** Metadata sensors. */
        public sensors: fk_data.ISensorInfo[];

        /** Metadata modules. */
        public modules: fk_data.IModuleInfo[];

        /** Metadata firmware. */
        public firmware?: (fk_data.IFirmware|null);

        /** Metadata generation. */
        public generation: Uint8Array;

        /** Metadata record. */
        public record: (number|Long);

        /**
         * Creates a new Metadata instance using the specified properties.
         * @param [properties] Properties to set
         * @returns Metadata instance
         */
        public static create(properties?: fk_data.IMetadata): fk_data.Metadata;

        /**
         * Encodes the specified Metadata message. Does not implicitly {@link fk_data.Metadata.verify|verify} messages.
         * @param message Metadata message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.IMetadata, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Metadata message, length delimited. Does not implicitly {@link fk_data.Metadata.verify|verify} messages.
         * @param message Metadata message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.IMetadata, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a Metadata message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Metadata
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.Metadata;

        /**
         * Decodes a Metadata message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Metadata
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.Metadata;

        /**
         * Verifies a Metadata message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a Metadata message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns Metadata
         */
        public static fromObject(object: { [k: string]: any }): fk_data.Metadata;

        /**
         * Creates a plain object from a Metadata message. Also converts values to other types if specified.
         * @param message Metadata
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.Metadata, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Metadata to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a Status. */
    interface IStatus {

        /** Status time */
        time?: (number|Long|null);

        /** Status uptime */
        uptime?: (number|null);

        /** Status battery */
        battery?: (number|null);

        /** Status memory */
        memory?: (number|null);

        /** Status busy */
        busy?: (number|Long|null);
    }

    /** Represents a Status. */
    class Status implements IStatus {

        /**
         * Constructs a new Status.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.IStatus);

        /** Status time. */
        public time: (number|Long);

        /** Status uptime. */
        public uptime: number;

        /** Status battery. */
        public battery: number;

        /** Status memory. */
        public memory: number;

        /** Status busy. */
        public busy: (number|Long);

        /**
         * Creates a new Status instance using the specified properties.
         * @param [properties] Properties to set
         * @returns Status instance
         */
        public static create(properties?: fk_data.IStatus): fk_data.Status;

        /**
         * Encodes the specified Status message. Does not implicitly {@link fk_data.Status.verify|verify} messages.
         * @param message Status message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.IStatus, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Status message, length delimited. Does not implicitly {@link fk_data.Status.verify|verify} messages.
         * @param message Status message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.IStatus, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a Status message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Status
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.Status;

        /**
         * Decodes a Status message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Status
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.Status;

        /**
         * Verifies a Status message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a Status message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns Status
         */
        public static fromObject(object: { [k: string]: any }): fk_data.Status;

        /**
         * Creates a plain object from a Status message. Also converts values to other types if specified.
         * @param message Status
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.Status, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Status to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a LogMessage. */
    interface ILogMessage {

        /** LogMessage time */
        time?: (number|Long|null);

        /** LogMessage uptime */
        uptime?: (number|null);

        /** LogMessage level */
        level?: (number|null);

        /** LogMessage facility */
        facility?: (string|null);

        /** LogMessage message */
        message?: (string|null);
    }

    /** Represents a LogMessage. */
    class LogMessage implements ILogMessage {

        /**
         * Constructs a new LogMessage.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.ILogMessage);

        /** LogMessage time. */
        public time: (number|Long);

        /** LogMessage uptime. */
        public uptime: number;

        /** LogMessage level. */
        public level: number;

        /** LogMessage facility. */
        public facility: string;

        /** LogMessage message. */
        public message: string;

        /**
         * Creates a new LogMessage instance using the specified properties.
         * @param [properties] Properties to set
         * @returns LogMessage instance
         */
        public static create(properties?: fk_data.ILogMessage): fk_data.LogMessage;

        /**
         * Encodes the specified LogMessage message. Does not implicitly {@link fk_data.LogMessage.verify|verify} messages.
         * @param message LogMessage message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.ILogMessage, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified LogMessage message, length delimited. Does not implicitly {@link fk_data.LogMessage.verify|verify} messages.
         * @param message LogMessage message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.ILogMessage, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a LogMessage message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns LogMessage
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.LogMessage;

        /**
         * Decodes a LogMessage message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns LogMessage
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.LogMessage;

        /**
         * Verifies a LogMessage message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a LogMessage message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns LogMessage
         */
        public static fromObject(object: { [k: string]: any }): fk_data.LogMessage;

        /**
         * Creates a plain object from a LogMessage message. Also converts values to other types if specified.
         * @param message LogMessage
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.LogMessage, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this LogMessage to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** DownloadFlags enum. */
    enum DownloadFlags {
        READING_FLAGS_NONE = 0,
        READING_FLAGS_NOT_RECORDING = 1,
        READING_FLAGS_MANUAL = 2
    }

    /** Properties of a SensorGroup. */
    interface ISensorGroup {

        /** SensorGroup module */
        module?: (number|null);

        /** SensorGroup readings */
        readings?: (fk_data.ISensorAndValue[]|null);
    }

    /** Represents a SensorGroup. */
    class SensorGroup implements ISensorGroup {

        /**
         * Constructs a new SensorGroup.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.ISensorGroup);

        /** SensorGroup module. */
        public module: number;

        /** SensorGroup readings. */
        public readings: fk_data.ISensorAndValue[];

        /**
         * Creates a new SensorGroup instance using the specified properties.
         * @param [properties] Properties to set
         * @returns SensorGroup instance
         */
        public static create(properties?: fk_data.ISensorGroup): fk_data.SensorGroup;

        /**
         * Encodes the specified SensorGroup message. Does not implicitly {@link fk_data.SensorGroup.verify|verify} messages.
         * @param message SensorGroup message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.ISensorGroup, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified SensorGroup message, length delimited. Does not implicitly {@link fk_data.SensorGroup.verify|verify} messages.
         * @param message SensorGroup message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.ISensorGroup, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a SensorGroup message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns SensorGroup
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.SensorGroup;

        /**
         * Decodes a SensorGroup message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns SensorGroup
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.SensorGroup;

        /**
         * Verifies a SensorGroup message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a SensorGroup message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns SensorGroup
         */
        public static fromObject(object: { [k: string]: any }): fk_data.SensorGroup;

        /**
         * Creates a plain object from a SensorGroup message. Also converts values to other types if specified.
         * @param message SensorGroup
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.SensorGroup, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this SensorGroup to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a Readings. */
    interface IReadings {

        /** Readings time */
        time?: (number|Long|null);

        /** Readings reading */
        reading?: (number|Long|null);

        /** Readings flags */
        flags?: (number|null);

        /** Readings meta */
        meta?: (number|Long|null);

        /** Readings uptime */
        uptime?: (number|null);

        /** Readings location */
        location?: (fk_data.IDeviceLocation|null);

        /** Readings sensorGroups */
        sensorGroups?: (fk_data.ISensorGroup[]|null);
    }

    /** Represents a Readings. */
    class Readings implements IReadings {

        /**
         * Constructs a new Readings.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.IReadings);

        /** Readings time. */
        public time: (number|Long);

        /** Readings reading. */
        public reading: (number|Long);

        /** Readings flags. */
        public flags: number;

        /** Readings meta. */
        public meta: (number|Long);

        /** Readings uptime. */
        public uptime: number;

        /** Readings location. */
        public location?: (fk_data.IDeviceLocation|null);

        /** Readings sensorGroups. */
        public sensorGroups: fk_data.ISensorGroup[];

        /**
         * Creates a new Readings instance using the specified properties.
         * @param [properties] Properties to set
         * @returns Readings instance
         */
        public static create(properties?: fk_data.IReadings): fk_data.Readings;

        /**
         * Encodes the specified Readings message. Does not implicitly {@link fk_data.Readings.verify|verify} messages.
         * @param message Readings message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.IReadings, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Readings message, length delimited. Does not implicitly {@link fk_data.Readings.verify|verify} messages.
         * @param message Readings message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.IReadings, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a Readings message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Readings
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.Readings;

        /**
         * Decodes a Readings message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Readings
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.Readings;

        /**
         * Verifies a Readings message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a Readings message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns Readings
         */
        public static fromObject(object: { [k: string]: any }): fk_data.Readings;

        /**
         * Creates a plain object from a Readings message. Also converts values to other types if specified.
         * @param message Readings
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.Readings, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Readings to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of an Interval. */
    interface IInterval {

        /** Interval start */
        start?: (number|Long|null);

        /** Interval end */
        end?: (number|Long|null);

        /** Interval interval */
        interval?: (number|null);
    }

    /** Represents an Interval. */
    class Interval implements IInterval {

        /**
         * Constructs a new Interval.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.IInterval);

        /** Interval start. */
        public start: (number|Long);

        /** Interval end. */
        public end: (number|Long);

        /** Interval interval. */
        public interval: number;

        /**
         * Creates a new Interval instance using the specified properties.
         * @param [properties] Properties to set
         * @returns Interval instance
         */
        public static create(properties?: fk_data.IInterval): fk_data.Interval;

        /**
         * Encodes the specified Interval message. Does not implicitly {@link fk_data.Interval.verify|verify} messages.
         * @param message Interval message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.IInterval, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Interval message, length delimited. Does not implicitly {@link fk_data.Interval.verify|verify} messages.
         * @param message Interval message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.IInterval, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes an Interval message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Interval
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.Interval;

        /**
         * Decodes an Interval message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Interval
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.Interval;

        /**
         * Verifies an Interval message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates an Interval message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns Interval
         */
        public static fromObject(object: { [k: string]: any }): fk_data.Interval;

        /**
         * Creates a plain object from an Interval message. Also converts values to other types if specified.
         * @param message Interval
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.Interval, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Interval to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a JobSchedule. */
    interface IJobSchedule {

        /** JobSchedule cron */
        cron?: (Uint8Array|null);

        /** JobSchedule interval */
        interval?: (number|null);

        /** JobSchedule repeated */
        repeated?: (number|null);

        /** JobSchedule duration */
        duration?: (number|null);

        /** JobSchedule jitter */
        jitter?: (number|null);

        /** JobSchedule intervals */
        intervals?: (fk_data.IInterval[]|null);
    }

    /** Represents a JobSchedule. */
    class JobSchedule implements IJobSchedule {

        /**
         * Constructs a new JobSchedule.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.IJobSchedule);

        /** JobSchedule cron. */
        public cron: Uint8Array;

        /** JobSchedule interval. */
        public interval: number;

        /** JobSchedule repeated. */
        public repeated: number;

        /** JobSchedule duration. */
        public duration: number;

        /** JobSchedule jitter. */
        public jitter: number;

        /** JobSchedule intervals. */
        public intervals: fk_data.IInterval[];

        /**
         * Creates a new JobSchedule instance using the specified properties.
         * @param [properties] Properties to set
         * @returns JobSchedule instance
         */
        public static create(properties?: fk_data.IJobSchedule): fk_data.JobSchedule;

        /**
         * Encodes the specified JobSchedule message. Does not implicitly {@link fk_data.JobSchedule.verify|verify} messages.
         * @param message JobSchedule message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.IJobSchedule, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified JobSchedule message, length delimited. Does not implicitly {@link fk_data.JobSchedule.verify|verify} messages.
         * @param message JobSchedule message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.IJobSchedule, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a JobSchedule message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns JobSchedule
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.JobSchedule;

        /**
         * Decodes a JobSchedule message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns JobSchedule
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.JobSchedule;

        /**
         * Verifies a JobSchedule message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a JobSchedule message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns JobSchedule
         */
        public static fromObject(object: { [k: string]: any }): fk_data.JobSchedule;

        /**
         * Creates a plain object from a JobSchedule message. Also converts values to other types if specified.
         * @param message JobSchedule
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.JobSchedule, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this JobSchedule to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a Schedule. */
    interface ISchedule {

        /** Schedule readings */
        readings?: (fk_data.IJobSchedule|null);

        /** Schedule network */
        network?: (fk_data.IJobSchedule|null);

        /** Schedule lora */
        lora?: (fk_data.IJobSchedule|null);

        /** Schedule gps */
        gps?: (fk_data.IJobSchedule|null);
    }

    /** Represents a Schedule. */
    class Schedule implements ISchedule {

        /**
         * Constructs a new Schedule.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.ISchedule);

        /** Schedule readings. */
        public readings?: (fk_data.IJobSchedule|null);

        /** Schedule network. */
        public network?: (fk_data.IJobSchedule|null);

        /** Schedule lora. */
        public lora?: (fk_data.IJobSchedule|null);

        /** Schedule gps. */
        public gps?: (fk_data.IJobSchedule|null);

        /**
         * Creates a new Schedule instance using the specified properties.
         * @param [properties] Properties to set
         * @returns Schedule instance
         */
        public static create(properties?: fk_data.ISchedule): fk_data.Schedule;

        /**
         * Encodes the specified Schedule message. Does not implicitly {@link fk_data.Schedule.verify|verify} messages.
         * @param message Schedule message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.ISchedule, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Schedule message, length delimited. Does not implicitly {@link fk_data.Schedule.verify|verify} messages.
         * @param message Schedule message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.ISchedule, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a Schedule message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Schedule
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.Schedule;

        /**
         * Decodes a Schedule message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Schedule
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.Schedule;

        /**
         * Verifies a Schedule message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a Schedule message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns Schedule
         */
        public static fromObject(object: { [k: string]: any }): fk_data.Schedule;

        /**
         * Creates a plain object from a Schedule message. Also converts values to other types if specified.
         * @param message Schedule
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.Schedule, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Schedule to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of an Identity. */
    interface IIdentity {

        /** Identity name */
        name?: (string|null);
    }

    /** Represents an Identity. */
    class Identity implements IIdentity {

        /**
         * Constructs a new Identity.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.IIdentity);

        /** Identity name. */
        public name: string;

        /**
         * Creates a new Identity instance using the specified properties.
         * @param [properties] Properties to set
         * @returns Identity instance
         */
        public static create(properties?: fk_data.IIdentity): fk_data.Identity;

        /**
         * Encodes the specified Identity message. Does not implicitly {@link fk_data.Identity.verify|verify} messages.
         * @param message Identity message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.IIdentity, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Identity message, length delimited. Does not implicitly {@link fk_data.Identity.verify|verify} messages.
         * @param message Identity message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.IIdentity, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes an Identity message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Identity
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.Identity;

        /**
         * Decodes an Identity message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Identity
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.Identity;

        /**
         * Verifies an Identity message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates an Identity message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns Identity
         */
        public static fromObject(object: { [k: string]: any }): fk_data.Identity;

        /**
         * Creates a plain object from an Identity message. Also converts values to other types if specified.
         * @param message Identity
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.Identity, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Identity to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** ConditionFlags enum. */
    enum ConditionFlags {
        CONDITION_FLAGS_NONE = 0,
        CONDITION_FLAGS_RECORDING = 1
    }

    /** Properties of a Condition. */
    interface ICondition {

        /** Condition flags */
        flags?: (number|null);

        /** Condition recording */
        recording?: (number|null);
    }

    /** Represents a Condition. */
    class Condition implements ICondition {

        /**
         * Constructs a new Condition.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.ICondition);

        /** Condition flags. */
        public flags: number;

        /** Condition recording. */
        public recording: number;

        /**
         * Creates a new Condition instance using the specified properties.
         * @param [properties] Properties to set
         * @returns Condition instance
         */
        public static create(properties?: fk_data.ICondition): fk_data.Condition;

        /**
         * Encodes the specified Condition message. Does not implicitly {@link fk_data.Condition.verify|verify} messages.
         * @param message Condition message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.ICondition, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Condition message, length delimited. Does not implicitly {@link fk_data.Condition.verify|verify} messages.
         * @param message Condition message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.ICondition, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a Condition message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Condition
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.Condition;

        /**
         * Decodes a Condition message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Condition
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.Condition;

        /**
         * Verifies a Condition message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a Condition message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns Condition
         */
        public static fromObject(object: { [k: string]: any }): fk_data.Condition;

        /**
         * Creates a plain object from a Condition message. Also converts values to other types if specified.
         * @param message Condition
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.Condition, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Condition to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a NetworkInfo. */
    interface INetworkInfo {

        /** NetworkInfo ssid */
        ssid?: (string|null);

        /** NetworkInfo password */
        password?: (string|null);

        /** NetworkInfo create */
        create?: (boolean|null);

        /** NetworkInfo preferred */
        preferred?: (boolean|null);
    }

    /** Represents a NetworkInfo. */
    class NetworkInfo implements INetworkInfo {

        /**
         * Constructs a new NetworkInfo.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.INetworkInfo);

        /** NetworkInfo ssid. */
        public ssid: string;

        /** NetworkInfo password. */
        public password: string;

        /** NetworkInfo create. */
        public create: boolean;

        /** NetworkInfo preferred. */
        public preferred: boolean;

        /**
         * Creates a new NetworkInfo instance using the specified properties.
         * @param [properties] Properties to set
         * @returns NetworkInfo instance
         */
        public static create(properties?: fk_data.INetworkInfo): fk_data.NetworkInfo;

        /**
         * Encodes the specified NetworkInfo message. Does not implicitly {@link fk_data.NetworkInfo.verify|verify} messages.
         * @param message NetworkInfo message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.INetworkInfo, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified NetworkInfo message, length delimited. Does not implicitly {@link fk_data.NetworkInfo.verify|verify} messages.
         * @param message NetworkInfo message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.INetworkInfo, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a NetworkInfo message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns NetworkInfo
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.NetworkInfo;

        /**
         * Decodes a NetworkInfo message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns NetworkInfo
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.NetworkInfo;

        /**
         * Verifies a NetworkInfo message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a NetworkInfo message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns NetworkInfo
         */
        public static fromObject(object: { [k: string]: any }): fk_data.NetworkInfo;

        /**
         * Creates a plain object from a NetworkInfo message. Also converts values to other types if specified.
         * @param message NetworkInfo
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.NetworkInfo, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this NetworkInfo to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a WifiTransmission. */
    interface IWifiTransmission {

        /** WifiTransmission url */
        url?: (string|null);

        /** WifiTransmission token */
        token?: (string|null);

        /** WifiTransmission enabled */
        enabled?: (boolean|null);
    }

    /** Represents a WifiTransmission. */
    class WifiTransmission implements IWifiTransmission {

        /**
         * Constructs a new WifiTransmission.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.IWifiTransmission);

        /** WifiTransmission url. */
        public url: string;

        /** WifiTransmission token. */
        public token: string;

        /** WifiTransmission enabled. */
        public enabled: boolean;

        /**
         * Creates a new WifiTransmission instance using the specified properties.
         * @param [properties] Properties to set
         * @returns WifiTransmission instance
         */
        public static create(properties?: fk_data.IWifiTransmission): fk_data.WifiTransmission;

        /**
         * Encodes the specified WifiTransmission message. Does not implicitly {@link fk_data.WifiTransmission.verify|verify} messages.
         * @param message WifiTransmission message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.IWifiTransmission, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified WifiTransmission message, length delimited. Does not implicitly {@link fk_data.WifiTransmission.verify|verify} messages.
         * @param message WifiTransmission message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.IWifiTransmission, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a WifiTransmission message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns WifiTransmission
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.WifiTransmission;

        /**
         * Decodes a WifiTransmission message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns WifiTransmission
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.WifiTransmission;

        /**
         * Verifies a WifiTransmission message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a WifiTransmission message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns WifiTransmission
         */
        public static fromObject(object: { [k: string]: any }): fk_data.WifiTransmission;

        /**
         * Creates a plain object from a WifiTransmission message. Also converts values to other types if specified.
         * @param message WifiTransmission
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.WifiTransmission, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this WifiTransmission to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a TransmissionSettings. */
    interface ITransmissionSettings {

        /** TransmissionSettings wifi */
        wifi?: (fk_data.IWifiTransmission|null);
    }

    /** Represents a TransmissionSettings. */
    class TransmissionSettings implements ITransmissionSettings {

        /**
         * Constructs a new TransmissionSettings.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.ITransmissionSettings);

        /** TransmissionSettings wifi. */
        public wifi?: (fk_data.IWifiTransmission|null);

        /**
         * Creates a new TransmissionSettings instance using the specified properties.
         * @param [properties] Properties to set
         * @returns TransmissionSettings instance
         */
        public static create(properties?: fk_data.ITransmissionSettings): fk_data.TransmissionSettings;

        /**
         * Encodes the specified TransmissionSettings message. Does not implicitly {@link fk_data.TransmissionSettings.verify|verify} messages.
         * @param message TransmissionSettings message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.ITransmissionSettings, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified TransmissionSettings message, length delimited. Does not implicitly {@link fk_data.TransmissionSettings.verify|verify} messages.
         * @param message TransmissionSettings message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.ITransmissionSettings, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a TransmissionSettings message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns TransmissionSettings
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.TransmissionSettings;

        /**
         * Decodes a TransmissionSettings message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns TransmissionSettings
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.TransmissionSettings;

        /**
         * Verifies a TransmissionSettings message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a TransmissionSettings message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns TransmissionSettings
         */
        public static fromObject(object: { [k: string]: any }): fk_data.TransmissionSettings;

        /**
         * Creates a plain object from a TransmissionSettings message. Also converts values to other types if specified.
         * @param message TransmissionSettings
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.TransmissionSettings, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this TransmissionSettings to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a NetworkSettings. */
    interface INetworkSettings {

        /** NetworkSettings networks */
        networks?: (fk_data.INetworkInfo[]|null);
    }

    /** Represents a NetworkSettings. */
    class NetworkSettings implements INetworkSettings {

        /**
         * Constructs a new NetworkSettings.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.INetworkSettings);

        /** NetworkSettings networks. */
        public networks: fk_data.INetworkInfo[];

        /**
         * Creates a new NetworkSettings instance using the specified properties.
         * @param [properties] Properties to set
         * @returns NetworkSettings instance
         */
        public static create(properties?: fk_data.INetworkSettings): fk_data.NetworkSettings;

        /**
         * Encodes the specified NetworkSettings message. Does not implicitly {@link fk_data.NetworkSettings.verify|verify} messages.
         * @param message NetworkSettings message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.INetworkSettings, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified NetworkSettings message, length delimited. Does not implicitly {@link fk_data.NetworkSettings.verify|verify} messages.
         * @param message NetworkSettings message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.INetworkSettings, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a NetworkSettings message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns NetworkSettings
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.NetworkSettings;

        /**
         * Decodes a NetworkSettings message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns NetworkSettings
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.NetworkSettings;

        /**
         * Verifies a NetworkSettings message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a NetworkSettings message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns NetworkSettings
         */
        public static fromObject(object: { [k: string]: any }): fk_data.NetworkSettings;

        /**
         * Creates a plain object from a NetworkSettings message. Also converts values to other types if specified.
         * @param message NetworkSettings
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.NetworkSettings, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this NetworkSettings to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a LoraSettings. */
    interface ILoraSettings {

        /** LoraSettings deviceEui */
        deviceEui?: (Uint8Array|null);

        /** LoraSettings appKey */
        appKey?: (Uint8Array|null);

        /** LoraSettings joinEui */
        joinEui?: (Uint8Array|null);

        /** LoraSettings frequencyBand */
        frequencyBand?: (number|null);

        /** LoraSettings deviceAddress */
        deviceAddress?: (Uint8Array|null);

        /** LoraSettings networkSessionKey */
        networkSessionKey?: (Uint8Array|null);

        /** LoraSettings appSessionKey */
        appSessionKey?: (Uint8Array|null);

        /** LoraSettings uplinkCounter */
        uplinkCounter?: (number|null);

        /** LoraSettings downlinkCounter */
        downlinkCounter?: (number|null);

        /** LoraSettings rxDelay1 */
        rxDelay1?: (number|null);

        /** LoraSettings rxDelay2 */
        rxDelay2?: (number|null);
    }

    /** Represents a LoraSettings. */
    class LoraSettings implements ILoraSettings {

        /**
         * Constructs a new LoraSettings.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.ILoraSettings);

        /** LoraSettings deviceEui. */
        public deviceEui: Uint8Array;

        /** LoraSettings appKey. */
        public appKey: Uint8Array;

        /** LoraSettings joinEui. */
        public joinEui: Uint8Array;

        /** LoraSettings frequencyBand. */
        public frequencyBand: number;

        /** LoraSettings deviceAddress. */
        public deviceAddress: Uint8Array;

        /** LoraSettings networkSessionKey. */
        public networkSessionKey: Uint8Array;

        /** LoraSettings appSessionKey. */
        public appSessionKey: Uint8Array;

        /** LoraSettings uplinkCounter. */
        public uplinkCounter: number;

        /** LoraSettings downlinkCounter. */
        public downlinkCounter: number;

        /** LoraSettings rxDelay1. */
        public rxDelay1: number;

        /** LoraSettings rxDelay2. */
        public rxDelay2: number;

        /**
         * Creates a new LoraSettings instance using the specified properties.
         * @param [properties] Properties to set
         * @returns LoraSettings instance
         */
        public static create(properties?: fk_data.ILoraSettings): fk_data.LoraSettings;

        /**
         * Encodes the specified LoraSettings message. Does not implicitly {@link fk_data.LoraSettings.verify|verify} messages.
         * @param message LoraSettings message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.ILoraSettings, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified LoraSettings message, length delimited. Does not implicitly {@link fk_data.LoraSettings.verify|verify} messages.
         * @param message LoraSettings message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.ILoraSettings, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a LoraSettings message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns LoraSettings
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.LoraSettings;

        /**
         * Decodes a LoraSettings message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns LoraSettings
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.LoraSettings;

        /**
         * Verifies a LoraSettings message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a LoraSettings message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns LoraSettings
         */
        public static fromObject(object: { [k: string]: any }): fk_data.LoraSettings;

        /**
         * Creates a plain object from a LoraSettings message. Also converts values to other types if specified.
         * @param message LoraSettings
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.LoraSettings, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this LoraSettings to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a Fault. */
    interface IFault {

        /** Fault time */
        time?: (number|null);

        /** Fault code */
        code?: (number|null);

        /** Fault description */
        description?: (string|null);

        /** Fault debug */
        debug?: (Uint8Array|null);
    }

    /** Represents a Fault. */
    class Fault implements IFault {

        /**
         * Constructs a new Fault.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.IFault);

        /** Fault time. */
        public time: number;

        /** Fault code. */
        public code: number;

        /** Fault description. */
        public description: string;

        /** Fault debug. */
        public debug: Uint8Array;

        /**
         * Creates a new Fault instance using the specified properties.
         * @param [properties] Properties to set
         * @returns Fault instance
         */
        public static create(properties?: fk_data.IFault): fk_data.Fault;

        /**
         * Encodes the specified Fault message. Does not implicitly {@link fk_data.Fault.verify|verify} messages.
         * @param message Fault message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.IFault, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Fault message, length delimited. Does not implicitly {@link fk_data.Fault.verify|verify} messages.
         * @param message Fault message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.IFault, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a Fault message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Fault
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.Fault;

        /**
         * Decodes a Fault message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Fault
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.Fault;

        /**
         * Verifies a Fault message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a Fault message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns Fault
         */
        public static fromObject(object: { [k: string]: any }): fk_data.Fault;

        /**
         * Creates a plain object from a Fault message. Also converts values to other types if specified.
         * @param message Fault
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.Fault, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Fault to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a DataRecord. */
    interface IDataRecord {

        /** DataRecord loggedReading */
        loggedReading?: (fk_data.ILoggedReading|null);

        /** DataRecord metadata */
        metadata?: (fk_data.IMetadata|null);

        /** DataRecord log */
        log?: (fk_data.ILogMessage|null);

        /** DataRecord logs */
        logs?: (fk_data.ILogMessage[]|null);

        /** DataRecord status */
        status?: (fk_data.IStatus|null);

        /** DataRecord readings */
        readings?: (fk_data.IReadings|null);

        /** DataRecord modules */
        modules?: (fk_data.IModuleInfo[]|null);

        /** DataRecord schedule */
        schedule?: (fk_data.ISchedule|null);

        /** DataRecord meta */
        meta?: (number|Long|null);

        /** DataRecord identity */
        identity?: (fk_data.IIdentity|null);

        /** DataRecord condition */
        condition?: (fk_data.ICondition|null);

        /** DataRecord lora */
        lora?: (fk_data.ILoraSettings|null);

        /** DataRecord network */
        network?: (fk_data.INetworkSettings|null);

        /** DataRecord transmission */
        transmission?: (fk_data.ITransmissionSettings|null);

        /** DataRecord faults */
        faults?: (fk_data.IFault[]|null);
    }

    /** I may break this into a MetaRecord. */
    class DataRecord implements IDataRecord {

        /**
         * Constructs a new DataRecord.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.IDataRecord);

        /** DataRecord loggedReading. */
        public loggedReading?: (fk_data.ILoggedReading|null);

        /** DataRecord metadata. */
        public metadata?: (fk_data.IMetadata|null);

        /** DataRecord log. */
        public log?: (fk_data.ILogMessage|null);

        /** DataRecord logs. */
        public logs: fk_data.ILogMessage[];

        /** DataRecord status. */
        public status?: (fk_data.IStatus|null);

        /** DataRecord readings. */
        public readings?: (fk_data.IReadings|null);

        /** DataRecord modules. */
        public modules: fk_data.IModuleInfo[];

        /** DataRecord schedule. */
        public schedule?: (fk_data.ISchedule|null);

        /** DataRecord meta. */
        public meta: (number|Long);

        /** DataRecord identity. */
        public identity?: (fk_data.IIdentity|null);

        /** DataRecord condition. */
        public condition?: (fk_data.ICondition|null);

        /** DataRecord lora. */
        public lora?: (fk_data.ILoraSettings|null);

        /** DataRecord network. */
        public network?: (fk_data.INetworkSettings|null);

        /** DataRecord transmission. */
        public transmission?: (fk_data.ITransmissionSettings|null);

        /** DataRecord faults. */
        public faults: fk_data.IFault[];

        /**
         * Creates a new DataRecord instance using the specified properties.
         * @param [properties] Properties to set
         * @returns DataRecord instance
         */
        public static create(properties?: fk_data.IDataRecord): fk_data.DataRecord;

        /**
         * Encodes the specified DataRecord message. Does not implicitly {@link fk_data.DataRecord.verify|verify} messages.
         * @param message DataRecord message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.IDataRecord, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified DataRecord message, length delimited. Does not implicitly {@link fk_data.DataRecord.verify|verify} messages.
         * @param message DataRecord message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.IDataRecord, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a DataRecord message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns DataRecord
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.DataRecord;

        /**
         * Decodes a DataRecord message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns DataRecord
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.DataRecord;

        /**
         * Verifies a DataRecord message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a DataRecord message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns DataRecord
         */
        public static fromObject(object: { [k: string]: any }): fk_data.DataRecord;

        /**
         * Creates a plain object from a DataRecord message. Also converts values to other types if specified.
         * @param message DataRecord
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.DataRecord, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this DataRecord to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** SignedRecordKind enum. */
    enum SignedRecordKind {
        SIGNED_RECORD_KIND_NONE = 0,
        SIGNED_RECORD_KIND_MODULES = 1,
        SIGNED_RECORD_KIND_SCHEDULE = 2,
        SIGNED_RECORD_KIND_STATE = 3,
        SIGNED_RECORD_KIND_RAW_STATE = 4,
        SIGNED_RECORD_KIND_FAULTS = 5,
        SIGNED_RECORD_KIND_OTHER = 255
    }

    /** Properties of a SignedRecord. */
    interface ISignedRecord {

        /** SignedRecord kind */
        kind?: (fk_data.SignedRecordKind|null);

        /** SignedRecord time */
        time?: (number|Long|null);

        /** SignedRecord data */
        data?: (Uint8Array|null);

        /** SignedRecord hash */
        hash?: (Uint8Array|null);

        /** SignedRecord record */
        record?: (number|Long|null);
    }

    /** Represents a SignedRecord. */
    class SignedRecord implements ISignedRecord {

        /**
         * Constructs a new SignedRecord.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.ISignedRecord);

        /** SignedRecord kind. */
        public kind: fk_data.SignedRecordKind;

        /** SignedRecord time. */
        public time: (number|Long);

        /** SignedRecord data. */
        public data: Uint8Array;

        /** SignedRecord hash. */
        public hash: Uint8Array;

        /** SignedRecord record. */
        public record: (number|Long);

        /**
         * Creates a new SignedRecord instance using the specified properties.
         * @param [properties] Properties to set
         * @returns SignedRecord instance
         */
        public static create(properties?: fk_data.ISignedRecord): fk_data.SignedRecord;

        /**
         * Encodes the specified SignedRecord message. Does not implicitly {@link fk_data.SignedRecord.verify|verify} messages.
         * @param message SignedRecord message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.ISignedRecord, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified SignedRecord message, length delimited. Does not implicitly {@link fk_data.SignedRecord.verify|verify} messages.
         * @param message SignedRecord message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.ISignedRecord, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a SignedRecord message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns SignedRecord
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.SignedRecord;

        /**
         * Decodes a SignedRecord message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns SignedRecord
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.SignedRecord;

        /**
         * Verifies a SignedRecord message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a SignedRecord message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns SignedRecord
         */
        public static fromObject(object: { [k: string]: any }): fk_data.SignedRecord;

        /**
         * Creates a plain object from a SignedRecord message. Also converts values to other types if specified.
         * @param message SignedRecord
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.SignedRecord, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this SignedRecord to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a LoraRecord. */
    interface ILoraRecord {

        /** LoraRecord deviceId */
        deviceId?: (Uint8Array|null);

        /** LoraRecord time */
        time?: (number|Long|null);

        /** LoraRecord number */
        number?: (number|Long|null);

        /** LoraRecord module */
        module?: (number|null);

        /** LoraRecord sensor */
        sensor?: (number|Long|null);

        /** LoraRecord values */
        values?: (number[]|null);

        /** LoraRecord data */
        data?: (Uint8Array|null);
    }

    /** Represents a LoraRecord. */
    class LoraRecord implements ILoraRecord {

        /**
         * Constructs a new LoraRecord.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.ILoraRecord);

        /** LoraRecord deviceId. */
        public deviceId: Uint8Array;

        /** LoraRecord time. */
        public time: (number|Long);

        /** LoraRecord number. */
        public number: (number|Long);

        /** LoraRecord module. */
        public module: number;

        /** LoraRecord sensor. */
        public sensor: (number|Long);

        /** LoraRecord values. */
        public values: number[];

        /** LoraRecord data. */
        public data: Uint8Array;

        /**
         * Creates a new LoraRecord instance using the specified properties.
         * @param [properties] Properties to set
         * @returns LoraRecord instance
         */
        public static create(properties?: fk_data.ILoraRecord): fk_data.LoraRecord;

        /**
         * Encodes the specified LoraRecord message. Does not implicitly {@link fk_data.LoraRecord.verify|verify} messages.
         * @param message LoraRecord message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.ILoraRecord, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified LoraRecord message, length delimited. Does not implicitly {@link fk_data.LoraRecord.verify|verify} messages.
         * @param message LoraRecord message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.ILoraRecord, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a LoraRecord message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns LoraRecord
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.LoraRecord;

        /**
         * Decodes a LoraRecord message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns LoraRecord
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.LoraRecord;

        /**
         * Verifies a LoraRecord message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a LoraRecord message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns LoraRecord
         */
        public static fromObject(object: { [k: string]: any }): fk_data.LoraRecord;

        /**
         * Creates a plain object from a LoraRecord message. Also converts values to other types if specified.
         * @param message LoraRecord
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.LoraRecord, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this LoraRecord to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** CurveType enum. */
    enum CurveType {
        CURVE_NONE = 0,
        CURVE_LINEAR = 1,
        CURVE_EXPONENTIAL = 2,
        CURVE_LOGARITHMIC = 3
    }

    /** Properties of a CalibrationPoint. */
    interface ICalibrationPoint {

        /** CalibrationPoint references */
        references?: (number[]|null);

        /** CalibrationPoint uncalibrated */
        uncalibrated?: (number[]|null);

        /** CalibrationPoint factory */
        factory?: (number[]|null);
    }

    /** Represents a CalibrationPoint. */
    class CalibrationPoint implements ICalibrationPoint {

        /**
         * Constructs a new CalibrationPoint.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.ICalibrationPoint);

        /** CalibrationPoint references. */
        public references: number[];

        /** CalibrationPoint uncalibrated. */
        public uncalibrated: number[];

        /** CalibrationPoint factory. */
        public factory: number[];

        /**
         * Creates a new CalibrationPoint instance using the specified properties.
         * @param [properties] Properties to set
         * @returns CalibrationPoint instance
         */
        public static create(properties?: fk_data.ICalibrationPoint): fk_data.CalibrationPoint;

        /**
         * Encodes the specified CalibrationPoint message. Does not implicitly {@link fk_data.CalibrationPoint.verify|verify} messages.
         * @param message CalibrationPoint message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.ICalibrationPoint, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified CalibrationPoint message, length delimited. Does not implicitly {@link fk_data.CalibrationPoint.verify|verify} messages.
         * @param message CalibrationPoint message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.ICalibrationPoint, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a CalibrationPoint message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns CalibrationPoint
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.CalibrationPoint;

        /**
         * Decodes a CalibrationPoint message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns CalibrationPoint
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.CalibrationPoint;

        /**
         * Verifies a CalibrationPoint message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a CalibrationPoint message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns CalibrationPoint
         */
        public static fromObject(object: { [k: string]: any }): fk_data.CalibrationPoint;

        /**
         * Creates a plain object from a CalibrationPoint message. Also converts values to other types if specified.
         * @param message CalibrationPoint
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.CalibrationPoint, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this CalibrationPoint to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a CalibrationCoefficients. */
    interface ICalibrationCoefficients {

        /** CalibrationCoefficients values */
        values?: (number[]|null);
    }

    /** Represents a CalibrationCoefficients. */
    class CalibrationCoefficients implements ICalibrationCoefficients {

        /**
         * Constructs a new CalibrationCoefficients.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.ICalibrationCoefficients);

        /** CalibrationCoefficients values. */
        public values: number[];

        /**
         * Creates a new CalibrationCoefficients instance using the specified properties.
         * @param [properties] Properties to set
         * @returns CalibrationCoefficients instance
         */
        public static create(properties?: fk_data.ICalibrationCoefficients): fk_data.CalibrationCoefficients;

        /**
         * Encodes the specified CalibrationCoefficients message. Does not implicitly {@link fk_data.CalibrationCoefficients.verify|verify} messages.
         * @param message CalibrationCoefficients message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.ICalibrationCoefficients, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified CalibrationCoefficients message, length delimited. Does not implicitly {@link fk_data.CalibrationCoefficients.verify|verify} messages.
         * @param message CalibrationCoefficients message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.ICalibrationCoefficients, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a CalibrationCoefficients message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns CalibrationCoefficients
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.CalibrationCoefficients;

        /**
         * Decodes a CalibrationCoefficients message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns CalibrationCoefficients
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.CalibrationCoefficients;

        /**
         * Verifies a CalibrationCoefficients message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a CalibrationCoefficients message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns CalibrationCoefficients
         */
        public static fromObject(object: { [k: string]: any }): fk_data.CalibrationCoefficients;

        /**
         * Creates a plain object from a CalibrationCoefficients message. Also converts values to other types if specified.
         * @param message CalibrationCoefficients
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.CalibrationCoefficients, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this CalibrationCoefficients to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a Calibration. */
    interface ICalibration {

        /** Calibration type */
        type?: (fk_data.CurveType|null);

        /** Calibration time */
        time?: (number|null);

        /** Calibration points */
        points?: (fk_data.ICalibrationPoint[]|null);

        /** Calibration coefficients */
        coefficients?: (fk_data.ICalibrationCoefficients|null);
    }

    /** Represents a Calibration. */
    class Calibration implements ICalibration {

        /**
         * Constructs a new Calibration.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.ICalibration);

        /** Calibration type. */
        public type: fk_data.CurveType;

        /** Calibration time. */
        public time: number;

        /** Calibration points. */
        public points: fk_data.ICalibrationPoint[];

        /** Calibration coefficients. */
        public coefficients?: (fk_data.ICalibrationCoefficients|null);

        /**
         * Creates a new Calibration instance using the specified properties.
         * @param [properties] Properties to set
         * @returns Calibration instance
         */
        public static create(properties?: fk_data.ICalibration): fk_data.Calibration;

        /**
         * Encodes the specified Calibration message. Does not implicitly {@link fk_data.Calibration.verify|verify} messages.
         * @param message Calibration message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.ICalibration, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Calibration message, length delimited. Does not implicitly {@link fk_data.Calibration.verify|verify} messages.
         * @param message Calibration message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.ICalibration, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a Calibration message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Calibration
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.Calibration;

        /**
         * Decodes a Calibration message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Calibration
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.Calibration;

        /**
         * Verifies a Calibration message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a Calibration message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns Calibration
         */
        public static fromObject(object: { [k: string]: any }): fk_data.Calibration;

        /**
         * Creates a plain object from a Calibration message. Also converts values to other types if specified.
         * @param message Calibration
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.Calibration, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Calibration to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a ModuleConfiguration. */
    interface IModuleConfiguration {

        /** ModuleConfiguration calibration */
        calibration?: (fk_data.ICalibration|null);
    }

    /** Represents a ModuleConfiguration. */
    class ModuleConfiguration implements IModuleConfiguration {

        /**
         * Constructs a new ModuleConfiguration.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_data.IModuleConfiguration);

        /** ModuleConfiguration calibration. */
        public calibration?: (fk_data.ICalibration|null);

        /**
         * Creates a new ModuleConfiguration instance using the specified properties.
         * @param [properties] Properties to set
         * @returns ModuleConfiguration instance
         */
        public static create(properties?: fk_data.IModuleConfiguration): fk_data.ModuleConfiguration;

        /**
         * Encodes the specified ModuleConfiguration message. Does not implicitly {@link fk_data.ModuleConfiguration.verify|verify} messages.
         * @param message ModuleConfiguration message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_data.IModuleConfiguration, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified ModuleConfiguration message, length delimited. Does not implicitly {@link fk_data.ModuleConfiguration.verify|verify} messages.
         * @param message ModuleConfiguration message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_data.IModuleConfiguration, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a ModuleConfiguration message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns ModuleConfiguration
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_data.ModuleConfiguration;

        /**
         * Decodes a ModuleConfiguration message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns ModuleConfiguration
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_data.ModuleConfiguration;

        /**
         * Verifies a ModuleConfiguration message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a ModuleConfiguration message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns ModuleConfiguration
         */
        public static fromObject(object: { [k: string]: any }): fk_data.ModuleConfiguration;

        /**
         * Creates a plain object from a ModuleConfiguration message. Also converts values to other types if specified.
         * @param message ModuleConfiguration
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_data.ModuleConfiguration, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this ModuleConfiguration to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }
}
