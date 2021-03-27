import * as $protobuf from "protobufjs";
/** Namespace fk_atlas. */
export namespace fk_atlas {

    /** SensorType enum. */
    enum SensorType {
        SENSOR_NONE = 0,
        SENSOR_PH = 1,
        SENSOR_TEMP = 2,
        SENSOR_ORP = 3,
        SENSOR_DO = 4,
        SENSOR_EC = 5
    }

    /** QueryType enum. */
    enum QueryType {
        QUERY_NONE = 0
    }

    /** CalibrationOperation enum. */
    enum CalibrationOperation {
        CALIBRATION_NONE = 0,
        CALIBRATION_STATUS = 1,
        CALIBRATION_CLEAR = 2,
        CALIBRATION_SET = 3
    }

    /** TempCalibrations enum. */
    enum TempCalibrations {
        TEMP_NONE = 0,
        TEMP_SINGLE = 1
    }

    /** TempCalibrateCommand enum. */
    enum TempCalibrateCommand {
        CALIBRATE_TEMP_NONE = 0,
        CALIBRATE_TEMP_CLEAR = 1,
        CALIBRATE_TEMP_SINGLE = 2
    }

    /** DoCalibrations enum. */
    enum DoCalibrations {
        DO_NONE = 0,
        DO_ATMOSPHERE = 1,
        DO_ZERO = 2
    }

    /** DoCalibrateCommand enum. */
    enum DoCalibrateCommand {
        CALIBRATE_DO_NONE = 0,
        CALIBRATE_DO_CLEAR = 1,
        CALIBRATE_DO_ATMOSPHERE = 2,
        CALIBRATE_DO_ZERO = 3
    }

    /** PhCalibrations enum. */
    enum PhCalibrations {
        PH_NONE = 0,
        PH_LOW = 1,
        PH_MIDDLE = 2,
        PH_HIGH = 4
    }

    /** PhCalibrateCommand enum. */
    enum PhCalibrateCommand {
        CALIBRATE_PH_NONE = 0,
        CALIBRATE_PH_CLEAR = 1,
        CALIBRATE_PH_LOW = 2,
        CALIBRATE_PH_MIDDLE = 3,
        CALIBRATE_PH_HIGH = 4
    }

    /** EcCalibrations enum. */
    enum EcCalibrations {
        EC_NONE = 0,
        EC_DRY = 1,
        EC_SINGLE = 2,
        EC_DUAL_LOW = 4,
        EC_DUAL_HIGH = 8
    }

    /** EcCalibrateCommand enum. */
    enum EcCalibrateCommand {
        CALIBRATE_EC_NONE = 0,
        CALIBRATE_EC_CLEAR = 1,
        CALIBRATE_EC_DRY = 2,
        CALIBRATE_EC_SINGLE = 3,
        CALIBRATE_EC_DUAL_LOW = 4,
        CALIBRATE_EC_DUAL_HIGH = 5
    }

    /** OrpCalibrations enum. */
    enum OrpCalibrations {
        ORP_NONE = 0,
        ORP_SINGLE = 1
    }

    /** OrpCalibrateCommand enum. */
    enum OrpCalibrateCommand {
        CALIBRATE_ORP_NONE = 0,
        CALIBRATE_ORP_SINGLE = 1
    }

    /** Properties of an AtlasCalibrationCommand. */
    interface IAtlasCalibrationCommand {

        /** AtlasCalibrationCommand operation */
        operation?: (fk_atlas.CalibrationOperation|null);

        /** AtlasCalibrationCommand temp */
        temp?: (fk_atlas.TempCalibrations|null);

        /** AtlasCalibrationCommand dissolvedOxygen */
        dissolvedOxygen?: (fk_atlas.DoCalibrations|null);

        /** AtlasCalibrationCommand ph */
        ph?: (fk_atlas.PhCalibrations|null);

        /** AtlasCalibrationCommand ec */
        ec?: (fk_atlas.EcCalibrations|null);

        /** AtlasCalibrationCommand orp */
        orp?: (fk_atlas.OrpCalibrations|null);

        /** AtlasCalibrationCommand which */
        which?: (number|null);

        /** AtlasCalibrationCommand value */
        value?: (number|null);

        /** AtlasCalibrationCommand configuration */
        configuration?: (Uint8Array|null);
    }

    /** Represents an AtlasCalibrationCommand. */
    class AtlasCalibrationCommand implements IAtlasCalibrationCommand {

        /**
         * Constructs a new AtlasCalibrationCommand.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_atlas.IAtlasCalibrationCommand);

        /** AtlasCalibrationCommand operation. */
        public operation: fk_atlas.CalibrationOperation;

