--TEST--
Bug #39297 (Memory corruption because of indirect modification of overloaded array)
--FILE--
<?php
function compareByRef(&$first, &$second) {
    return $first === $second;
}

class MyTree implements ArrayAccess {
    public $parent;
    public $children = array();

    public function offsetExists($offset): bool {
    }

    public function offsetUnset($offset): void {
    }

    public function offsetSet($offset, $value): void {
        echo "offsetSet()\n";
        $canonicalName = strtolower($offset);
        $this->children[$canonicalName] = $value;
        $value->parent = $this;
    }

    public function offsetGet($offset): mixed {
        echo "offsetGet()\n";
        $canonicalName = strtolower($offset);
        return $this->children[$canonicalName];
    }

}

$id = 'Test';

$root = new MyTree();
$child = new MyTree();
$root[$id] = $child;

var_dump(compareByRef($root[$id], $child));
?>
--EXPECT--
offsetSet()
offsetGet()
bool(true)
