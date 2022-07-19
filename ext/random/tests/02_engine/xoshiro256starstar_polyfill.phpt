--TEST--
Random: Engine: Xoshiro256StarStar: Polyfill
--SKIPIF--
<?php if (PHP_INT_SIZE != 8) die("skip: 64-bit only"); ?>
--FILE--
<?php

function rotl(int $x, int $k): int
{
    return (($x << $k)) | (($x >> (64 - $k)) & ~(~0 << $k));
}

function unsigned_mult(int $a, int $b): int
{
    $a4 = ($a & 0xffff);
    $a3 = (($a >> 16) & 0xffff);
    $a2 = (($a >> 32) & 0xffff);
    $a1 = (($a >> 48) & 0xffff);
    $b4 = ($b & 0xffff);
    $b3 = (($b >> 16) & 0xffff);
    $b2 = (($b >> 32) & 0xffff);
    $b1 = (($b >> 48) & 0xffff);

    $r4 = ($a4 * $b4);
    $r3 = ($a3 * $b4 + $a4 * $b3) + ($r4 >> 16);
    $r2 = ($a2 * $b4 + $a3 * $b3 + $a4 * $b2) + ($r3 >> 16);
    $r1 = ($a1 * $b4 + $a2 * $b3 + $a3 * $b2 + $a4 * $b1) + ($r2 >> 16);

    $r1 = $r1 & 0xffff;
    $r2 = $r2 & 0xffff;
    $r3 = $r3 & 0xffff;
    $r4 = $r4 & 0xffff;

    return ($r1 << 48) + ($r2 << 32) + ($r3 << 16) + $r4;
}

function unsigned_add(int $a, int $b): int
{
    $lowerA = ($a & 0xffffffff);
    $lowerB = ($b & 0xffffffff);
    $upperA = (($a >> 32) & 0xffffffff);
    $upperB = (($b >> 32) & 0xffffffff);

    $lower = $lowerA + $lowerB;
    $upper = $upperA + $upperB + (($lower >> 32) & 0xffffffff);

    $lower = $lower & 0xffffffff;
    $upper = $upper & 0xffffffff;

    return ($upper << 32) + $lower;
}

final class SplitMix64 implements \Random\Engine
{
    public function __construct(private int $x) {}

    public function generate(): string
    {
        $this->x = unsigned_add($this->x, (0x9e3779b9 << 32) + 0x7f4a7c15);
        $z = $this->x;
        $z = unsigned_mult($z ^ (($z >> 30) & ~(~0 << (64 - 30))), (0xbf58476d << 32) + 0x1ce4e5b9);
        $z = unsigned_mult($z ^ (($z >> 27) & ~(~0 << (64 - 27))), (0x94d049bb << 32) + 0x133111eb);

        return pack('P', $z ^ (($z >> 31) & ~(~0 << (64 - 31))));
    }
}


final class Xoshiro256StarStar implements \Random\Engine
{
    private int $s0, $s1, $s2, $s3;

    private const JUMP = [
        (0x180ec6d3 << 32) + 0x3cfd0aba,
        (0xd5a61266 << 32) + 0xf0c9392c,
        (0xa9582618 << 32) + 0xe03fc9aa,
        (0x39abdc45 << 32) + 0x29b1661c,
    ];

    private const LONG_JUMP = [
        (0x76e15d3e << 32) + 0xfefdcbbf,
        (0xc5004e44 << 32) + 0x1c522fb3,
        (0x77710069 << 32) + 0x854ee241,
        (0x39109bb0 << 32) + 0x2acbe635,
    ];

    public function __construct(string|int|null $seed = null)
    {
        if ($seed === null) {
            $seed = random_bytes(32);
        } else if (is_int($seed)) {
            $splitMix = new SplitMix64($seed);
            $seed = "";
            $seed .= $splitMix->generate();
            $seed .= $splitMix->generate();
            $seed .= $splitMix->generate();
            $seed .= $splitMix->generate();
        }

        if (strlen($seed) !== 32) {
            throw new ValueError('state strings must be 32 bytes');
        }

        [, $this->s0, $this->s1, $this->s2, $this->s3] = unpack('P4', $seed);
    }

    public function jump(): void
    {
        $this->jumpImpl(self::JUMP);
    }

    public function jumpLong(): void
    {
        $this->jumpImpl(self::LONG_JUMP);
    }

    private function jumpImpl(array $constants): void
    {
        $s0 = $s1 = $s2 = $s3 = 0;

        foreach ($constants as $jump) {
            for ($b = 0; $b < 64; $b++) {
                if ($jump & (1 << $b)) {
                    $s0 ^= $this->s0;
                    $s1 ^= $this->s1;
                    $s2 ^= $this->s2;
                    $s3 ^= $this->s3;
                }

                $this->generate();
            }
        }

        $this->s0 = $s0;
        $this->s1 = $s1;
        $this->s2 = $s2;
        $this->s3 = $s3;
    }

    public function generate(): string
    {
        $times5 = unsigned_add(($this->s1 << 2), $this->s1);
        $rot7 = rotl($times5, 7);
        $times9 = unsigned_add(($rot7 << 3), $rot7);
        $result = $times9;

        $t = $this->s1 << 17;

        $this->s2 ^= $this->s0;
        $this->s3 ^= $this->s1;
        $this->s1 ^= $this->s2;
        $this->s0 ^= $this->s3;

        $this->s2 ^= $t;

        $this->s3 = rotl($this->s3, 45);

        return pack('P', $result);
    }

    public function __debugInfo()
    {
        return [
            '__states' => [
                bin2hex(pack('P', $this->s0)),
                bin2hex(pack('P', $this->s1)),
                bin2hex(pack('P', $this->s2)),
                bin2hex(pack('P', $this->s3)),
            ],
        ];
    }

    public function __serialize(): array
    {
        return [
            [],
            [
                bin2hex(pack('P', $this->s0)),
                bin2hex(pack('P', $this->s1)),
                bin2hex(pack('P', $this->s2)),
                bin2hex(pack('P', $this->s3)),
            ],
        ];
    }

    public function __unserialize(array $data): void
    {
        [$dummy, [$s0, $s1, $s2, $s3]] = $data;

        $this->s0 = unpack('P', hex2bin($s0))[1];
        $this->s1 = unpack('P', hex2bin($s1))[1];
        $this->s2 = unpack('P', hex2bin($s2))[1];
        $this->s3 = unpack('P', hex2bin($s3))[1];
    }
}


$native = new \Random\Engine\Xoshiro256StarStar(1234);
$user = new Xoshiro256StarStar(1234);

for ($i = 0; $i < 5000; $i++) {
    if ($native->generate() !== $user->generate()) {
        die("failure: a-{$i}\n");
    }
}

$native->jump();
$native->jumpLong();
$user->jump();
$user->jumpLong();

for ($i = 0; $i < 5000; $i++) {
    if ($native->generate() !== $user->generate()) {
        die("failure: b-{$i}\n");
    }
}

echo bin2hex($native->generate()), PHP_EOL;
echo bin2hex($user->generate());
?>
--EXPECT--
1f197e9ca7969123
1f197e9ca7969123
