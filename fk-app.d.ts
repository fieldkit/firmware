import * as $protobuf from "protobufjs";

/** Namespace fk_app. */
export namespace fk_app {

    /** QueryFlags enum. */
    enum QueryFlags {
        QUERY_FLAGS_NONE = 0,
        QUERY_FLAGS_LOGS = 1
    }

    /** QueryType enum. */
    enum QueryType {
        QUERY_NONE = 0,
        QUERY_CAPABILITIES = 1,
        QUERY_CONFIGURE_SENSOR = 2,
        QUERY_LIVE_DATA_POLL = 7,
        QUERY_SCHEDULES = 8,
        QUERY_CONFIGURE_SCHEDULES = 9,
        QUERY_FILES_SD = 10,
        QUERY_DOWNLOAD_FILE = 11,
        QUERY_ERASE_FILE = 12,
        QUERY_RESET = 13,
        QUERY_NETWORK_SETTINGS = 14,
        QUERY_CONFIGURE_NETWORK_SETTINGS = 15,
        QUERY_SCAN_MODULES = 16,
        QUERY_CONFIGURE_IDENTITY = 17,
        QUERY_STATUS = 18,
        QUERY_MODULE = 19,
        QUERY_METADATA = 20,
        QUERY_FORMAT = 21,
        QUERY_GET_READINGS = 22,
        QUERY_TAKE_READINGS = 23,
        QUERY_RECORDING_CONTROL = 24,
        QUERY_CONFIGURE = 25,
        QUERY_SCAN_NETWORKS = 26,
        QUERY_FILES_SPI = 27,
        QUERY_FILES_QSPI = 28
    }

    /** ReplyType enum. */
    enum ReplyType {
        REPLY_NONE = 0,
        REPLY_SUCCESS = 1,
        REPLY_BUSY = 2,
        REPLY_ERROR = 3,
        REPLY_CAPABILITIES = 4,
        REPLY_LIVE_DATA_POLL = 8,
        REPLY_SCHEDULES = 9,
        REPLY_FILES = 10,
        REPLY_DOWNLOAD_FILE = 11,
        REPLY_RESET = 12,
        REPLY_NETWORK_SETTINGS = 13,
        REPLY_IDENTITY = 14,
        REPLY_STATUS = 15,
        REPLY_MODULE = 16,
        REPLY_METADATA = 17,
        REPLY_READINGS = 18,
        REPLY_NETWORKS = 19
    }

    /** DownloadFlags enum. */
    enum DownloadFlags {
        DOWNLOAD_FLAG_NONE = 0,
        DOWNLOAD_FLAG_METADATA_PREPEND = 1,
        DOWNLOAD_FLAG_METADATA_ONLY = 2
    }

    /** ModuleFlags enum. */
    enum ModuleFlags {
        MODULE_FLAG_NONE = 0,
        MODULE_FLAG_INTERNAL = 1
    }

    /** SensorFlags enum. */
    enum SensorFlags {
        SENSOR_FLAG_NONE = 0
    }

    /** Properties of a QueryCapabilities. */
    interface IQueryCapabilities {

        /** QueryCapabilities version */
        version?: number;

        /** QueryCapabilities callerTime */
        callerTime?: number;
    }

    /** Represents a QueryCapabilities. */
    class QueryCapabilities {

        /**
         * Constructs a new QueryCapabilities.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IQueryCapabilities);

        /** QueryCapabilities version. */
        public version: number;

        /** QueryCapabilities callerTime. */
        public callerTime: number;

        /**
         * Creates a new QueryCapabilities instance using the specified properties.
         * @param [properties] Properties to set
         * @returns QueryCapabilities instance
         */
        public static create(properties?: fk_app.IQueryCapabilities): fk_app.QueryCapabilities;

        /**
         * Encodes the specified QueryCapabilities message. Does not implicitly {@link fk_app.QueryCapabilities.verify|verify} messages.
         * @param message QueryCapabilities message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IQueryCapabilities, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified QueryCapabilities message, length delimited. Does not implicitly {@link fk_app.QueryCapabilities.verify|verify} messages.
         * @param message QueryCapabilities message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IQueryCapabilities, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a QueryCapabilities message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns QueryCapabilities
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.QueryCapabilities;

        /**
         * Decodes a QueryCapabilities message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns QueryCapabilities
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.QueryCapabilities;

        /**
         * Verifies a QueryCapabilities message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a QueryCapabilities message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns QueryCapabilities
         */
        public static fromObject(object: { [k: string]: any }): fk_app.QueryCapabilities;

        /**
         * Creates a plain object from a QueryCapabilities message. Also converts values to other types if specified.
         * @param message QueryCapabilities
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.QueryCapabilities, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this QueryCapabilities to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a LiveValue. */
    interface ILiveValue {

        /** LiveValue valid */
        valid?: boolean;

        /** LiveValue value */
        value?: number;

        /** LiveValue uncalibrated */
        uncalibrated?: number;
    }

    /** Represents a LiveValue. */
    class LiveValue {

        /**
         * Constructs a new LiveValue.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.ILiveValue);

        /** LiveValue valid. */
        public valid: boolean;

        /** LiveValue value. */
        public value: number;

        /** LiveValue uncalibrated. */
        public uncalibrated: number;

        /**
         * Creates a new LiveValue instance using the specified properties.
         * @param [properties] Properties to set
         * @returns LiveValue instance
         */
        public static create(properties?: fk_app.ILiveValue): fk_app.LiveValue;

        /**
         * Encodes the specified LiveValue message. Does not implicitly {@link fk_app.LiveValue.verify|verify} messages.
         * @param message LiveValue message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.ILiveValue, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified LiveValue message, length delimited. Does not implicitly {@link fk_app.LiveValue.verify|verify} messages.
         * @param message LiveValue message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.ILiveValue, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a LiveValue message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns LiveValue
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.LiveValue;

        /**
         * Decodes a LiveValue message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns LiveValue
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.LiveValue;

        /**
         * Verifies a LiveValue message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a LiveValue message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns LiveValue
         */
        public static fromObject(object: { [k: string]: any }): fk_app.LiveValue;

        /**
         * Creates a plain object from a LiveValue message. Also converts values to other types if specified.
         * @param message LiveValue
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.LiveValue, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this LiveValue to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a SensorCapabilities. */
    interface ISensorCapabilities {

        /** SensorCapabilities number */
        number?: number;

        /** SensorCapabilities module */
        module?: number;

        /** SensorCapabilities name */
        name?: string;

        /** SensorCapabilities frequency */
        frequency?: number;

        /** SensorCapabilities unitOfMeasure */
        unitOfMeasure?: string;

        /** SensorCapabilities path */
        path?: string;

        /** SensorCapabilities flags */
        flags?: number;

        /** SensorCapabilities value */
        value?: fk_app.ILiveValue;
    }

    /** Represents a SensorCapabilities. */
    class SensorCapabilities {

        /**
         * Constructs a new SensorCapabilities.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.ISensorCapabilities);

        /** SensorCapabilities number. */
        public number: number;

        /** SensorCapabilities module. */
        public module: number;

        /** SensorCapabilities name. */
        public name: string;

        /** SensorCapabilities frequency. */
        public frequency: number;

        /** SensorCapabilities unitOfMeasure. */
        public unitOfMeasure: string;

        /** SensorCapabilities path. */
        public path: string;

        /** SensorCapabilities flags. */
        public flags: number;

        /** SensorCapabilities value. */
        public value?: (fk_app.ILiveValue|null);

        /**
         * Creates a new SensorCapabilities instance using the specified properties.
         * @param [properties] Properties to set
         * @returns SensorCapabilities instance
         */
        public static create(properties?: fk_app.ISensorCapabilities): fk_app.SensorCapabilities;

        /**
         * Encodes the specified SensorCapabilities message. Does not implicitly {@link fk_app.SensorCapabilities.verify|verify} messages.
         * @param message SensorCapabilities message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.ISensorCapabilities, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified SensorCapabilities message, length delimited. Does not implicitly {@link fk_app.SensorCapabilities.verify|verify} messages.
         * @param message SensorCapabilities message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.ISensorCapabilities, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a SensorCapabilities message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns SensorCapabilities
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.SensorCapabilities;

        /**
         * Decodes a SensorCapabilities message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns SensorCapabilities
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.SensorCapabilities;

        /**
         * Verifies a SensorCapabilities message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a SensorCapabilities message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns SensorCapabilities
         */
        public static fromObject(object: { [k: string]: any }): fk_app.SensorCapabilities;

        /**
         * Creates a plain object from a SensorCapabilities message. Also converts values to other types if specified.
         * @param message SensorCapabilities
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.SensorCapabilities, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this SensorCapabilities to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a ModuleHeader. */
    interface IModuleHeader {

        /** ModuleHeader manufacturer */
        manufacturer?: number;

        /** ModuleHeader kind */
        kind?: number;

        /** ModuleHeader version */
        version?: number;
    }

    /** Represents a ModuleHeader. */
    class ModuleHeader {

        /**
         * Constructs a new ModuleHeader.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IModuleHeader);

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
        public static create(properties?: fk_app.IModuleHeader): fk_app.ModuleHeader;

        /**
         * Encodes the specified ModuleHeader message. Does not implicitly {@link fk_app.ModuleHeader.verify|verify} messages.
         * @param message ModuleHeader message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IModuleHeader, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified ModuleHeader message, length delimited. Does not implicitly {@link fk_app.ModuleHeader.verify|verify} messages.
         * @param message ModuleHeader message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IModuleHeader, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a ModuleHeader message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns ModuleHeader
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.ModuleHeader;

        /**
         * Decodes a ModuleHeader message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns ModuleHeader
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.ModuleHeader;

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
        public static fromObject(object: { [k: string]: any }): fk_app.ModuleHeader;

        /**
         * Creates a plain object from a ModuleHeader message. Also converts values to other types if specified.
         * @param message ModuleHeader
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.ModuleHeader, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this ModuleHeader to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a ModuleCapabilities. */
    interface IModuleCapabilities {

        /** ModuleCapabilities position */
        position?: number;

        /** ModuleCapabilities name */
        name?: string;

        /** ModuleCapabilities sensors */
        sensors?: fk_app.ISensorCapabilities[];

        /** ModuleCapabilities path */
        path?: string;

        /** ModuleCapabilities flags */
        flags?: number;

        /** ModuleCapabilities id */
        id?: Uint8Array;

        /** ModuleCapabilities header */
        header?: fk_app.IModuleHeader;

        /** ModuleCapabilities configuration */
        configuration?: Uint8Array;
    }

    /** Represents a ModuleCapabilities. */
    class ModuleCapabilities {

        /**
         * Constructs a new ModuleCapabilities.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IModuleCapabilities);

        /** ModuleCapabilities position. */
        public position: number;

        /** ModuleCapabilities name. */
        public name: string;

        /** ModuleCapabilities sensors. */
        public sensors: fk_app.ISensorCapabilities[];

        /** ModuleCapabilities path. */
        public path: string;

        /** ModuleCapabilities flags. */
        public flags: number;

        /** ModuleCapabilities id. */
        public id: Uint8Array;

        /** ModuleCapabilities header. */
        public header?: (fk_app.IModuleHeader|null);

        /** ModuleCapabilities configuration. */
        public configuration: Uint8Array;

        /**
         * Creates a new ModuleCapabilities instance using the specified properties.
         * @param [properties] Properties to set
         * @returns ModuleCapabilities instance
         */
        public static create(properties?: fk_app.IModuleCapabilities): fk_app.ModuleCapabilities;

        /**
         * Encodes the specified ModuleCapabilities message. Does not implicitly {@link fk_app.ModuleCapabilities.verify|verify} messages.
         * @param message ModuleCapabilities message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IModuleCapabilities, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified ModuleCapabilities message, length delimited. Does not implicitly {@link fk_app.ModuleCapabilities.verify|verify} messages.
         * @param message ModuleCapabilities message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IModuleCapabilities, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a ModuleCapabilities message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns ModuleCapabilities
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.ModuleCapabilities;

        /**
         * Decodes a ModuleCapabilities message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns ModuleCapabilities
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.ModuleCapabilities;

        /**
         * Verifies a ModuleCapabilities message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a ModuleCapabilities message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns ModuleCapabilities
         */
        public static fromObject(object: { [k: string]: any }): fk_app.ModuleCapabilities;

        /**
         * Creates a plain object from a ModuleCapabilities message. Also converts values to other types if specified.
         * @param message ModuleCapabilities
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.ModuleCapabilities, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this ModuleCapabilities to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a Capabilities. */
    interface ICapabilities {

        /** Capabilities version */
        version?: number;

        /** Capabilities deviceId */
        deviceId?: Uint8Array;

        /** Capabilities name */
        name?: string;

        /** Capabilities modules */
        modules?: fk_app.IModuleCapabilities[];

        /** Capabilities sensors */
        sensors?: fk_app.ISensorCapabilities[];
    }

    /** Represents a Capabilities. */
    class Capabilities {

        /**
         * Constructs a new Capabilities.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.ICapabilities);

        /** Capabilities version. */
        public version: number;

        /** Capabilities deviceId. */
        public deviceId: Uint8Array;

        /** Capabilities name. */
        public name: string;

        /** Capabilities modules. */
        public modules: fk_app.IModuleCapabilities[];

