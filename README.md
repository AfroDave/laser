# laser

`laser` is a single-header library for loading [LAS](https://www.asprs.org/committee-general/laser-las-file-format-exchange-activities.html) files.
It currently supports a common subset of LAS 1.0, 1.1, 1.2 and 1.3.

## Usage

```C
#define LASER_IMPL
#include "laser.h"
```

The implementation part is only required *once* in a C/C++ file that includes
the header. Otherwise include only the header:

```C
#include "laser.h"
```

## Motivation

 - Zero dependencies.
 - Ease of integration.
 - Minimal API.
 - No internal allocation.

## Todo

 - Writing LAS files - maybe separate library?
 - VLR and EVLR support.
 - Endianess.
 - Additional more granuler interface.