        /** AtlasCalibrationCommand temp. */
        public temp: fk_atlas.TempCalibrations;

        /** AtlasCalibrationCommand dissolvedOxygen. */
        public dissolvedOxygen: fk_atlas.DoCalibrations;

        /** AtlasCalibrationCommand ph. */
        public ph: fk_atlas.PhCalibrations;

        /** AtlasCalibrationCommand ec. */
        public ec: fk_atlas.EcCalibrations;

        /** AtlasCalibrationCommand orp. */
        public orp: fk_atlas.OrpCalibrations;

        /** AtlasCalibrationCommand which. */
        public which: number;

        /** AtlasCalibrationCommand value. */
        public value: number;

        /** AtlasCalibrationCommand configuration. */
        public configuration: Uint8Array;

        /**
         * Creates a new AtlasCalibrationCommand instance using the specified properties.
         * @param [properties] Properties to set
         * @returns AtlasCalibrationCommand instance
         */
        public static create(properties?: fk_atlas.IAtlasCalibrationCommand): fk_atlas.AtlasCalibrationCommand;

        /**
         * Encodes the specified AtlasCalibrationCommand message. Does not implicitly {@link fk_atlas.AtlasCalibrationCommand.verify|verify} messages.
         * @param message AtlasCalibrationCommand message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_atlas.IAtlasCalibrationCommand, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified AtlasCalibrationCommand message, length delimited. Does not implicitly {@link fk_atlas.AtlasCalibrationCommand.verify|verify} messages.
         * @param message AtlasCalibrationCommand message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_atlas.IAtlasCalibrationCommand, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes an AtlasCalibrationCommand message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns AtlasCalibrationCommand
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_atlas.AtlasCalibrationCommand;

        /**
         * Decodes an AtlasCalibrationCommand message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns AtlasCalibrationCommand
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_atlas.AtlasCalibrationCommand;

        /**
         * Verifies an AtlasCalibrationCommand message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates an AtlasCalibrationCommand message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns AtlasCalibrationCommand
         */
        public static fromObject(object: { [k: string]: any }): fk_atlas.AtlasCalibrationCommand;

        /**
         * Creates a plain object from an AtlasCalibrationCommand message. Also converts values to other types if specified.
         * @param message AtlasCalibrationCommand
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_atlas.AtlasCalibrationCommand, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this AtlasCalibrationCommand to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a TwoWireQuery. */
    interface ITwoWireQuery {

        /** TwoWireQuery read */
        read?: (number|null);

        /** TwoWireQuery write */
        write?: (number|null);

        /** TwoWireQuery data */
        data?: (Uint8Array|null);
    }

    /** Represents a TwoWireQuery. */
    class TwoWireQuery implements ITwoWireQuery {

        /**
         * Constructs a new TwoWireQuery.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_atlas.ITwoWireQuery);

        /** TwoWireQuery read. */
        public read: number;

        /** TwoWireQuery write. */
        public write: number;

        /** TwoWireQuery data. */
        public data: Uint8Array;

        /**
         * Creates a new TwoWireQuery instance using the specified properties.
         * @param [properties] Properties to set
         * @returns TwoWireQuery instance
         */
        public static create(properties?: fk_atlas.ITwoWireQuery): fk_atlas.TwoWireQuery;

        /**
         * Encodes the specified TwoWireQuery message. Does not implicitly {@link fk_atlas.TwoWireQuery.verify|verify} messages.
         * @param message TwoWireQuery message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_atlas.ITwoWireQuery, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified TwoWireQuery message, length delimited. Does not implicitly {@link fk_atlas.TwoWireQuery.verify|verify} messages.
         * @param message TwoWireQuery message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_atlas.ITwoWireQuery, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a TwoWireQuery message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns TwoWireQuery
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_atlas.TwoWireQuery;

        /**
         * Decodes a TwoWireQuery message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns TwoWireQuery
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_atlas.TwoWireQuery;

        /**
         * Verifies a TwoWireQuery message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a TwoWireQuery message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns TwoWireQuery
         */
        public static fromObject(object: { [k: string]: any }): fk_atlas.TwoWireQuery;

        /**
         * Creates a plain object from a TwoWireQuery message. Also converts values to other types if specified.
         * @param message TwoWireQuery
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_atlas.TwoWireQuery, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this TwoWireQuery to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a Compensations. */
    interface ICompensations {

        /** Compensations temperature */
        temperature?: (number|null);
    }

    /** Represents a Compensations. */
    class Compensations implements ICompensations {