        /** Capabilities sensors. */
        public sensors: fk_app.ISensorCapabilities[];

        /**
         * Creates a new Capabilities instance using the specified properties.
         * @param [properties] Properties to set
         * @returns Capabilities instance
         */
        public static create(properties?: fk_app.ICapabilities): fk_app.Capabilities;

        /**
         * Encodes the specified Capabilities message. Does not implicitly {@link fk_app.Capabilities.verify|verify} messages.
         * @param message Capabilities message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.ICapabilities, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Capabilities message, length delimited. Does not implicitly {@link fk_app.Capabilities.verify|verify} messages.
         * @param message Capabilities message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.ICapabilities, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a Capabilities message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Capabilities
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.Capabilities;

        /**
         * Decodes a Capabilities message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Capabilities
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.Capabilities;

        /**
         * Verifies a Capabilities message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a Capabilities message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns Capabilities
         */
        public static fromObject(object: { [k: string]: any }): fk_app.Capabilities;

        /**
         * Creates a plain object from a Capabilities message. Also converts values to other types if specified.
         * @param message Capabilities
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.Capabilities, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Capabilities to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a NetworkInfo. */
    interface INetworkInfo {

        /** NetworkInfo ssid */
        ssid?: string;

        /** NetworkInfo password */
        password?: string;

        /** NetworkInfo create */
        create?: boolean;

        /** NetworkInfo preferred */
        preferred?: boolean;

        /** NetworkInfo keeping */
        keeping?: boolean;
    }

    /** Represents a NetworkInfo. */
    class NetworkInfo {

        /**
         * Constructs a new NetworkInfo.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.INetworkInfo);

        /** NetworkInfo ssid. */
        public ssid: string;

        /** NetworkInfo password. */
        public password: string;

        /** NetworkInfo create. */
        public create: boolean;

        /** NetworkInfo preferred. */
        public preferred: boolean;

        /** NetworkInfo keeping. */
        public keeping: boolean;

        /**
         * Creates a new NetworkInfo instance using the specified properties.
         * @param [properties] Properties to set
         * @returns NetworkInfo instance
         */
        public static create(properties?: fk_app.INetworkInfo): fk_app.NetworkInfo;

        /**
         * Encodes the specified NetworkInfo message. Does not implicitly {@link fk_app.NetworkInfo.verify|verify} messages.
         * @param message NetworkInfo message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.INetworkInfo, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified NetworkInfo message, length delimited. Does not implicitly {@link fk_app.NetworkInfo.verify|verify} messages.
         * @param message NetworkInfo message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.INetworkInfo, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a NetworkInfo message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns NetworkInfo
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.NetworkInfo;

        /**
         * Decodes a NetworkInfo message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns NetworkInfo
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.NetworkInfo;

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
        public static fromObject(object: { [k: string]: any }): fk_app.NetworkInfo;

        /**
         * Creates a plain object from a NetworkInfo message. Also converts values to other types if specified.
         * @param message NetworkInfo
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.NetworkInfo, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this NetworkInfo to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a NetworkSettings. */
    interface INetworkSettings {

        /** NetworkSettings createAccessPoint */
        createAccessPoint?: number;

        /** NetworkSettings connected */
        connected?: fk_app.INetworkInfo;

        /** NetworkSettings macAddress */
        macAddress?: string;

        /** NetworkSettings modifying */
        modifying?: boolean;

        /** NetworkSettings networks */
        networks?: fk_app.INetworkInfo[];
    }

    /** Represents a NetworkSettings. */
    class NetworkSettings {

        /**
         * Constructs a new NetworkSettings.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.INetworkSettings);

        /** NetworkSettings createAccessPoint. */
        public createAccessPoint: number;

        /** NetworkSettings connected. */
        public connected?: (fk_app.INetworkInfo|null);

        /** NetworkSettings macAddress. */
        public macAddress: string;

        /** NetworkSettings modifying. */
        public modifying: boolean;

        /** NetworkSettings networks. */
        public networks: fk_app.INetworkInfo[];

        /**
         * Creates a new NetworkSettings instance using the specified properties.
         * @param [properties] Properties to set
         * @returns NetworkSettings instance
         */
        public static create(properties?: fk_app.INetworkSettings): fk_app.NetworkSettings;

        /**
         * Encodes the specified NetworkSettings message. Does not implicitly {@link fk_app.NetworkSettings.verify|verify} messages.
         * @param message NetworkSettings message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.INetworkSettings, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified NetworkSettings message, length delimited. Does not implicitly {@link fk_app.NetworkSettings.verify|verify} messages.
         * @param message NetworkSettings message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.INetworkSettings, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a NetworkSettings message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns NetworkSettings
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.NetworkSettings;

        /**
         * Decodes a NetworkSettings message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns NetworkSettings
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.NetworkSettings;

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
        public static fromObject(object: { [k: string]: any }): fk_app.NetworkSettings;

        /**
         * Creates a plain object from a NetworkSettings message. Also converts values to other types if specified.
         * @param message NetworkSettings
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.NetworkSettings, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this NetworkSettings to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a Firmware. */
    interface IFirmware {

        /** Firmware version */
        version?: string;

        /** Firmware build */
        build?: string;

        /** Firmware number */
        number?: string;

        /** Firmware timestamp */
        timestamp?: (number|Long);

        /** Firmware hash */
        hash?: string;

        /** Firmware logicalAddress */
        logicalAddress?: (number|Long);

        /** Firmware name */
        name?: string;
    }

    /** Represents a Firmware. */
    class Firmware {

        /**
         * Constructs a new Firmware.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IFirmware);

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

        /** Firmware logicalAddress. */
        public logicalAddress: (number|Long);

        /** Firmware name. */
        public name: string;

        /**
         * Creates a new Firmware instance using the specified properties.
         * @param [properties] Properties to set
         * @returns Firmware instance
         */
        public static create(properties?: fk_app.IFirmware): fk_app.Firmware;

        /**
         * Encodes the specified Firmware message. Does not implicitly {@link fk_app.Firmware.verify|verify} messages.
         * @param message Firmware message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IFirmware, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Firmware message, length delimited. Does not implicitly {@link fk_app.Firmware.verify|verify} messages.
         * @param message Firmware message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IFirmware, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a Firmware message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Firmware
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.Firmware;

        /**
         * Decodes a Firmware message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Firmware
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.Firmware;

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
        public static fromObject(object: { [k: string]: any }): fk_app.Firmware;

        /**
         * Creates a plain object from a Firmware message. Also converts values to other types if specified.
         * @param message Firmware
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.Firmware, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Firmware to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of an Identity. */
    interface IIdentity {

        /** Identity device */
        device?: string;

        /** Identity stream */
        stream?: string;

        /** Identity deviceId */
        deviceId?: Uint8Array;

        /** Identity firmware */
        firmware?: string;

        /** Identity build */
        build?: string;

        /** Identity number */
        number?: string;

        /** Identity name */
        name?: string;

        /** Identity generationId */
        generationId?: Uint8Array;
    }

    /** Represents an Identity. */
    class Identity {

        /**
         * Constructs a new Identity.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IIdentity);

        /** Identity device. */
        public device: string;

        /** Identity stream. */
        public stream: string;

        /** Identity deviceId. */
        public deviceId: Uint8Array;

        /** Identity firmware. */
        public firmware: string;

        /** Identity build. */
        public build: string;

        /** Identity number. */
        public number: string;

        /** Identity name. */
        public name: string;

        /** Identity generationId. */
        public generationId: Uint8Array;

        /**
         * Creates a new Identity instance using the specified properties.
         * @param [properties] Properties to set
         * @returns Identity instance
         */
        public static create(properties?: fk_app.IIdentity): fk_app.Identity;

        /**
         * Encodes the specified Identity message. Does not implicitly {@link fk_app.Identity.verify|verify} messages.
         * @param message Identity message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IIdentity, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Identity message, length delimited. Does not implicitly {@link fk_app.Identity.verify|verify} messages.
         * @param message Identity message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IIdentity, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes an Identity message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Identity
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.Identity;

        /**
         * Decodes an Identity message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Identity
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.Identity;

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
        public static fromObject(object: { [k: string]: any }): fk_app.Identity;

        /**
         * Creates a plain object from an Identity message. Also converts values to other types if specified.
         * @param message Identity
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.Identity, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Identity to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a ConfigureSensorQuery. */
    interface IConfigureSensorQuery {

        /** ConfigureSensorQuery id */
        id?: number;

        /** ConfigureSensorQuery frequency */
        frequency?: number;
    }

    /** Represents a ConfigureSensorQuery. */
    class ConfigureSensorQuery {

        /**
         * Constructs a new ConfigureSensorQuery.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IConfigureSensorQuery);

        /** ConfigureSensorQuery id. */
        public id: number;

        /** ConfigureSensorQuery frequency. */
        public frequency: number;

        /**
         * Creates a new ConfigureSensorQuery instance using the specified properties.
         * @param [properties] Properties to set
         * @returns ConfigureSensorQuery instance
         */
        public static create(properties?: fk_app.IConfigureSensorQuery): fk_app.ConfigureSensorQuery;

        /**
         * Encodes the specified ConfigureSensorQuery message. Does not implicitly {@link fk_app.ConfigureSensorQuery.verify|verify} messages.
         * @param message ConfigureSensorQuery message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IConfigureSensorQuery, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified ConfigureSensorQuery message, length delimited. Does not implicitly {@link fk_app.ConfigureSensorQuery.verify|verify} messages.
         * @param message ConfigureSensorQuery message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IConfigureSensorQuery, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a ConfigureSensorQuery message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns ConfigureSensorQuery
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.ConfigureSensorQuery;

        /**
         * Decodes a ConfigureSensorQuery message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns ConfigureSensorQuery
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.ConfigureSensorQuery;

        /**
         * Verifies a ConfigureSensorQuery message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a ConfigureSensorQuery message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns ConfigureSensorQuery
         */
        public static fromObject(object: { [k: string]: any }): fk_app.ConfigureSensorQuery;

        /**
         * Creates a plain object from a ConfigureSensorQuery message. Also converts values to other types if specified.
         * @param message ConfigureSensorQuery
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.ConfigureSensorQuery, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this ConfigureSensorQuery to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a LiveDataPoll. */
    interface ILiveDataPoll {

        /** LiveDataPoll interval */
        interval?: number;
    }

    /** Represents a LiveDataPoll. */
    class LiveDataPoll {

        /**
         * Constructs a new LiveDataPoll.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.ILiveDataPoll);

        /** LiveDataPoll interval. */
        public interval: number;

        /**
         * Creates a new LiveDataPoll instance using the specified properties.
         * @param [properties] Properties to set
         * @returns LiveDataPoll instance
         */
        public static create(properties?: fk_app.ILiveDataPoll): fk_app.LiveDataPoll;

        /**
         * Encodes the specified LiveDataPoll message. Does not implicitly {@link fk_app.LiveDataPoll.verify|verify} messages.
         * @param message LiveDataPoll message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.ILiveDataPoll, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified LiveDataPoll message, length delimited. Does not implicitly {@link fk_app.LiveDataPoll.verify|verify} messages.
         * @param message LiveDataPoll message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.ILiveDataPoll, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a LiveDataPoll message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns LiveDataPoll
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.LiveDataPoll;

        /**
         * Decodes a LiveDataPoll message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns LiveDataPoll
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.LiveDataPoll;

        /**
         * Verifies a LiveDataPoll message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a LiveDataPoll message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns LiveDataPoll
         */
        public static fromObject(object: { [k: string]: any }): fk_app.LiveDataPoll;

        /**
         * Creates a plain object from a LiveDataPoll message. Also converts values to other types if specified.
         * @param message LiveDataPoll
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.LiveDataPoll, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this LiveDataPoll to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a LiveDataSample. */
    interface ILiveDataSample {

        /** LiveDataSample sensor */
        sensor?: number;

        /** LiveDataSample time */
        time?: (number|Long);

        /** LiveDataSample value */
        value?: number;
    }

    /** Represents a LiveDataSample. */
    class LiveDataSample {

        /**
         * Constructs a new LiveDataSample.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.ILiveDataSample);

        /** LiveDataSample sensor. */
        public sensor: number;

        /** LiveDataSample time. */
        public time: (number|Long);

        /** LiveDataSample value. */
        public value: number;

        /**
         * Creates a new LiveDataSample instance using the specified properties.
         * @param [properties] Properties to set
         * @returns LiveDataSample instance
         */
        public static create(properties?: fk_app.ILiveDataSample): fk_app.LiveDataSample;

        /**
         * Encodes the specified LiveDataSample message. Does not implicitly {@link fk_app.LiveDataSample.verify|verify} messages.
         * @param message LiveDataSample message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.ILiveDataSample, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified LiveDataSample message, length delimited. Does not implicitly {@link fk_app.LiveDataSample.verify|verify} messages.
         * @param message LiveDataSample message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.ILiveDataSample, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a LiveDataSample message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns LiveDataSample
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.LiveDataSample;

        /**
         * Decodes a LiveDataSample message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns LiveDataSample
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.LiveDataSample;

        /**
         * Verifies a LiveDataSample message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a LiveDataSample message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns LiveDataSample
         */
        public static fromObject(object: { [k: string]: any }): fk_app.LiveDataSample;

