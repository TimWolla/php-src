<?php

/**
 * @generate-class-entries
 * @generate-c-enums
 */

namespace Encoding {
    class EncodingError extends \Error
    {
    }

    class EncodingException extends \Exception
    {
    }

    class UnableToDecodeException extends EncodingException
    {
    }

    class UnableToEncodeException extends EncodingException
    {
    }

    enum Base16
    {
        case Upper;
        case Lower;
    }

    enum Base32
    {
        case Ascii;
        case Hex;
        case Crockford;
        case Z;
    }

    enum Base58
    {
        case Bitcoin;
        case Flickr;
    }

    enum Base64
    {
        case Standard;
        case UrlSafe;
        case Imap;
    }

    enum Base85
    {
        case Adobe;
        case Z85;
        case Git;
    }

    enum PaddingMode
    {
        case VariantControlled;
        case StripPadding;
        case PreservePadding;
    }

    enum DecodingMode
    {
        case Forgiving;
        case Strict;
    }

    enum TimingMode
    {
        case Variable;
        case Constant;
    }
}