        /**
         * Constructs a new Compensations.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_atlas.ICompensations);

        /** Compensations temperature. */
        public temperature: number;

        /**
         * Creates a new Compensations instance using the specified properties.
         * @param [properties] Properties to set
         * @returns Compensations instance
         */
        public static create(properties?: fk_atlas.ICompensations): fk_atlas.Compensations;

        /**
         * Encodes the specified Compensations message. Does not implicitly {@link fk_atlas.Compensations.verify|verify} messages.
         * @param message Compensations message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_atlas.ICompensations, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Compensations message, length delimited. Does not implicitly {@link fk_atlas.Compensations.verify|verify} messages.
         * @param message Compensations message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_atlas.ICompensations, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a Compensations message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Compensations
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_atlas.Compensations;

        /**
         * Decodes a Compensations message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Compensations
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_atlas.Compensations;

        /**
         * Verifies a Compensations message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a Compensations message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns Compensations
         */
        public static fromObject(object: { [k: string]: any }): fk_atlas.Compensations;

        /**
         * Creates a plain object from a Compensations message. Also converts values to other types if specified.
         * @param message Compensations
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_atlas.Compensations, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Compensations to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a WireAtlasQuery. */
    interface IWireAtlasQuery {

        /** WireAtlasQuery type */
        type?: (fk_atlas.QueryType|null);

        /** WireAtlasQuery calibration */
        calibration?: (fk_atlas.IAtlasCalibrationCommand|null);

        /** WireAtlasQuery compensations */
        compensations?: (fk_atlas.ICompensations|null);

        /** WireAtlasQuery wire */
        wire?: (fk_atlas.ITwoWireQuery|null);
    }

    /** Represents a WireAtlasQuery. */
    class WireAtlasQuery implements IWireAtlasQuery {

        /**
         * Constructs a new WireAtlasQuery.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_atlas.IWireAtlasQuery);

        /** WireAtlasQuery type. */
        public type: fk_atlas.QueryType;

        /** WireAtlasQuery calibration. */
        public calibration?: (fk_atlas.IAtlasCalibrationCommand|null);

        /** WireAtlasQuery compensations. */
        public compensations?: (fk_atlas.ICompensations|null);

        /** WireAtlasQuery wire. */
        public wire?: (fk_atlas.ITwoWireQuery|null);

        /**
         * Creates a new WireAtlasQuery instance using the specified properties.
         * @param [properties] Properties to set
         * @returns WireAtlasQuery instance
         */
        public static create(properties?: fk_atlas.IWireAtlasQuery): fk_atlas.WireAtlasQuery;

        /**
         * Encodes the specified WireAtlasQuery message. Does not implicitly {@link fk_atlas.WireAtlasQuery.verify|verify} messages.
         * @param message WireAtlasQuery message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_atlas.IWireAtlasQuery, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified WireAtlasQuery message, length delimited. Does not implicitly {@link fk_atlas.WireAtlasQuery.verify|verify} messages.
         * @param message WireAtlasQuery message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_atlas.IWireAtlasQuery, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a WireAtlasQuery message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns WireAtlasQuery
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_atlas.WireAtlasQuery;

        /**
         * Decodes a WireAtlasQuery message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns WireAtlasQuery
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_atlas.WireAtlasQuery;

        /**
         * Verifies a WireAtlasQuery message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a WireAtlasQuery message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns WireAtlasQuery
         */
        public static fromObject(object: { [k: string]: any }): fk_atlas.WireAtlasQuery;

        /**
         * Creates a plain object from a WireAtlasQuery message. Also converts values to other types if specified.
         * @param message WireAtlasQuery
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_atlas.WireAtlasQuery, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this WireAtlasQuery to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of an AtlasCalibrationStatus. */
    interface IAtlasCalibrationStatus {

        /** AtlasCalibrationStatus type */
        type?: (fk_atlas.SensorType|null);

        /** AtlasCalibrationStatus time */
        time?: (number|null);

        /** AtlasCalibrationStatus raw */
        raw?: (number|null);

        /** AtlasCalibrationStatus temp */
        temp?: (fk_atlas.TempCalibrations|null);

        /** AtlasCalibrationStatus dissolvedOxygen */
        dissolvedOxygen?: (fk_atlas.DoCalibrations|null);

        /** AtlasCalibrationStatus ph */
        ph?: (fk_atlas.PhCalibrations|null);

        /** AtlasCalibrationStatus ec */
        ec?: (fk_atlas.EcCalibrations|null);

        /** AtlasCalibrationStatus orp */
        orp?: (fk_atlas.OrpCalibrations|null);