        /**
         * Creates a plain object from a LiveDataSample message. Also converts values to other types if specified.
         * @param message LiveDataSample
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.LiveDataSample, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this LiveDataSample to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a LiveData. */
    interface ILiveData {

        /** LiveData samples */
        samples?: fk_app.ILiveDataSample[];
    }

    /** Represents a LiveData. */
    class LiveData {

        /**
         * Constructs a new LiveData.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.ILiveData);

        /** LiveData samples. */
        public samples: fk_app.ILiveDataSample[];

        /**
         * Creates a new LiveData instance using the specified properties.
         * @param [properties] Properties to set
         * @returns LiveData instance
         */
        public static create(properties?: fk_app.ILiveData): fk_app.LiveData;

        /**
         * Encodes the specified LiveData message. Does not implicitly {@link fk_app.LiveData.verify|verify} messages.
         * @param message LiveData message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.ILiveData, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified LiveData message, length delimited. Does not implicitly {@link fk_app.LiveData.verify|verify} messages.
         * @param message LiveData message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.ILiveData, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a LiveData message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns LiveData
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.LiveData;

        /**
         * Decodes a LiveData message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns LiveData
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.LiveData;

        /**
         * Verifies a LiveData message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a LiveData message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns LiveData
         */
        public static fromObject(object: { [k: string]: any }): fk_app.LiveData;

        /**
         * Creates a plain object from a LiveData message. Also converts values to other types if specified.
         * @param message LiveData
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.LiveData, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this LiveData to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a File. */
    interface IFile {

        /** File id */
        id?: number;

        /** File time */
        time?: (number|Long);

        /** File size */
        size?: (number|Long);

        /** File version */
        version?: number;

        /** File name */
        name?: string;

        /** File maximum */
        maximum?: (number|Long);
    }

    /** Represents a File. */
    class File {

        /**
         * Constructs a new File.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IFile);

        /** File id. */
        public id: number;

        /** File time. */
        public time: (number|Long);

        /** File size. */
        public size: (number|Long);

        /** File version. */
        public version: number;

        /** File name. */
        public name: string;

        /** File maximum. */
        public maximum: (number|Long);

        /**
         * Creates a new File instance using the specified properties.
         * @param [properties] Properties to set
         * @returns File instance
         */
        public static create(properties?: fk_app.IFile): fk_app.File;

        /**
         * Encodes the specified File message. Does not implicitly {@link fk_app.File.verify|verify} messages.
         * @param message File message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IFile, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified File message, length delimited. Does not implicitly {@link fk_app.File.verify|verify} messages.
         * @param message File message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IFile, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a File message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns File
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.File;

        /**
         * Decodes a File message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns File
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.File;

        /**
         * Verifies a File message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a File message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns File
         */
        public static fromObject(object: { [k: string]: any }): fk_app.File;

        /**
         * Creates a plain object from a File message. Also converts values to other types if specified.
         * @param message File
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.File, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this File to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a Files. */
    interface IFiles {

        /** Files files */
        files?: fk_app.IFile[];
    }

    /** Represents a Files. */
    class Files {

        /**
         * Constructs a new Files.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IFiles);

        /** Files files. */
        public files: fk_app.IFile[];

        /**
         * Creates a new Files instance using the specified properties.
         * @param [properties] Properties to set
         * @returns Files instance
         */
        public static create(properties?: fk_app.IFiles): fk_app.Files;

        /**
         * Encodes the specified Files message. Does not implicitly {@link fk_app.Files.verify|verify} messages.
         * @param message Files message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IFiles, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Files message, length delimited. Does not implicitly {@link fk_app.Files.verify|verify} messages.
         * @param message Files message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IFiles, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a Files message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Files
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.Files;

        /**
         * Decodes a Files message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Files
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.Files;

        /**
         * Verifies a Files message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a Files message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns Files
         */
        public static fromObject(object: { [k: string]: any }): fk_app.Files;

        /**
         * Creates a plain object from a Files message. Also converts values to other types if specified.
         * @param message Files
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.Files, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Files to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a DownloadFile. */
    interface IDownloadFile {

        /** DownloadFile id */
        id?: number;

        /** DownloadFile offset */
        offset?: number;

        /** DownloadFile length */
        length?: number;

        /** DownloadFile flags */
        flags?: number;
    }

    /** Represents a DownloadFile. */
    class DownloadFile {

        /**
         * Constructs a new DownloadFile.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IDownloadFile);

        /** DownloadFile id. */
        public id: number;

        /** DownloadFile offset. */
        public offset: number;

        /** DownloadFile length. */
        public length: number;

        /** DownloadFile flags. */
        public flags: number;

        /**
         * Creates a new DownloadFile instance using the specified properties.
         * @param [properties] Properties to set
         * @returns DownloadFile instance
         */
        public static create(properties?: fk_app.IDownloadFile): fk_app.DownloadFile;

        /**
         * Encodes the specified DownloadFile message. Does not implicitly {@link fk_app.DownloadFile.verify|verify} messages.
         * @param message DownloadFile message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IDownloadFile, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified DownloadFile message, length delimited. Does not implicitly {@link fk_app.DownloadFile.verify|verify} messages.
         * @param message DownloadFile message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IDownloadFile, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a DownloadFile message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns DownloadFile
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.DownloadFile;

        /**
         * Decodes a DownloadFile message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns DownloadFile
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.DownloadFile;

        /**
         * Verifies a DownloadFile message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a DownloadFile message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns DownloadFile
         */
        public static fromObject(object: { [k: string]: any }): fk_app.DownloadFile;

        /**
         * Creates a plain object from a DownloadFile message. Also converts values to other types if specified.
         * @param message DownloadFile
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.DownloadFile, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this DownloadFile to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of an EraseFile. */
    interface IEraseFile {

        /** EraseFile id */
        id?: number;
    }

    /** Represents an EraseFile. */
    class EraseFile {

        /**
         * Constructs a new EraseFile.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IEraseFile);

        /** EraseFile id. */
        public id: number;

        /**
         * Creates a new EraseFile instance using the specified properties.
         * @param [properties] Properties to set
         * @returns EraseFile instance
         */
        public static create(properties?: fk_app.IEraseFile): fk_app.EraseFile;

        /**
         * Encodes the specified EraseFile message. Does not implicitly {@link fk_app.EraseFile.verify|verify} messages.
         * @param message EraseFile message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IEraseFile, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified EraseFile message, length delimited. Does not implicitly {@link fk_app.EraseFile.verify|verify} messages.
         * @param message EraseFile message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IEraseFile, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes an EraseFile message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns EraseFile
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.EraseFile;

        /**
         * Decodes an EraseFile message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns EraseFile
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.EraseFile;

        /**
         * Verifies an EraseFile message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates an EraseFile message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns EraseFile
         */
        public static fromObject(object: { [k: string]: any }): fk_app.EraseFile;

        /**
         * Creates a plain object from an EraseFile message. Also converts values to other types if specified.
         * @param message EraseFile
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.EraseFile, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this EraseFile to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a FileData. */
    interface IFileData {

        /** FileData offset */
        offset?: number;

        /** FileData data */
        data?: Uint8Array;

        /** FileData size */
        size?: number;

        /** FileData hash */
        hash?: number;

        /** FileData version */
        version?: number;

        /** FileData id */
        id?: number;
    }

    /** Represents a FileData. */
    class FileData {

        /**
         * Constructs a new FileData.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IFileData);

        /** FileData offset. */
        public offset: number;

        /** FileData data. */
        public data: Uint8Array;

        /** FileData size. */
        public size: number;

        /** FileData hash. */
        public hash: number;

        /** FileData version. */
        public version: number;

        /** FileData id. */
        public id: number;

        /**
         * Creates a new FileData instance using the specified properties.
         * @param [properties] Properties to set
         * @returns FileData instance
         */
        public static create(properties?: fk_app.IFileData): fk_app.FileData;

        /**
         * Encodes the specified FileData message. Does not implicitly {@link fk_app.FileData.verify|verify} messages.
         * @param message FileData message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IFileData, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified FileData message, length delimited. Does not implicitly {@link fk_app.FileData.verify|verify} messages.
         * @param message FileData message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IFileData, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a FileData message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns FileData
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.FileData;

        /**
         * Decodes a FileData message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns FileData
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.FileData;

        /**
         * Verifies a FileData message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a FileData message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns FileData
         */
        public static fromObject(object: { [k: string]: any }): fk_app.FileData;

        /**
         * Creates a plain object from a FileData message. Also converts values to other types if specified.
         * @param message FileData
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.FileData, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this FileData to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a DeviceStatus. */
    interface IDeviceStatus {

        /** DeviceStatus uptime */
        uptime?: number;

        /** DeviceStatus batteryPercentage */
        batteryPercentage?: number;

        /** DeviceStatus batteryVoltage */
        batteryVoltage?: number;

        /** DeviceStatus gpsHasFix */
        gpsHasFix?: number;

        /** DeviceStatus gpsSatellites */
        gpsSatellites?: number;
    }

    /** Represents a DeviceStatus. */
    class DeviceStatus {

        /**
         * Constructs a new DeviceStatus.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IDeviceStatus);

        /** DeviceStatus uptime. */
        public uptime: number;

        /** DeviceStatus batteryPercentage. */
        public batteryPercentage: number;

        /** DeviceStatus batteryVoltage. */
        public batteryVoltage: number;

        /** DeviceStatus gpsHasFix. */
        public gpsHasFix: number;

        /** DeviceStatus gpsSatellites. */
        public gpsSatellites: number;

        /**
         * Creates a new DeviceStatus instance using the specified properties.
         * @param [properties] Properties to set
         * @returns DeviceStatus instance
         */
        public static create(properties?: fk_app.IDeviceStatus): fk_app.DeviceStatus;

        /**
         * Encodes the specified DeviceStatus message. Does not implicitly {@link fk_app.DeviceStatus.verify|verify} messages.
         * @param message DeviceStatus message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IDeviceStatus, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified DeviceStatus message, length delimited. Does not implicitly {@link fk_app.DeviceStatus.verify|verify} messages.
         * @param message DeviceStatus message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IDeviceStatus, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a DeviceStatus message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns DeviceStatus
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.DeviceStatus;

        /**
         * Decodes a DeviceStatus message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns DeviceStatus
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.DeviceStatus;

        /**
         * Verifies a DeviceStatus message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a DeviceStatus message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns DeviceStatus
         */
        public static fromObject(object: { [k: string]: any }): fk_app.DeviceStatus;

        /**
         * Creates a plain object from a DeviceStatus message. Also converts values to other types if specified.
         * @param message DeviceStatus
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.DeviceStatus, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this DeviceStatus to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a QueryModule. */
    interface IQueryModule {

        /** QueryModule id */
        id?: number;

        /** QueryModule address */
        address?: number;

        /** QueryModule message */
        message?: Uint8Array;
    }

    /** Represents a QueryModule. */
    class QueryModule {

        /**
         * Constructs a new QueryModule.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IQueryModule);

        /** QueryModule id. */
        public id: number;

        /** QueryModule address. */
        public address: number;

        /** QueryModule message. */
        public message: Uint8Array;

        /**
         * Creates a new QueryModule instance using the specified properties.
         * @param [properties] Properties to set
         * @returns QueryModule instance
         */
        public static create(properties?: fk_app.IQueryModule): fk_app.QueryModule;

        /**
         * Encodes the specified QueryModule message. Does not implicitly {@link fk_app.QueryModule.verify|verify} messages.
         * @param message QueryModule message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IQueryModule, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified QueryModule message, length delimited. Does not implicitly {@link fk_app.QueryModule.verify|verify} messages.
         * @param message QueryModule message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IQueryModule, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a QueryModule message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns QueryModule
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.QueryModule;

        /**
         * Decodes a QueryModule message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns QueryModule
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.QueryModule;

        /**
         * Verifies a QueryModule message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a QueryModule message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns QueryModule
         */
        public static fromObject(object: { [k: string]: any }): fk_app.QueryModule;

        /**
         * Creates a plain object from a QueryModule message. Also converts values to other types if specified.
         * @param message QueryModule
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.QueryModule, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this QueryModule to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a ModuleReply. */
    interface IModuleReply {

        /** ModuleReply id */
        id?: number;

        /** ModuleReply address */
        address?: number;

        /** ModuleReply message */
        message?: Uint8Array;
    }

    /** Represents a ModuleReply. */
    class ModuleReply {

        /**
         * Constructs a new ModuleReply.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IModuleReply);

        /** ModuleReply id. */
        public id: number;

        /** ModuleReply address. */
        public address: number;

        /** ModuleReply message. */
        public message: Uint8Array;

        /**
         * Creates a new ModuleReply instance using the specified properties.
         * @param [properties] Properties to set
         * @returns ModuleReply instance
         */
        public static create(properties?: fk_app.IModuleReply): fk_app.ModuleReply;

        /**
         * Encodes the specified ModuleReply message. Does not implicitly {@link fk_app.ModuleReply.verify|verify} messages.
         * @param message ModuleReply message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IModuleReply, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified ModuleReply message, length delimited. Does not implicitly {@link fk_app.ModuleReply.verify|verify} messages.
         * @param message ModuleReply message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IModuleReply, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a ModuleReply message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns ModuleReply
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.ModuleReply;

        /**
         * Decodes a ModuleReply message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns ModuleReply
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.ModuleReply;

        /**
         * Verifies a ModuleReply message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a ModuleReply message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns ModuleReply
         */
        public static fromObject(object: { [k: string]: any }): fk_app.ModuleReply;

