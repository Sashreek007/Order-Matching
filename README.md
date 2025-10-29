# Order-Matching

Practical C++ examples demonstrating Liquibook order matching: buy/sell orders, trade execution, and market depth tracking.

## Quick Start

### 1. Clone this repo
```bash
git clone https://github.com/Sashreek007/Order-Matching.git
cd Order-Matching
```

### 2. Clone Liquibook dependency
```bash
git clone https://github.com/enewhuis/liquibook.git
```

### 3. Build
```bash
mkdir -p build && cd build
cmake ..
make
```

### 4. Run
```bash
./order
```

## Structure
```
.
├── liquibook/     # Required: Order matching engine
├── src/           # Example files
└── build/         # Build output (./order executable)
```

## Credits

Built with [Liquibook](https://github.com/enewhuis/liquibook) 