        /** AtlasCalibrationStatus configuration */
        configuration?: (Uint8Array|null);
    }

    /** Represents an AtlasCalibrationStatus. */
    class AtlasCalibrationStatus implements IAtlasCalibrationStatus {

        /**
         * Constructs a new AtlasCalibrationStatus.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_atlas.IAtlasCalibrationStatus);

        /** AtlasCalibrationStatus type. */
        public type: fk_atlas.SensorType;

        /** AtlasCalibrationStatus time. */
        public time: number;

        /** AtlasCalibrationStatus raw. */
        public raw: number;

        /** AtlasCalibrationStatus temp. */
        public temp: fk_atlas.TempCalibrations;

        /** AtlasCalibrationStatus dissolvedOxygen. */
        public dissolvedOxygen: fk_atlas.DoCalibrations;

        /** AtlasCalibrationStatus ph. */
        public ph: fk_atlas.PhCalibrations;

        /** AtlasCalibrationStatus ec. */
        public ec: fk_atlas.EcCalibrations;

        /** AtlasCalibrationStatus orp. */
        public orp: fk_atlas.OrpCalibrations;

        /** AtlasCalibrationStatus configuration. */
        public configuration: Uint8Array;

        /**
         * Creates a new AtlasCalibrationStatus instance using the specified properties.
         * @param [properties] Properties to set
         * @returns AtlasCalibrationStatus instance
         */
        public static create(properties?: fk_atlas.IAtlasCalibrationStatus): fk_atlas.AtlasCalibrationStatus;

        /**
         * Encodes the specified AtlasCalibrationStatus message. Does not implicitly {@link fk_atlas.AtlasCalibrationStatus.verify|verify} messages.
         * @param message AtlasCalibrationStatus message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_atlas.IAtlasCalibrationStatus, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified AtlasCalibrationStatus message, length delimited. Does not implicitly {@link fk_atlas.AtlasCalibrationStatus.verify|verify} messages.
         * @param message AtlasCalibrationStatus message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_atlas.IAtlasCalibrationStatus, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes an AtlasCalibrationStatus message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns AtlasCalibrationStatus
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_atlas.AtlasCalibrationStatus;

        /**
         * Decodes an AtlasCalibrationStatus message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns AtlasCalibrationStatus
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_atlas.AtlasCalibrationStatus;

        /**
         * Verifies an AtlasCalibrationStatus message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates an AtlasCalibrationStatus message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns AtlasCalibrationStatus
         */
        public static fromObject(object: { [k: string]: any }): fk_atlas.AtlasCalibrationStatus;

        /**
         * Creates a plain object from an AtlasCalibrationStatus message. Also converts values to other types if specified.
         * @param message AtlasCalibrationStatus
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_atlas.AtlasCalibrationStatus, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this AtlasCalibrationStatus to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a TwoWireReply. */
    interface ITwoWireReply {

        /** TwoWireReply data */
        data?: (Uint8Array|null);
    }

    /** Represents a TwoWireReply. */
    class TwoWireReply implements ITwoWireReply {

        /**
         * Constructs a new TwoWireReply.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_atlas.ITwoWireReply);

        /** TwoWireReply data. */
        public data: Uint8Array;

        /**
         * Creates a new TwoWireReply instance using the specified properties.
         * @param [properties] Properties to set
         * @returns TwoWireReply instance
         */
        public static create(properties?: fk_atlas.ITwoWireReply): fk_atlas.TwoWireReply;

        /**
         * Encodes the specified TwoWireReply message. Does not implicitly {@link fk_atlas.TwoWireReply.verify|verify} messages.
         * @param message TwoWireReply message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_atlas.ITwoWireReply, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified TwoWireReply message, length delimited. Does not implicitly {@link fk_atlas.TwoWireReply.verify|verify} messages.
         * @param message TwoWireReply message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_atlas.ITwoWireReply, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a TwoWireReply message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns TwoWireReply
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_atlas.TwoWireReply;

        /**
         * Decodes a TwoWireReply message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns TwoWireReply
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_atlas.TwoWireReply;

        /**
         * Verifies a TwoWireReply message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a TwoWireReply message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns TwoWireReply
         */
        public static fromObject(object: { [k: string]: any }): fk_atlas.TwoWireReply;

        /**
         * Creates a plain object from a TwoWireReply message. Also converts values to other types if specified.
         * @param message TwoWireReply
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_atlas.TwoWireReply, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this TwoWireReply to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** ReplyType enum. */
    enum ReplyType {
        REPLY_NONE = 0,
        REPLY_RETRY = 1,
        REPLY_ERROR = 2,
        REPLY_STATUS = 3,
        REPLY_COMMAND = 4
    }