        /**
         * Creates a plain object from a ModuleReply message. Also converts values to other types if specified.
         * @param message ModuleReply
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.ModuleReply, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this ModuleReply to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a WireMessageQuery. */
    interface IWireMessageQuery {

        /** WireMessageQuery type */
        type?: fk_app.QueryType;

        /** WireMessageQuery queryCapabilities */
        queryCapabilities?: fk_app.IQueryCapabilities;

        /** WireMessageQuery configureSensor */
        configureSensor?: fk_app.IConfigureSensorQuery;

        /** WireMessageQuery liveDataPoll */
        liveDataPoll?: fk_app.ILiveDataPoll;

        /** WireMessageQuery downloadFile */
        downloadFile?: fk_app.IDownloadFile;

        /** WireMessageQuery eraseFile */
        eraseFile?: fk_app.IEraseFile;

        /** WireMessageQuery networkSettings */
        networkSettings?: fk_app.INetworkSettings;

        /** WireMessageQuery identity */
        identity?: fk_app.IIdentity;

        /** WireMessageQuery module */
        module?: fk_app.IQueryModule;
    }

    /** Represents a WireMessageQuery. */
    class WireMessageQuery {

        /**
         * Constructs a new WireMessageQuery.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IWireMessageQuery);

        /** WireMessageQuery type. */
        public type: fk_app.QueryType;

        /** WireMessageQuery queryCapabilities. */
        public queryCapabilities?: (fk_app.IQueryCapabilities|null);

        /** WireMessageQuery configureSensor. */
        public configureSensor?: (fk_app.IConfigureSensorQuery|null);

        /** WireMessageQuery liveDataPoll. */
        public liveDataPoll?: (fk_app.ILiveDataPoll|null);

        /** WireMessageQuery downloadFile. */
        public downloadFile?: (fk_app.IDownloadFile|null);

        /** WireMessageQuery eraseFile. */
        public eraseFile?: (fk_app.IEraseFile|null);

        /** WireMessageQuery networkSettings. */
        public networkSettings?: (fk_app.INetworkSettings|null);

        /** WireMessageQuery identity. */
        public identity?: (fk_app.IIdentity|null);

        /** WireMessageQuery module. */
        public module?: (fk_app.IQueryModule|null);

        /**
         * Creates a new WireMessageQuery instance using the specified properties.
         * @param [properties] Properties to set
         * @returns WireMessageQuery instance
         */
        public static create(properties?: fk_app.IWireMessageQuery): fk_app.WireMessageQuery;

        /**
         * Encodes the specified WireMessageQuery message. Does not implicitly {@link fk_app.WireMessageQuery.verify|verify} messages.
         * @param message WireMessageQuery message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IWireMessageQuery, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified WireMessageQuery message, length delimited. Does not implicitly {@link fk_app.WireMessageQuery.verify|verify} messages.
         * @param message WireMessageQuery message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IWireMessageQuery, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a WireMessageQuery message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns WireMessageQuery
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.WireMessageQuery;

        /**
         * Decodes a WireMessageQuery message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns WireMessageQuery
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.WireMessageQuery;

        /**
         * Verifies a WireMessageQuery message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a WireMessageQuery message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns WireMessageQuery
         */
        public static fromObject(object: { [k: string]: any }): fk_app.WireMessageQuery;

        /**
         * Creates a plain object from a WireMessageQuery message. Also converts values to other types if specified.
         * @param message WireMessageQuery
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.WireMessageQuery, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this WireMessageQuery to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of an Error. */
    interface IError {

        /** Error message */
        message?: string;

        /** Error delay */
        delay?: number;
    }

    /** Represents an Error. */
    class Error {

        /**
         * Constructs a new Error.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IError);

        /** Error message. */
        public message: string;

        /** Error delay. */
        public delay: number;

        /**
         * Creates a new Error instance using the specified properties.
         * @param [properties] Properties to set
         * @returns Error instance
         */
        public static create(properties?: fk_app.IError): fk_app.Error;

        /**
         * Encodes the specified Error message. Does not implicitly {@link fk_app.Error.verify|verify} messages.
         * @param message Error message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IError, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Error message, length delimited. Does not implicitly {@link fk_app.Error.verify|verify} messages.
         * @param message Error message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IError, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes an Error message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Error
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.Error;

        /**
         * Decodes an Error message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Error
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.Error;

        /**
         * Verifies an Error message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates an Error message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns Error
         */
        public static fromObject(object: { [k: string]: any }): fk_app.Error;

        /**
         * Creates a plain object from an Error message. Also converts values to other types if specified.
         * @param message Error
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.Error, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Error to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a WireMessageReply. */
    interface IWireMessageReply {

        /** WireMessageReply type */
        type?: fk_app.ReplyType;

        /** WireMessageReply errors */
        errors?: fk_app.IError[];

        /** WireMessageReply capabilities */
        capabilities?: fk_app.ICapabilities;

        /** WireMessageReply liveData */
        liveData?: fk_app.ILiveData;

        /** WireMessageReply files */
        files?: fk_app.IFiles;

        /** WireMessageReply fileData */
        fileData?: fk_app.IFileData;

        /** WireMessageReply networkSettings */
        networkSettings?: fk_app.INetworkSettings;

        /** WireMessageReply identity */
        identity?: fk_app.IIdentity;

        /** WireMessageReply status */
        status?: fk_app.IDeviceStatus;

        /** WireMessageReply module */
        module?: fk_app.IModuleReply;
    }

    /** Represents a WireMessageReply. */
    class WireMessageReply {

        /**
         * Constructs a new WireMessageReply.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IWireMessageReply);

        /** WireMessageReply type. */
        public type: fk_app.ReplyType;

        /** WireMessageReply errors. */
        public errors: fk_app.IError[];

        /** WireMessageReply capabilities. */
        public capabilities?: (fk_app.ICapabilities|null);

        /** WireMessageReply liveData. */
        public liveData?: (fk_app.ILiveData|null);

        /** WireMessageReply files. */
        public files?: (fk_app.IFiles|null);

        /** WireMessageReply fileData. */
        public fileData?: (fk_app.IFileData|null);

        /** WireMessageReply networkSettings. */
        public networkSettings?: (fk_app.INetworkSettings|null);

        /** WireMessageReply identity. */
        public identity?: (fk_app.IIdentity|null);

        /** WireMessageReply status. */
        public status?: (fk_app.IDeviceStatus|null);

        /** WireMessageReply module. */
        public module?: (fk_app.IModuleReply|null);

        /**
         * Creates a new WireMessageReply instance using the specified properties.
         * @param [properties] Properties to set
         * @returns WireMessageReply instance
         */
        public static create(properties?: fk_app.IWireMessageReply): fk_app.WireMessageReply;

        /**
         * Encodes the specified WireMessageReply message. Does not implicitly {@link fk_app.WireMessageReply.verify|verify} messages.
         * @param message WireMessageReply message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IWireMessageReply, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified WireMessageReply message, length delimited. Does not implicitly {@link fk_app.WireMessageReply.verify|verify} messages.
         * @param message WireMessageReply message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IWireMessageReply, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a WireMessageReply message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns WireMessageReply
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.WireMessageReply;

        /**
         * Decodes a WireMessageReply message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns WireMessageReply
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.WireMessageReply;

        /**
         * Verifies a WireMessageReply message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a WireMessageReply message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns WireMessageReply
         */
        public static fromObject(object: { [k: string]: any }): fk_app.WireMessageReply;

        /**
         * Creates a plain object from a WireMessageReply message. Also converts values to other types if specified.
         * @param message WireMessageReply
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.WireMessageReply, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this WireMessageReply to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of an Interval. */
    interface IInterval {

        /** Interval start */
        start?: (number|Long);

        /** Interval end */
        end?: (number|Long);

        /** Interval interval */
        interval?: number;
    }

    /** Represents an Interval. */
    class Interval {

        /**
         * Constructs a new Interval.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IInterval);

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
        public static create(properties?: fk_app.IInterval): fk_app.Interval;

        /**
         * Encodes the specified Interval message. Does not implicitly {@link fk_app.Interval.verify|verify} messages.
         * @param message Interval message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IInterval, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Interval message, length delimited. Does not implicitly {@link fk_app.Interval.verify|verify} messages.
         * @param message Interval message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IInterval, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes an Interval message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Interval
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.Interval;

        /**
         * Decodes an Interval message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Interval
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.Interval;

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
        public static fromObject(object: { [k: string]: any }): fk_app.Interval;

        /**
         * Creates a plain object from an Interval message. Also converts values to other types if specified.
         * @param message Interval
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.Interval, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Interval to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a Schedule. */
    interface ISchedule {

        /** Schedule cron */
        cron?: Uint8Array;

        /** Schedule interval */
        interval?: number;

        /** Schedule repeated */
        repeated?: number;

        /** Schedule duration */
        duration?: number;

        /** Schedule jitter */
        jitter?: number;

        /** Schedule intervals */
        intervals?: fk_app.IInterval[];
    }

    /** Represents a Schedule. */
    class Schedule {

        /**
         * Constructs a new Schedule.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.ISchedule);

        /** Schedule cron. */
        public cron: Uint8Array;

        /** Schedule interval. */
        public interval: number;

        /** Schedule repeated. */
        public repeated: number;

        /** Schedule duration. */
        public duration: number;

        /** Schedule jitter. */
        public jitter: number;

        /** Schedule intervals. */
        public intervals: fk_app.IInterval[];

        /**
         * Creates a new Schedule instance using the specified properties.
         * @param [properties] Properties to set
         * @returns Schedule instance
         */
        public static create(properties?: fk_app.ISchedule): fk_app.Schedule;

        /**
         * Encodes the specified Schedule message. Does not implicitly {@link fk_app.Schedule.verify|verify} messages.
         * @param message Schedule message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.ISchedule, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Schedule message, length delimited. Does not implicitly {@link fk_app.Schedule.verify|verify} messages.
         * @param message Schedule message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.ISchedule, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a Schedule message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Schedule
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.Schedule;

        /**
         * Decodes a Schedule message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Schedule
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.Schedule;

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
        public static fromObject(object: { [k: string]: any }): fk_app.Schedule;

        /**
         * Creates a plain object from a Schedule message. Also converts values to other types if specified.
         * @param message Schedule
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.Schedule, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Schedule to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a Schedules. */
    interface ISchedules {

        /** Schedules modifying */
        modifying?: boolean;

        /** Schedules readings */
        readings?: fk_app.ISchedule;

        /** Schedules lora */
        lora?: fk_app.ISchedule;

        /** Schedules network */
        network?: fk_app.ISchedule;

        /** Schedules gps */
        gps?: fk_app.ISchedule;
    }

    /** Represents a Schedules. */
    class Schedules {

        /**
         * Constructs a new Schedules.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.ISchedules);

        /** Schedules modifying. */
        public modifying: boolean;

        /** Schedules readings. */
        public readings?: (fk_app.ISchedule|null);

        /** Schedules lora. */
        public lora?: (fk_app.ISchedule|null);

        /** Schedules network. */
        public network?: (fk_app.ISchedule|null);

        /** Schedules gps. */
        public gps?: (fk_app.ISchedule|null);

        /**
         * Creates a new Schedules instance using the specified properties.
         * @param [properties] Properties to set
         * @returns Schedules instance
         */
        public static create(properties?: fk_app.ISchedules): fk_app.Schedules;

        /**
         * Encodes the specified Schedules message. Does not implicitly {@link fk_app.Schedules.verify|verify} messages.
         * @param message Schedules message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.ISchedules, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Schedules message, length delimited. Does not implicitly {@link fk_app.Schedules.verify|verify} messages.
         * @param message Schedules message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.ISchedules, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a Schedules message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Schedules
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.Schedules;

        /**
         * Decodes a Schedules message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Schedules
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.Schedules;

        /**
         * Verifies a Schedules message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a Schedules message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns Schedules
         */
        public static fromObject(object: { [k: string]: any }): fk_app.Schedules;

        /**
         * Creates a plain object from a Schedules message. Also converts values to other types if specified.
         * @param message Schedules
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.Schedules, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Schedules to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a HardwareStatus. */
    interface IHardwareStatus {
    }

    /** Represents a HardwareStatus. */
    class HardwareStatus {

        /**
         * Constructs a new HardwareStatus.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IHardwareStatus);

        /**
         * Creates a new HardwareStatus instance using the specified properties.
         * @param [properties] Properties to set
         * @returns HardwareStatus instance
         */
        public static create(properties?: fk_app.IHardwareStatus): fk_app.HardwareStatus;

        /**
         * Encodes the specified HardwareStatus message. Does not implicitly {@link fk_app.HardwareStatus.verify|verify} messages.
         * @param message HardwareStatus message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IHardwareStatus, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified HardwareStatus message, length delimited. Does not implicitly {@link fk_app.HardwareStatus.verify|verify} messages.
         * @param message HardwareStatus message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IHardwareStatus, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a HardwareStatus message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns HardwareStatus
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.HardwareStatus;

        /**
         * Decodes a HardwareStatus message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns HardwareStatus
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.HardwareStatus;

        /**
         * Verifies a HardwareStatus message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a HardwareStatus message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns HardwareStatus
         */
        public static fromObject(object: { [k: string]: any }): fk_app.HardwareStatus;

