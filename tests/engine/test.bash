SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

BINARY="$SCRIPT_DIR/../out/main";

if [ ! -f "$BINARY" ]; then
	echo "Binary not found. Run 'make build' first."
	exit 1
fi

function test_main() {
	# Test the binary
	$BINARY
	assert_equals 5 $? "correct exit code"
}