    /** ErrorType enum. */
    enum ErrorType {
        NONE = 0,
        GENERAL = 1,
        UNEXPECTED = 2
    }

    /** Properties of an Error. */
    interface IError {

        /** Error type */
        type?: (fk_atlas.ErrorType|null);

        /** Error message */
        message?: (string|null);
    }

    /** Represents an Error. */
    class Error implements IError {

        /**
         * Constructs a new Error.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_atlas.IError);

        /** Error type. */
        public type: fk_atlas.ErrorType;

        /** Error message. */
        public message: string;

        /**
         * Creates a new Error instance using the specified properties.
         * @param [properties] Properties to set
         * @returns Error instance
         */
        public static create(properties?: fk_atlas.IError): fk_atlas.Error;

        /**
         * Encodes the specified Error message. Does not implicitly {@link fk_atlas.Error.verify|verify} messages.
         * @param message Error message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_atlas.IError, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified Error message, length delimited. Does not implicitly {@link fk_atlas.Error.verify|verify} messages.
         * @param message Error message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_atlas.IError, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes an Error message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns Error
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_atlas.Error;

        /**
         * Decodes an Error message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns Error
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_atlas.Error;

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
        public static fromObject(object: { [k: string]: any }): fk_atlas.Error;

        /**
         * Creates a plain object from an Error message. Also converts values to other types if specified.
         * @param message Error
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_atlas.Error, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Error to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a WireAtlasReply. */
    interface IWireAtlasReply {

        /** WireAtlasReply type */
        type?: (fk_atlas.ReplyType|null);

        /** WireAtlasReply errors */
        errors?: (fk_atlas.IError[]|null);

        /** WireAtlasReply calibration */
        calibration?: (fk_atlas.IAtlasCalibrationStatus|null);

        /** WireAtlasReply wire */
        wire?: (fk_atlas.ITwoWireReply|null);
    }

    /** Represents a WireAtlasReply. */
    class WireAtlasReply implements IWireAtlasReply {

        /**
         * Constructs a new WireAtlasReply.
         * @param [properties] Properties to set
         */
        constructor(properties?: fk_atlas.IWireAtlasReply);

        /** WireAtlasReply type. */
        public type: fk_atlas.ReplyType;

        /** WireAtlasReply errors. */
        public errors: fk_atlas.IError[];

        /** WireAtlasReply calibration. */
        public calibration?: (fk_atlas.IAtlasCalibrationStatus|null);

        /** WireAtlasReply wire. */
        public wire?: (fk_atlas.ITwoWireReply|null);

        /**
         * Creates a new WireAtlasReply instance using the specified properties.
         * @param [properties] Properties to set
         * @returns WireAtlasReply instance
         */
        public static create(properties?: fk_atlas.IWireAtlasReply): fk_atlas.WireAtlasReply;

        /**
         * Encodes the specified WireAtlasReply message. Does not implicitly {@link fk_atlas.WireAtlasReply.verify|verify} messages.
         * @param message WireAtlasReply message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encode(message: fk_atlas.IWireAtlasReply, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Encodes the specified WireAtlasReply message, length delimited. Does not implicitly {@link fk_atlas.WireAtlasReply.verify|verify} messages.
         * @param message WireAtlasReply message or plain object to encode
         * @param [writer] Writer to encode to
         * @returns Writer
         */
        public static encodeDelimited(message: fk_atlas.IWireAtlasReply, writer?: $protobuf.Writer): $protobuf.Writer;

        /**
         * Decodes a WireAtlasReply message from the specified reader or buffer.
         * @param reader Reader or buffer to decode from
         * @param [length] Message length if known beforehand
         * @returns WireAtlasReply
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): fk_atlas.WireAtlasReply;

        /**
         * Decodes a WireAtlasReply message from the specified reader or buffer, length delimited.
         * @param reader Reader or buffer to decode from
         * @returns WireAtlasReply
         * @throws {Error} If the payload is not a reader or valid buffer
         * @throws {$protobuf.util.ProtocolError} If required fields are missing
         */
        public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): fk_atlas.WireAtlasReply;

        /**
         * Verifies a WireAtlasReply message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a WireAtlasReply message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns WireAtlasReply
         */
        public static fromObject(object: { [k: string]: any }): fk_atlas.WireAtlasReply;

        /**
         * Creates a plain object from a WireAtlasReply message. Also converts values to other types if specified.
         * @param message WireAtlasReply
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: fk_atlas.WireAtlasReply, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this WireAtlasReply to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }
}