        /**
         * Creates a plain object from a HardwareStatus message. Also converts values to other types if specified.
         * @param message HardwareStatus
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.HardwareStatus, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this HardwareStatus to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a GpsStatus. */
    interface IGpsStatus {

        /** GpsStatus enabled */
        enabled?: number;

        /** GpsStatus fix */
        fix?: number;

        /** GpsStatus time */
        time?: (number|Long);

        /** GpsStatus satellites */
        satellites?: number;

        /** GpsStatus longitude */
        longitude?: number;

        /** GpsStatus latitude */
        latitude?: number;

        /** GpsStatus altitude */
        altitude?: number;
    }

    /** Represents a GpsStatus. */
    class GpsStatus {

        /**
         * Constructs a new GpsStatus.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IGpsStatus);

        /** GpsStatus enabled. */
        public enabled: number;

        /** GpsStatus fix. */
        public fix: number;

        /** GpsStatus time. */
        public time: (number|Long);

        /** GpsStatus satellites. */
        public satellites: number;

        /** GpsStatus longitude. */
        public longitude: number;

        /** GpsStatus latitude. */
        public latitude: number;

        /** GpsStatus altitude. */
        public altitude: number;

        /**
         * Creates a new GpsStatus instance using the specified properties.
         * @param [properties] Properties to set
         * @returns GpsStatus instance
         */
        public static create(properties?: fk_app.IGpsStatus): fk_app.GpsStatus;

        /**
         * Encodes the specified GpsStatus message. Does not implicitly {@link fk_app.GpsStatus.verify|verify} messages.
         * @param message GpsStatus message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IGpsStatus, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified GpsStatus message, length delimited. Does not implicitly {@link fk_app.GpsStatus.verify|verify} messages.
         * @param message GpsStatus message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IGpsStatus, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a GpsStatus message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns GpsStatus
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.GpsStatus;

        /**
         * Decodes a GpsStatus message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns GpsStatus
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.GpsStatus;

        /**
         * Verifies a GpsStatus message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a GpsStatus message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns GpsStatus
         */
        public static fromObject(object: { [k: string]: any }): fk_app.GpsStatus;

        /**
         * Creates a plain object from a GpsStatus message. Also converts values to other types if specified.
         * @param message GpsStatus
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.GpsStatus, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this GpsStatus to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a MemoryStatus. */
    interface IMemoryStatus {

        /** MemoryStatus sramAvailable */
        sramAvailable?: number;

        /** MemoryStatus programFlashAvailable */
        programFlashAvailable?: number;

        /** MemoryStatus extendedMemoryAvailable */
        extendedMemoryAvailable?: number;

        /** MemoryStatus dataMemoryInstalled */
        dataMemoryInstalled?: number;

        /** MemoryStatus dataMemoryUsed */
        dataMemoryUsed?: number;

        /** MemoryStatus dataMemoryConsumption */
        dataMemoryConsumption?: number;

        /** MemoryStatus firmware */
        firmware?: fk_app.IFirmware[];
    }

    /** Represents a MemoryStatus. */
    class MemoryStatus {

        /**
         * Constructs a new MemoryStatus.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IMemoryStatus);

        /** MemoryStatus sramAvailable. */
        public sramAvailable: number;

        /** MemoryStatus programFlashAvailable. */
        public programFlashAvailable: number;

        /** MemoryStatus extendedMemoryAvailable. */
        public extendedMemoryAvailable: number;

        /** MemoryStatus dataMemoryInstalled. */
        public dataMemoryInstalled: number;

        /** MemoryStatus dataMemoryUsed. */
        public dataMemoryUsed: number;

        /** MemoryStatus dataMemoryConsumption. */
        public dataMemoryConsumption: number;

        /** MemoryStatus firmware. */
        public firmware: fk_app.IFirmware[];

        /**
         * Creates a new MemoryStatus instance using the specified properties.
         * @param [properties] Properties to set
         * @returns MemoryStatus instance
         */
        public static create(properties?: fk_app.IMemoryStatus): fk_app.MemoryStatus;

        /**
         * Encodes the specified MemoryStatus message. Does not implicitly {@link fk_app.MemoryStatus.verify|verify} messages.
         * @param message MemoryStatus message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IMemoryStatus, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified MemoryStatus message, length delimited. Does not implicitly {@link fk_app.MemoryStatus.verify|verify} messages.
         * @param message MemoryStatus message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IMemoryStatus, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a MemoryStatus message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns MemoryStatus
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.MemoryStatus;

        /**
         * Decodes a MemoryStatus message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns MemoryStatus
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.MemoryStatus;

        /**
         * Verifies a MemoryStatus message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a MemoryStatus message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns MemoryStatus
         */
        public static fromObject(object: { [k: string]: any }): fk_app.MemoryStatus;

        /**
         * Creates a plain object from a MemoryStatus message. Also converts values to other types if specified.
         * @param message MemoryStatus
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.MemoryStatus, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this MemoryStatus to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a BatteryStatus. */
    interface IBatteryStatus {

        /** BatteryStatus voltage */
        voltage?: number;

        /** BatteryStatus percentage */
        percentage?: number;
    }

    /** Represents a BatteryStatus. */
    class BatteryStatus {

        /**
         * Constructs a new BatteryStatus.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IBatteryStatus);

        /** BatteryStatus voltage. */
        public voltage: number;

        /** BatteryStatus percentage. */
        public percentage: number;

        /**
         * Creates a new BatteryStatus instance using the specified properties.
         * @param [properties] Properties to set
         * @returns BatteryStatus instance
         */
        public static create(properties?: fk_app.IBatteryStatus): fk_app.BatteryStatus;

        /**
         * Encodes the specified BatteryStatus message. Does not implicitly {@link fk_app.BatteryStatus.verify|verify} messages.
         * @param message BatteryStatus message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IBatteryStatus, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified BatteryStatus message, length delimited. Does not implicitly {@link fk_app.BatteryStatus.verify|verify} messages.
         * @param message BatteryStatus message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IBatteryStatus, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a BatteryStatus message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns BatteryStatus
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.BatteryStatus;

        /**
         * Decodes a BatteryStatus message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns BatteryStatus
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.BatteryStatus;

        /**
         * Verifies a BatteryStatus message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a BatteryStatus message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns BatteryStatus
         */
        public static fromObject(object: { [k: string]: any }): fk_app.BatteryStatus;

        /**
         * Creates a plain object from a BatteryStatus message. Also converts values to other types if specified.
         * @param message BatteryStatus
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.BatteryStatus, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this BatteryStatus to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a SolarStatus. */
    interface ISolarStatus {

        /** SolarStatus voltage */
        voltage?: number;
    }

    /** Represents a SolarStatus. */
    class SolarStatus {

        /**
         * Constructs a new SolarStatus.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.ISolarStatus);

        /** SolarStatus voltage. */
        public voltage: number;

        /**
         * Creates a new SolarStatus instance using the specified properties.
         * @param [properties] Properties to set
         * @returns SolarStatus instance
         */
        public static create(properties?: fk_app.ISolarStatus): fk_app.SolarStatus;

        /**
         * Encodes the specified SolarStatus message. Does not implicitly {@link fk_app.SolarStatus.verify|verify} messages.
         * @param message SolarStatus message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.ISolarStatus, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified SolarStatus message, length delimited. Does not implicitly {@link fk_app.SolarStatus.verify|verify} messages.
         * @param message SolarStatus message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.ISolarStatus, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a SolarStatus message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns SolarStatus
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.SolarStatus;

        /**
         * Decodes a SolarStatus message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns SolarStatus
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.SolarStatus;

        /**
         * Verifies a SolarStatus message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a SolarStatus message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns SolarStatus
         */
        public static fromObject(object: { [k: string]: any }): fk_app.SolarStatus;

        /**
         * Creates a plain object from a SolarStatus message. Also converts values to other types if specified.
         * @param message SolarStatus
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.SolarStatus, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this SolarStatus to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a PowerStatus. */
    interface IPowerStatus {

        /** PowerStatus battery */
        battery?: fk_app.IBatteryStatus;

        /** PowerStatus solar */
        solar?: fk_app.ISolarStatus;
    }

    /** Represents a PowerStatus. */
    class PowerStatus {

        /**
         * Constructs a new PowerStatus.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IPowerStatus);

        /** PowerStatus battery. */
        public battery?: (fk_app.IBatteryStatus|null);

        /** PowerStatus solar. */
        public solar?: (fk_app.ISolarStatus|null);

        /**
         * Creates a new PowerStatus instance using the specified properties.
         * @param [properties] Properties to set
         * @returns PowerStatus instance
         */
        public static create(properties?: fk_app.IPowerStatus): fk_app.PowerStatus;

        /**
         * Encodes the specified PowerStatus message. Does not implicitly {@link fk_app.PowerStatus.verify|verify} messages.
         * @param message PowerStatus message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IPowerStatus, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified PowerStatus message, length delimited. Does not implicitly {@link fk_app.PowerStatus.verify|verify} messages.
         * @param message PowerStatus message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IPowerStatus, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a PowerStatus message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns PowerStatus
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.PowerStatus;

        /**
         * Decodes a PowerStatus message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns PowerStatus
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.PowerStatus;

        /**
         * Verifies a PowerStatus message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a PowerStatus message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns PowerStatus
         */
        public static fromObject(object: { [k: string]: any }): fk_app.PowerStatus;

        /**
         * Creates a plain object from a PowerStatus message. Also converts values to other types if specified.
         * @param message PowerStatus
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.PowerStatus, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this PowerStatus to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a Status. */
    interface IStatus {

        /** Status version */
        version?: number;

        /** Status uptime */
        uptime?: number;

        /** Status identity */
        identity?: fk_app.IIdentity;

        /** Status hardware */
        hardware?: fk_app.IHardwareStatus;

        /** Status power */
        power?: fk_app.IPowerStatus;

        /** Status memory */
        memory?: fk_app.IMemoryStatus;

        /** Status gps */
        gps?: fk_app.IGpsStatus;

        /** Status schedules */
        schedules?: fk_app.ISchedules;

        /** Status recording */
        recording?: fk_app.IRecording;

        /** Status network */
        network?: fk_app.INetworkSettings;

        /** Status time */
        time?: (number|Long);

        /** Status firmware */
        firmware?: fk_app.IFirmware;

        /** Status logs */
        logs?: string;
    }

    /** Represents a Status. */
    class Status {

        /**
         * Constructs a new Status.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IStatus);

        /** Status version. */
        public version: number;

        /** Status uptime. */
        public uptime: number;

        /** Status identity. */
        public identity?: (fk_app.IIdentity|null);

        /** Status hardware. */
        public hardware?: (fk_app.IHardwareStatus|null);

        /** Status power. */
        public power?: (fk_app.IPowerStatus|null);

        /** Status memory. */
        public memory?: (fk_app.IMemoryStatus|null);

        /** Status gps. */
        public gps?: (fk_app.IGpsStatus|null);

        /** Status schedules. */
        public schedules?: (fk_app.ISchedules|null);

        /** Status recording. */
        public recording?: (fk_app.IRecording|null);

        /** Status network. */
        public network?: (fk_app.INetworkSettings|null);

        /** Status time. */
        public time: (number|Long);

        /** Status firmware. */
        public firmware?: (fk_app.IFirmware|null);

        /** Status logs. */
        public logs: string;

        /**
         * Creates a new Status instance using the specified properties.
         * @param [properties] Properties to set
         * @returns Status instance
         */
        public static create(properties?: fk_app.IStatus): fk_app.Status;

        /**
         * Encodes the specified Status message. Does not implicitly {@link fk_app.Status.verify|verify} messages.
         * @param message Status message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IStatus, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Status message, length delimited. Does not implicitly {@link fk_app.Status.verify|verify} messages.
         * @param message Status message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IStatus, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a Status message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Status
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.Status;

        /**
         * Decodes a Status message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Status
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.Status;

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
        public static fromObject(object: { [k: string]: any }): fk_app.Status;

        /**
         * Creates a plain object from a Status message. Also converts values to other types if specified.
         * @param message Status
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.Status, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Status to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a Range. */
    interface IRange {

        /** Range start */
        start?: number;

        /** Range end */
        end?: number;
    }

    /** Represents a Range. */
    class Range {

        /**
         * Constructs a new Range.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IRange);

        /** Range start. */
        public start: number;

        /** Range end. */
        public end: number;

        /**
         * Creates a new Range instance using the specified properties.
         * @param [properties] Properties to set
         * @returns Range instance
         */
        public static create(properties?: fk_app.IRange): fk_app.Range;

        /**
         * Encodes the specified Range message. Does not implicitly {@link fk_app.Range.verify|verify} messages.
         * @param message Range message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IRange, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Range message, length delimited. Does not implicitly {@link fk_app.Range.verify|verify} messages.
         * @param message Range message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IRange, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a Range message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Range
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.Range;

        /**
         * Decodes a Range message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Range
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.Range;

        /**
         * Verifies a Range message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a Range message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns Range
         */
        public static fromObject(object: { [k: string]: any }): fk_app.Range;

        /**
         * Creates a plain object from a Range message. Also converts values to other types if specified.
         * @param message Range
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.Range, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Range to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a DownloadQuery. */
    interface IDownloadQuery {

        /** DownloadQuery stream */
        stream?: number;

        /** DownloadQuery ranges */
        ranges?: fk_app.IRange[];

        /** DownloadQuery blocks */
        blocks?: number[];
    }

    /** Represents a DownloadQuery. */
    class DownloadQuery {

        /**
         * Constructs a new DownloadQuery.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IDownloadQuery);

        /** DownloadQuery stream. */
        public stream: number;

        /** DownloadQuery ranges. */
        public ranges: fk_app.IRange[];

        /** DownloadQuery blocks. */
        public blocks: number[];

        /**
         * Creates a new DownloadQuery instance using the specified properties.
         * @param [properties] Properties to set
         * @returns DownloadQuery instance
         */
        public static create(properties?: fk_app.IDownloadQuery): fk_app.DownloadQuery;

        /**
         * Encodes the specified DownloadQuery message. Does not implicitly {@link fk_app.DownloadQuery.verify|verify} messages.
         * @param message DownloadQuery message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IDownloadQuery, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified DownloadQuery message, length delimited. Does not implicitly {@link fk_app.DownloadQuery.verify|verify} messages.
         * @param message DownloadQuery message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IDownloadQuery, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a DownloadQuery message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns DownloadQuery
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.DownloadQuery;

        /**
         * Decodes a DownloadQuery message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns DownloadQuery
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.DownloadQuery;

        /**
         * Verifies a DownloadQuery message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a DownloadQuery message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns DownloadQuery
         */
        public static fromObject(object: { [k: string]: any }): fk_app.DownloadQuery;

        /**
         * Creates a plain object from a DownloadQuery message. Also converts values to other types if specified.
         * @param message DownloadQuery
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.DownloadQuery, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this DownloadQuery to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a Recording. */
    interface IRecording {

        /** Recording modifying */
        modifying?: boolean;

        /** Recording enabled */
        enabled?: boolean;

        /** Recording startedTime */
        startedTime?: (number|Long);

        /** Recording location */
        location?: fk_app.ILocation;
    }

    /** Represents a Recording. */
    class Recording {

        /**
         * Constructs a new Recording.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IRecording);

        /** Recording modifying. */
        public modifying: boolean;

        /** Recording enabled. */
        public enabled: boolean;

        /** Recording startedTime. */
        public startedTime: (number|Long);

        /** Recording location. */
        public location?: (fk_app.ILocation|null);

        /**
         * Creates a new Recording instance using the specified properties.
         * @param [properties] Properties to set
         * @returns Recording instance
         */
        public static create(properties?: fk_app.IRecording): fk_app.Recording;

        /**
         * Encodes the specified Recording message. Does not implicitly {@link fk_app.Recording.verify|verify} messages.
         * @param message Recording message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IRecording, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Recording message, length delimited. Does not implicitly {@link fk_app.Recording.verify|verify} messages.
         * @param message Recording message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IRecording, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a Recording message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Recording
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.Recording;

        /**
         * Decodes a Recording message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Recording
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.Recording;

        /**
         * Verifies a Recording message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a Recording message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns Recording
         */
        public static fromObject(object: { [k: string]: any }): fk_app.Recording;

        /**
         * Creates a plain object from a Recording message. Also converts values to other types if specified.
         * @param message Recording
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.Recording, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Recording to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a LoraSettings. */
    interface ILoraSettings {

        /** LoraSettings available */
        available?: boolean;

        /** LoraSettings modifying */
        modifying?: boolean;

        /** LoraSettings clearing */
        clearing?: boolean;

        /** LoraSettings frequencyBand */
        frequencyBand?: number;

        /** LoraSettings deviceEui */
        deviceEui?: Uint8Array;

        /** LoraSettings appKey */
        appKey?: Uint8Array;

        /** LoraSettings joinEui */
        joinEui?: Uint8Array;

        /** LoraSettings deviceAddress */
        deviceAddress?: Uint8Array;

        /** LoraSettings networkSessionKey */
        networkSessionKey?: Uint8Array;

        /** LoraSettings appSessionKey */
        appSessionKey?: Uint8Array;
    }

    /** Represents a LoraSettings. */
    class LoraSettings {

        /**
         * Constructs a new LoraSettings.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.ILoraSettings);

        /** LoraSettings available. */
        public available: boolean;

        /** LoraSettings modifying. */
        public modifying: boolean;

        /** LoraSettings clearing. */
        public clearing: boolean;

        /** LoraSettings frequencyBand. */
        public frequencyBand: number;

        /** LoraSettings deviceEui. */
        public deviceEui: Uint8Array;

        /** LoraSettings appKey. */
        public appKey: Uint8Array;

        /** LoraSettings joinEui. */
        public joinEui: Uint8Array;

        /** LoraSettings deviceAddress. */
        public deviceAddress: Uint8Array;

        /** LoraSettings networkSessionKey. */
        public networkSessionKey: Uint8Array;

        /** LoraSettings appSessionKey. */
        public appSessionKey: Uint8Array;

        /**
         * Creates a new LoraSettings instance using the specified properties.
         * @param [properties] Properties to set
         * @returns LoraSettings instance
         */
        public static create(properties?: fk_app.ILoraSettings): fk_app.LoraSettings;

        /**
         * Encodes the specified LoraSettings message. Does not implicitly {@link fk_app.LoraSettings.verify|verify} messages.
         * @param message LoraSettings message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.ILoraSettings, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified LoraSettings message, length delimited. Does not implicitly {@link fk_app.LoraSettings.verify|verify} messages.
         * @param message LoraSettings message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.ILoraSettings, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a LoraSettings message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns LoraSettings
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.LoraSettings;

        /**
         * Decodes a LoraSettings message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns LoraSettings
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.LoraSettings;

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
        public static fromObject(object: { [k: string]: any }): fk_app.LoraSettings;

        /**
         * Creates a plain object from a LoraSettings message. Also converts values to other types if specified.
         * @param message LoraSettings
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.LoraSettings, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this LoraSettings to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a Location. */
    interface ILocation {

        /** Location modifying */
        modifying?: boolean;

        /** Location longitude */
        longitude?: number;

        /** Location latitude */
        latitude?: number;

        /** Location time */
        time?: (number|Long);
    }

    /** Represents a Location. */
    class Location {

        /**
         * Constructs a new Location.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.ILocation);

        /** Location modifying. */
        public modifying: boolean;

        /** Location longitude. */
        public longitude: number;

        /** Location latitude. */
        public latitude: number;

        /** Location time. */
        public time: (number|Long);

        /**
         * Creates a new Location instance using the specified properties.
         * @param [properties] Properties to set
         * @returns Location instance
         */
        public static create(properties?: fk_app.ILocation): fk_app.Location;

        /**
         * Encodes the specified Location message. Does not implicitly {@link fk_app.Location.verify|verify} messages.
         * @param message Location message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.ILocation, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Location message, length delimited. Does not implicitly {@link fk_app.Location.verify|verify} messages.
         * @param message Location message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.ILocation, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a Location message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Location
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.Location;

        /**
         * Decodes a Location message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Location
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.Location;

        /**
         * Verifies a Location message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a Location message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns Location
         */
        public static fromObject(object: { [k: string]: any }): fk_app.Location;

        /**
         * Creates a plain object from a Location message. Also converts values to other types if specified.
         * @param message Location
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.Location, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Location to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a WifiTransmission. */
    interface IWifiTransmission {

        /** WifiTransmission modifying */
        modifying?: boolean;

        /** WifiTransmission url */
        url?: string;

        /** WifiTransmission token */
        token?: string;

        /** WifiTransmission enabled */
        enabled?: boolean;
    }

    /** Represents a WifiTransmission. */
    class WifiTransmission {

        /**
         * Constructs a new WifiTransmission.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IWifiTransmission);

        /** WifiTransmission modifying. */
        public modifying: boolean;

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
        public static create(properties?: fk_app.IWifiTransmission): fk_app.WifiTransmission;

        /**
         * Encodes the specified WifiTransmission message. Does not implicitly {@link fk_app.WifiTransmission.verify|verify} messages.
         * @param message WifiTransmission message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IWifiTransmission, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified WifiTransmission message, length delimited. Does not implicitly {@link fk_app.WifiTransmission.verify|verify} messages.
         * @param message WifiTransmission message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IWifiTransmission, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a WifiTransmission message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns WifiTransmission
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.WifiTransmission;

        /**
         * Decodes a WifiTransmission message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns WifiTransmission
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.WifiTransmission;

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
        public static fromObject(object: { [k: string]: any }): fk_app.WifiTransmission;

        /**
         * Creates a plain object from a WifiTransmission message. Also converts values to other types if specified.
         * @param message WifiTransmission
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.WifiTransmission, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this WifiTransmission to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a Transmission. */
    interface ITransmission {

        /** Transmission wifi */
        wifi?: fk_app.IWifiTransmission;
    }

    /** Represents a Transmission. */
    class Transmission {

        /**
         * Constructs a new Transmission.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.ITransmission);

        /** Transmission wifi. */
        public wifi?: (fk_app.IWifiTransmission|null);

        /**
         * Creates a new Transmission instance using the specified properties.
         * @param [properties] Properties to set
         * @returns Transmission instance
         */
        public static create(properties?: fk_app.ITransmission): fk_app.Transmission;

        /**
         * Encodes the specified Transmission message. Does not implicitly {@link fk_app.Transmission.verify|verify} messages.
         * @param message Transmission message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.ITransmission, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Transmission message, length delimited. Does not implicitly {@link fk_app.Transmission.verify|verify} messages.
         * @param message Transmission message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.ITransmission, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a Transmission message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Transmission
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.Transmission;

        /**
         * Decodes a Transmission message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Transmission
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.Transmission;

        /**
         * Verifies a Transmission message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a Transmission message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns Transmission
         */
        public static fromObject(object: { [k: string]: any }): fk_app.Transmission;

        /**
         * Creates a plain object from a Transmission message. Also converts values to other types if specified.
         * @param message Transmission
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.Transmission, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Transmission to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a ListDirectory. */
    interface IListDirectory {

        /** ListDirectory path */
        path?: string;

        /** ListDirectory page */
        page?: number;
    }

    /** Represents a ListDirectory. */
    class ListDirectory {

        /**
         * Constructs a new ListDirectory.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IListDirectory);

        /** ListDirectory path. */
        public path: string;

        /** ListDirectory page. */
        public page: number;

        /**
         * Creates a new ListDirectory instance using the specified properties.
         * @param [properties] Properties to set
         * @returns ListDirectory instance
         */
        public static create(properties?: fk_app.IListDirectory): fk_app.ListDirectory;

        /**
         * Encodes the specified ListDirectory message. Does not implicitly {@link fk_app.ListDirectory.verify|verify} messages.
         * @param message ListDirectory message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IListDirectory, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified ListDirectory message, length delimited. Does not implicitly {@link fk_app.ListDirectory.verify|verify} messages.
         * @param message ListDirectory message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IListDirectory, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a ListDirectory message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns ListDirectory
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.ListDirectory;

        /**
         * Decodes a ListDirectory message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns ListDirectory
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.ListDirectory;

        /**
         * Verifies a ListDirectory message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a ListDirectory message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns ListDirectory
         */
        public static fromObject(object: { [k: string]: any }): fk_app.ListDirectory;

        /**
         * Creates a plain object from a ListDirectory message. Also converts values to other types if specified.
         * @param message ListDirectory
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.ListDirectory, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this ListDirectory to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a HttpQuery. */
    interface IHttpQuery {

        /** HttpQuery type */
        type?: fk_app.QueryType;

        /** HttpQuery identity */
        identity?: fk_app.IIdentity;

        /** HttpQuery recording */
        recording?: fk_app.IRecording;

        /** HttpQuery schedules */
        schedules?: fk_app.ISchedules;

        /** HttpQuery networkSettings */
        networkSettings?: fk_app.INetworkSettings;

        /** HttpQuery loraSettings */
        loraSettings?: fk_app.ILoraSettings;

        /** HttpQuery locate */
        locate?: fk_app.ILocation;

        /** HttpQuery transmission */
        transmission?: fk_app.ITransmission;

        /** HttpQuery directory */
        directory?: fk_app.IListDirectory;

        /** HttpQuery flags */
        flags?: number;

        /** HttpQuery time */
        time?: (number|Long);
    }

    /** Represents a HttpQuery. */
    class HttpQuery {

        /**
         * Constructs a new HttpQuery.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IHttpQuery);

        /** HttpQuery type. */
        public type: fk_app.QueryType;

        /** HttpQuery identity. */
        public identity?: (fk_app.IIdentity|null);

        /** HttpQuery recording. */
        public recording?: (fk_app.IRecording|null);

        /** HttpQuery schedules. */
        public schedules?: (fk_app.ISchedules|null);

        /** HttpQuery networkSettings. */
        public networkSettings?: (fk_app.INetworkSettings|null);

        /** HttpQuery loraSettings. */
        public loraSettings?: (fk_app.ILoraSettings|null);

        /** HttpQuery locate. */
        public locate?: (fk_app.ILocation|null);

        /** HttpQuery transmission. */
        public transmission?: (fk_app.ITransmission|null);

        /** HttpQuery directory. */
        public directory?: (fk_app.IListDirectory|null);

        /** HttpQuery flags. */
        public flags: number;

        /** HttpQuery time. */
        public time: (number|Long);

        /**
         * Creates a new HttpQuery instance using the specified properties.
         * @param [properties] Properties to set
         * @returns HttpQuery instance
         */
        public static create(properties?: fk_app.IHttpQuery): fk_app.HttpQuery;

        /**
         * Encodes the specified HttpQuery message. Does not implicitly {@link fk_app.HttpQuery.verify|verify} messages.
         * @param message HttpQuery message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IHttpQuery, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified HttpQuery message, length delimited. Does not implicitly {@link fk_app.HttpQuery.verify|verify} messages.
         * @param message HttpQuery message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IHttpQuery, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a HttpQuery message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns HttpQuery
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.HttpQuery;

        /**
         * Decodes a HttpQuery message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns HttpQuery
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.HttpQuery;

        /**
         * Verifies a HttpQuery message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a HttpQuery message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns HttpQuery
         */
        public static fromObject(object: { [k: string]: any }): fk_app.HttpQuery;

        /**
         * Creates a plain object from a HttpQuery message. Also converts values to other types if specified.
         * @param message HttpQuery
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.HttpQuery, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this HttpQuery to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a DataStream. */
    interface IDataStream {

        /** DataStream id */
        id?: number;

        /** DataStream time */
        time?: (number|Long);

        /** DataStream size */
        size?: (number|Long);

        /** DataStream version */
        version?: number;

        /** DataStream block */
        block?: (number|Long);

        /** DataStream hash */
        hash?: Uint8Array;

        /** DataStream name */
        name?: string;

        /** DataStream path */
        path?: string;
    }

    /** Represents a DataStream. */
    class DataStream {

        /**
         * Constructs a new DataStream.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IDataStream);

        /** DataStream id. */
        public id: number;

        /** DataStream time. */
        public time: (number|Long);

        /** DataStream size. */
        public size: (number|Long);

        /** DataStream version. */
        public version: number;

        /** DataStream block. */
        public block: (number|Long);

        /** DataStream hash. */
        public hash: Uint8Array;

        /** DataStream name. */
        public name: string;

        /** DataStream path. */
        public path: string;

        /**
         * Creates a new DataStream instance using the specified properties.
         * @param [properties] Properties to set
         * @returns DataStream instance
         */
        public static create(properties?: fk_app.IDataStream): fk_app.DataStream;

        /**
         * Encodes the specified DataStream message. Does not implicitly {@link fk_app.DataStream.verify|verify} messages.
         * @param message DataStream message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IDataStream, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified DataStream message, length delimited. Does not implicitly {@link fk_app.DataStream.verify|verify} messages.
         * @param message DataStream message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IDataStream, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a DataStream message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns DataStream
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.DataStream;

        /**
         * Decodes a DataStream message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns DataStream
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.DataStream;

        /**
         * Verifies a DataStream message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a DataStream message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns DataStream
         */
        public static fromObject(object: { [k: string]: any }): fk_app.DataStream;

        /**
         * Creates a plain object from a DataStream message. Also converts values to other types if specified.
         * @param message DataStream
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.DataStream, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this DataStream to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a LiveSensorReading. */
    interface ILiveSensorReading {

        /** LiveSensorReading sensor */
        sensor?: fk_app.ISensorCapabilities;

        /** LiveSensorReading value */
        value?: number;

        /** LiveSensorReading uncalibrated */
        uncalibrated?: number;

        /** LiveSensorReading factory */
        factory?: number;
    }

    /** Represents a LiveSensorReading. */
    class LiveSensorReading {

        /**
         * Constructs a new LiveSensorReading.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.ILiveSensorReading);

        /** LiveSensorReading sensor. */
        public sensor?: (fk_app.ISensorCapabilities|null);

        /** LiveSensorReading value. */
        public value: number;

        /** LiveSensorReading uncalibrated. */
        public uncalibrated: number;

        /** LiveSensorReading factory. */
        public factory: number;

        /**
         * Creates a new LiveSensorReading instance using the specified properties.
         * @param [properties] Properties to set
         * @returns LiveSensorReading instance
         */
        public static create(properties?: fk_app.ILiveSensorReading): fk_app.LiveSensorReading;

        /**
         * Encodes the specified LiveSensorReading message. Does not implicitly {@link fk_app.LiveSensorReading.verify|verify} messages.
         * @param message LiveSensorReading message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.ILiveSensorReading, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified LiveSensorReading message, length delimited. Does not implicitly {@link fk_app.LiveSensorReading.verify|verify} messages.
         * @param message LiveSensorReading message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.ILiveSensorReading, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a LiveSensorReading message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns LiveSensorReading
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.LiveSensorReading;

        /**
         * Decodes a LiveSensorReading message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns LiveSensorReading
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.LiveSensorReading;

        /**
         * Verifies a LiveSensorReading message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a LiveSensorReading message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns LiveSensorReading
         */
        public static fromObject(object: { [k: string]: any }): fk_app.LiveSensorReading;

        /**
         * Creates a plain object from a LiveSensorReading message. Also converts values to other types if specified.
         * @param message LiveSensorReading
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.LiveSensorReading, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this LiveSensorReading to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a LiveModuleReadings. */
    interface ILiveModuleReadings {

        /** LiveModuleReadings module */
        module?: fk_app.IModuleCapabilities;

        /** LiveModuleReadings readings */
        readings?: fk_app.ILiveSensorReading[];
    }

    /** Represents a LiveModuleReadings. */
    class LiveModuleReadings {

        /**
         * Constructs a new LiveModuleReadings.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.ILiveModuleReadings);

        /** LiveModuleReadings module. */
        public module?: (fk_app.IModuleCapabilities|null);

        /** LiveModuleReadings readings. */
        public readings: fk_app.ILiveSensorReading[];

        /**
         * Creates a new LiveModuleReadings instance using the specified properties.
         * @param [properties] Properties to set
         * @returns LiveModuleReadings instance
         */
        public static create(properties?: fk_app.ILiveModuleReadings): fk_app.LiveModuleReadings;

        /**
         * Encodes the specified LiveModuleReadings message. Does not implicitly {@link fk_app.LiveModuleReadings.verify|verify} messages.
         * @param message LiveModuleReadings message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.ILiveModuleReadings, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified LiveModuleReadings message, length delimited. Does not implicitly {@link fk_app.LiveModuleReadings.verify|verify} messages.
         * @param message LiveModuleReadings message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.ILiveModuleReadings, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a LiveModuleReadings message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns LiveModuleReadings
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.LiveModuleReadings;

        /**
         * Decodes a LiveModuleReadings message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns LiveModuleReadings
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.LiveModuleReadings;

        /**
         * Verifies a LiveModuleReadings message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a LiveModuleReadings message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns LiveModuleReadings
         */
        public static fromObject(object: { [k: string]: any }): fk_app.LiveModuleReadings;

        /**
         * Creates a plain object from a LiveModuleReadings message. Also converts values to other types if specified.
         * @param message LiveModuleReadings
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.LiveModuleReadings, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this LiveModuleReadings to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a LiveReadings. */
    interface ILiveReadings {

        /** LiveReadings time */
        time?: (number|Long);

        /** LiveReadings modules */
        modules?: fk_app.ILiveModuleReadings[];
    }

    /** Represents a LiveReadings. */
    class LiveReadings {

        /**
         * Constructs a new LiveReadings.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.ILiveReadings);

        /** LiveReadings time. */
        public time: (number|Long);

        /** LiveReadings modules. */
        public modules: fk_app.ILiveModuleReadings[];

        /**
         * Creates a new LiveReadings instance using the specified properties.
         * @param [properties] Properties to set
         * @returns LiveReadings instance
         */
        public static create(properties?: fk_app.ILiveReadings): fk_app.LiveReadings;

        /**
         * Encodes the specified LiveReadings message. Does not implicitly {@link fk_app.LiveReadings.verify|verify} messages.
         * @param message LiveReadings message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.ILiveReadings, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified LiveReadings message, length delimited. Does not implicitly {@link fk_app.LiveReadings.verify|verify} messages.
         * @param message LiveReadings message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.ILiveReadings, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a LiveReadings message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns LiveReadings
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.LiveReadings;

        /**
         * Decodes a LiveReadings message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns LiveReadings
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.LiveReadings;

        /**
         * Verifies a LiveReadings message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a LiveReadings message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns LiveReadings
         */
        public static fromObject(object: { [k: string]: any }): fk_app.LiveReadings;

        /**
         * Creates a plain object from a LiveReadings message. Also converts values to other types if specified.
         * @param message LiveReadings
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.LiveReadings, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this LiveReadings to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a DirectoryEntry. */
    interface IDirectoryEntry {

        /** DirectoryEntry name */
        name?: string;

        /** DirectoryEntry path */
        path?: string;

        /** DirectoryEntry size */
        size?: number;

        /** DirectoryEntry directory */
        directory?: boolean;
    }

    /** Represents a DirectoryEntry. */
    class DirectoryEntry {

        /**
         * Constructs a new DirectoryEntry.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IDirectoryEntry);

        /** DirectoryEntry name. */
        public name: string;

        /** DirectoryEntry path. */
        public path: string;

        /** DirectoryEntry size. */
        public size: number;

        /** DirectoryEntry directory. */
        public directory: boolean;

        /**
         * Creates a new DirectoryEntry instance using the specified properties.
         * @param [properties] Properties to set
         * @returns DirectoryEntry instance
         */
        public static create(properties?: fk_app.IDirectoryEntry): fk_app.DirectoryEntry;

        /**
         * Encodes the specified DirectoryEntry message. Does not implicitly {@link fk_app.DirectoryEntry.verify|verify} messages.
         * @param message DirectoryEntry message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IDirectoryEntry, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified DirectoryEntry message, length delimited. Does not implicitly {@link fk_app.DirectoryEntry.verify|verify} messages.
         * @param message DirectoryEntry message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IDirectoryEntry, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a DirectoryEntry message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns DirectoryEntry
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.DirectoryEntry;

        /**
         * Decodes a DirectoryEntry message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns DirectoryEntry
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.DirectoryEntry;

        /**
         * Verifies a DirectoryEntry message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a DirectoryEntry message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns DirectoryEntry
         */
        public static fromObject(object: { [k: string]: any }): fk_app.DirectoryEntry;

        /**
         * Creates a plain object from a DirectoryEntry message. Also converts values to other types if specified.
         * @param message DirectoryEntry
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.DirectoryEntry, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this DirectoryEntry to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a DirectoryListing. */
    interface IDirectoryListing {

        /** DirectoryListing path */
        path?: string;

        /** DirectoryListing totalEntries */
        totalEntries?: number;

        /** DirectoryListing entries */
        entries?: fk_app.IDirectoryEntry[];
    }

    /** Represents a DirectoryListing. */
    class DirectoryListing {

        /**
         * Constructs a new DirectoryListing.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IDirectoryListing);

        /** DirectoryListing path. */
        public path: string;

        /** DirectoryListing totalEntries. */
        public totalEntries: number;

        /** DirectoryListing entries. */
        public entries: fk_app.IDirectoryEntry[];

        /**
         * Creates a new DirectoryListing instance using the specified properties.
         * @param [properties] Properties to set
         * @returns DirectoryListing instance
         */
        public static create(properties?: fk_app.IDirectoryListing): fk_app.DirectoryListing;

        /**
         * Encodes the specified DirectoryListing message. Does not implicitly {@link fk_app.DirectoryListing.verify|verify} messages.
         * @param message DirectoryListing message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IDirectoryListing, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified DirectoryListing message, length delimited. Does not implicitly {@link fk_app.DirectoryListing.verify|verify} messages.
         * @param message DirectoryListing message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IDirectoryListing, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a DirectoryListing message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns DirectoryListing
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.DirectoryListing;

        /**
         * Decodes a DirectoryListing message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns DirectoryListing
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.DirectoryListing;

        /**
         * Verifies a DirectoryListing message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a DirectoryListing message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns DirectoryListing
         */
        public static fromObject(object: { [k: string]: any }): fk_app.DirectoryListing;

        /**
         * Creates a plain object from a DirectoryListing message. Also converts values to other types if specified.
         * @param message DirectoryListing
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.DirectoryListing, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this DirectoryListing to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a NearbyNetwork. */
    interface INearbyNetwork {

        /** NearbyNetwork ssid */
        ssid?: string;
    }

    /** Represents a NearbyNetwork. */
    class NearbyNetwork {

        /**
         * Constructs a new NearbyNetwork.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.INearbyNetwork);

        /** NearbyNetwork ssid. */
        public ssid: string;

        /**
         * Creates a new NearbyNetwork instance using the specified properties.
         * @param [properties] Properties to set
         * @returns NearbyNetwork instance
         */
        public static create(properties?: fk_app.INearbyNetwork): fk_app.NearbyNetwork;

        /**
         * Encodes the specified NearbyNetwork message. Does not implicitly {@link fk_app.NearbyNetwork.verify|verify} messages.
         * @param message NearbyNetwork message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.INearbyNetwork, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified NearbyNetwork message, length delimited. Does not implicitly {@link fk_app.NearbyNetwork.verify|verify} messages.
         * @param message NearbyNetwork message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.INearbyNetwork, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a NearbyNetwork message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns NearbyNetwork
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.NearbyNetwork;

        /**
         * Decodes a NearbyNetwork message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns NearbyNetwork
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.NearbyNetwork;

        /**
         * Verifies a NearbyNetwork message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a NearbyNetwork message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns NearbyNetwork
         */
        public static fromObject(object: { [k: string]: any }): fk_app.NearbyNetwork;

        /**
         * Creates a plain object from a NearbyNetwork message. Also converts values to other types if specified.
         * @param message NearbyNetwork
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.NearbyNetwork, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this NearbyNetwork to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a NearbyNetworks. */
    interface INearbyNetworks {

        /** NearbyNetworks networks */
        networks?: fk_app.INearbyNetwork[];
    }

    /** Represents a NearbyNetworks. */
    class NearbyNetworks {

        /**
         * Constructs a new NearbyNetworks.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.INearbyNetworks);

        /** NearbyNetworks networks. */
        public networks: fk_app.INearbyNetwork[];

        /**
         * Creates a new NearbyNetworks instance using the specified properties.
         * @param [properties] Properties to set
         * @returns NearbyNetworks instance
         */
        public static create(properties?: fk_app.INearbyNetworks): fk_app.NearbyNetworks;

        /**
         * Encodes the specified NearbyNetworks message. Does not implicitly {@link fk_app.NearbyNetworks.verify|verify} messages.
         * @param message NearbyNetworks message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.INearbyNetworks, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified NearbyNetworks message, length delimited. Does not implicitly {@link fk_app.NearbyNetworks.verify|verify} messages.
         * @param message NearbyNetworks message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.INearbyNetworks, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a NearbyNetworks message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns NearbyNetworks
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.NearbyNetworks;

        /**
         * Decodes a NearbyNetworks message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns NearbyNetworks
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.NearbyNetworks;

        /**
         * Verifies a NearbyNetworks message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a NearbyNetworks message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns NearbyNetworks
         */
        public static fromObject(object: { [k: string]: any }): fk_app.NearbyNetworks;

        /**
         * Creates a plain object from a NearbyNetworks message. Also converts values to other types if specified.
         * @param message NearbyNetworks
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.NearbyNetworks, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this NearbyNetworks to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a Fault. */
    interface IFault {

        /** Fault time */
        time?: number;

        /** Fault code */
        code?: number;

        /** Fault description */
        description?: string;

        /** Fault debug */
        debug?: Uint8Array;
    }

    /** Represents a Fault. */
    class Fault {

        /**
         * Constructs a new Fault.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IFault);

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
        public static create(properties?: fk_app.IFault): fk_app.Fault;

        /**
         * Encodes the specified Fault message. Does not implicitly {@link fk_app.Fault.verify|verify} messages.
         * @param message Fault message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IFault, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Fault message, length delimited. Does not implicitly {@link fk_app.Fault.verify|verify} messages.
         * @param message Fault message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IFault, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a Fault message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Fault
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.Fault;

        /**
         * Decodes a Fault message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Fault
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.Fault;

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
        public static fromObject(object: { [k: string]: any }): fk_app.Fault;

        /**
         * Creates a plain object from a Fault message. Also converts values to other types if specified.
         * @param message Fault
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.Fault, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Fault to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a HttpReply. */
    interface IHttpReply {

        /** HttpReply type */
        type?: fk_app.ReplyType;

        /** HttpReply errors */
        errors?: fk_app.IError[];

        /** HttpReply status */
        status?: fk_app.IStatus;

        /** HttpReply networkSettings */
        networkSettings?: fk_app.INetworkSettings;

        /** HttpReply loraSettings */
        loraSettings?: fk_app.ILoraSettings;

        /** HttpReply modules */
        modules?: fk_app.IModuleCapabilities[];

        /** HttpReply streams */
        streams?: fk_app.IDataStream[];

        /** HttpReply liveReadings */
        liveReadings?: fk_app.ILiveReadings[];

        /** HttpReply schedules */
        schedules?: fk_app.ISchedules;

        /** HttpReply transmission */
        transmission?: fk_app.ITransmission;

        /** HttpReply listing */
        listing?: fk_app.IDirectoryListing;

        /** HttpReply nearbyNetworks */
        nearbyNetworks?: fk_app.INearbyNetworks;

        /** HttpReply faults */
        faults?: fk_app.IFault[];
    }

    /** Represents a HttpReply. */
    class HttpReply {

        /**
         * Constructs a new HttpReply.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IHttpReply);

        /** HttpReply type. */
        public type: fk_app.ReplyType;

        /** HttpReply errors. */
        public errors: fk_app.IError[];

        /** HttpReply status. */
        public status?: (fk_app.IStatus|null);

        /** HttpReply networkSettings. */
        public networkSettings?: (fk_app.INetworkSettings|null);

        /** HttpReply loraSettings. */
        public loraSettings?: (fk_app.ILoraSettings|null);

        /** HttpReply modules. */
        public modules: fk_app.IModuleCapabilities[];

        /** HttpReply streams. */
        public streams: fk_app.IDataStream[];

        /** HttpReply liveReadings. */
        public liveReadings: fk_app.ILiveReadings[];

        /** HttpReply schedules. */
        public schedules?: (fk_app.ISchedules|null);

        /** HttpReply transmission. */
        public transmission?: (fk_app.ITransmission|null);

        /** HttpReply listing. */
        public listing?: (fk_app.IDirectoryListing|null);

        /** HttpReply nearbyNetworks. */
        public nearbyNetworks?: (fk_app.INearbyNetworks|null);

        /** HttpReply faults. */
        public faults: fk_app.IFault[];

        /**
         * Creates a new HttpReply instance using the specified properties.
         * @param [properties] Properties to set
         * @returns HttpReply instance
         */
        public static create(properties?: fk_app.IHttpReply): fk_app.HttpReply;

        /**
         * Encodes the specified HttpReply message. Does not implicitly {@link fk_app.HttpReply.verify|verify} messages.
         * @param message HttpReply message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IHttpReply, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified HttpReply message, length delimited. Does not implicitly {@link fk_app.HttpReply.verify|verify} messages.
         * @param message HttpReply message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IHttpReply, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a HttpReply message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns HttpReply
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.HttpReply;

        /**
         * Decodes a HttpReply message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns HttpReply
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.HttpReply;

        /**
         * Verifies a HttpReply message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a HttpReply message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns HttpReply
         */
        public static fromObject(object: { [k: string]: any }): fk_app.HttpReply;

        /**
         * Creates a plain object from a HttpReply message. Also converts values to other types if specified.
         * @param message HttpReply
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.HttpReply, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this HttpReply to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** ModuleQueryType enum. */
    enum ModuleQueryType {
        MODULE_QUERY_NONE = 0,
        MODULE_QUERY_STATUS = 1,
        MODULE_QUERY_CONFIGURE = 2,
        MODULE_QUERY_RESET = 3
    }

    /** Properties of a ModuleHttpQuery. */
    interface IModuleHttpQuery {

        /** ModuleHttpQuery type */
        type?: fk_app.ModuleQueryType;

        /** ModuleHttpQuery errors */
        errors?: fk_app.IError[];

        /** ModuleHttpQuery configuration */
        configuration?: Uint8Array;
    }

    /** Represents a ModuleHttpQuery. */
    class ModuleHttpQuery {

        /**
         * Constructs a new ModuleHttpQuery.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IModuleHttpQuery);

        /** ModuleHttpQuery type. */
        public type: fk_app.ModuleQueryType;

        /** ModuleHttpQuery errors. */
        public errors: fk_app.IError[];

        /** ModuleHttpQuery configuration. */
        public configuration: Uint8Array;

        /**
         * Creates a new ModuleHttpQuery instance using the specified properties.
         * @param [properties] Properties to set
         * @returns ModuleHttpQuery instance
         */
        public static create(properties?: fk_app.IModuleHttpQuery): fk_app.ModuleHttpQuery;

        /**
         * Encodes the specified ModuleHttpQuery message. Does not implicitly {@link fk_app.ModuleHttpQuery.verify|verify} messages.
         * @param message ModuleHttpQuery message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IModuleHttpQuery, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified ModuleHttpQuery message, length delimited. Does not implicitly {@link fk_app.ModuleHttpQuery.verify|verify} messages.
         * @param message ModuleHttpQuery message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IModuleHttpQuery, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a ModuleHttpQuery message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns ModuleHttpQuery
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.ModuleHttpQuery;

        /**
         * Decodes a ModuleHttpQuery message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns ModuleHttpQuery
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.ModuleHttpQuery;

        /**
         * Verifies a ModuleHttpQuery message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a ModuleHttpQuery message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns ModuleHttpQuery
         */
        public static fromObject(object: { [k: string]: any }): fk_app.ModuleHttpQuery;

        /**
         * Creates a plain object from a ModuleHttpQuery message. Also converts values to other types if specified.
         * @param message ModuleHttpQuery
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.ModuleHttpQuery, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this ModuleHttpQuery to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** ModuleReplyType enum. */
    enum ModuleReplyType {
        MODULE_REPLY_NONE = 0,
        MODULE_REPLY_SUCCESS = 1,
        MODULE_REPLY_BUSY = 2,
        MODULE_REPLY_ERROR = 3
    }

    /** Properties of a ModuleHttpReply. */
    interface IModuleHttpReply {

        /** ModuleHttpReply type */
        type?: fk_app.ModuleReplyType;

        /** ModuleHttpReply errors */
        errors?: fk_app.IError[];

        /** ModuleHttpReply configuration */
        configuration?: Uint8Array;
    }

    /** Represents a ModuleHttpReply. */
    class ModuleHttpReply {

        /**
         * Constructs a new ModuleHttpReply.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IModuleHttpReply);

        /** ModuleHttpReply type. */
        public type: fk_app.ModuleReplyType;

        /** ModuleHttpReply errors. */
        public errors: fk_app.IError[];

        /** ModuleHttpReply configuration. */
        public configuration: Uint8Array;

        /**
         * Creates a new ModuleHttpReply instance using the specified properties.
         * @param [properties] Properties to set
         * @returns ModuleHttpReply instance
         */
        public static create(properties?: fk_app.IModuleHttpReply): fk_app.ModuleHttpReply;

        /**
         * Encodes the specified ModuleHttpReply message. Does not implicitly {@link fk_app.ModuleHttpReply.verify|verify} messages.
         * @param message ModuleHttpReply message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IModuleHttpReply, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified ModuleHttpReply message, length delimited. Does not implicitly {@link fk_app.ModuleHttpReply.verify|verify} messages.
         * @param message ModuleHttpReply message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IModuleHttpReply, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a ModuleHttpReply message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns ModuleHttpReply
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.ModuleHttpReply;

        /**
         * Decodes a ModuleHttpReply message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns ModuleHttpReply
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.ModuleHttpReply;

        /**
         * Verifies a ModuleHttpReply message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a ModuleHttpReply message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns ModuleHttpReply
         */
        public static fromObject(object: { [k: string]: any }): fk_app.ModuleHttpReply;

        /**
         * Creates a plain object from a ModuleHttpReply message. Also converts values to other types if specified.
         * @param message ModuleHttpReply
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.ModuleHttpReply, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this ModuleHttpReply to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** UdpStatus enum. */
    enum UdpStatus {
        UDP_STATUS_ONLINE = 0,
        UDP_STATUS_BYE = 1
    }

    /** Properties of an UdpMessage. */
    interface IUdpMessage {

        /** UdpMessage deviceId */
        deviceId?: Uint8Array;

        /** UdpMessage status */
        status?: fk_app.UdpStatus;

        /** UdpMessage counter */
        counter?: number;
    }

    /** Represents an UdpMessage. */
    class UdpMessage {

        /**
         * Constructs a new UdpMessage.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_app.IUdpMessage);

        /** UdpMessage deviceId. */
        public deviceId: Uint8Array;

        /** UdpMessage status. */
        public status: fk_app.UdpStatus;

        /** UdpMessage counter. */
        public counter: number;

        /**
         * Creates a new UdpMessage instance using the specified properties.
         * @param [properties] Properties to set
         * @returns UdpMessage instance
         */
        public static create(properties?: fk_app.IUdpMessage): fk_app.UdpMessage;

        /**
         * Encodes the specified UdpMessage message. Does not implicitly {@link fk_app.UdpMessage.verify|verify} messages.
         * @param message UdpMessage message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_app.IUdpMessage, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified UdpMessage message, length delimited. Does not implicitly {@link fk_app.UdpMessage.verify|verify} messages.
         * @param message UdpMessage message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_app.IUdpMessage, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes an UdpMessage message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns UdpMessage
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_app.UdpMessage;

        /**
         * Decodes an UdpMessage message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns UdpMessage
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_app.UdpMessage;

        /**
         * Verifies an UdpMessage message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates an UdpMessage message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns UdpMessage
         */
        public static fromObject(object: { [k: string]: any }): fk_app.UdpMessage;

        /**
         * Creates a plain object from an UdpMessage message. Also converts values to other types if specified.
         * @param message UdpMessage
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_app.UdpMessage, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this UdpMessage to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }
}
