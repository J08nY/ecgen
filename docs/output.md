# Output

There only output format currently supported in ecgen is JSON.

## JSON

Self-explanatory format. The curve dictionaries are enclosed in an array as you might generate more curves at once (`-c / --count`). The only interesting thing is that while generating invalid curves for a curve with the `-i / --invalid` option, the original curve is output first.

#### Example - prime field

	[
	{
	    "field": {
	        "p": "0xfa4101a601c3088f"
	    },
	    "a": "0x223fcc1306c21406",
	    "b": "0x4114b86128071651",
	    "order": "0xfa4101a5b65111b6",
	    "subgroups": [
	        {
	            "x": "0x41b71e83794c614a",
	            "y": "0xc43c15e114f16ba1",
	            "order": "0xfa4101a5b65111b6",
	            "cofactor": "0x1",
                "points": [
                    {
                        "x": "0x7427f55c615d0c60",
                        "y": "0x0000000000000000",
                        "order": "0x2"
                    },
                    {
                        "x": "0x357cbff05dacc66c",
                        "y": "0x4f5d5e523a38a35a",
                        "order": "0xd"
                    },
                    {
                        "x": "0x4ef832dbb406dac6",
                        "y": "0x684eb7a227fc23c3",
                        "order": "0x8b60007"
                    },
                    {
                        "x": "0xd82bdb55db6bb3ef",
                        "y": "0xc103b77986a1c2e3",
                        "order": "0x11ade0181"
                    }
                ]
	        }
	    ]
	}]

#### Example - binary field

	[
	{
	    "field": {
	        "m": "0x40",
	        "e1": "0x4",
	        "e2": "0x3",
	        "e3": "0x1"
	    },
	    "a": "0x3869c3f4bbeb4087",
	    "b": "0xba60557c283d94cd",
	    "order": "0xffffffffea115f5a",
	    "subgroups": [
	        {
	            "x": "0x4dcc3b35abcc13e4",
	            "y": "0x4380aa919232c5b1",
	            "order": "0xffffffffea115f5a",
	            "cofactor": "0x1",
	            "points": [
                    {
                        "x": "0x0000000000000000",
                        "y": "0x0000000000000000",
                        "order": "0x2"
                    },
                    {
                        "x": "0xde23b127982c9db7",
                        "y": "0x1c63aa4b52327ca0",
                        "order": "0x13"
                    },
                    {
                        "x": "0xcb00f2cf13a8bab9",
                        "y": "0x192a159ad11df21b",
                        "order": "0xf1"
                    },
                    {
                        "x": "0x3b9cd618b48e9a73",
                        "y": "0xdacc98d7c32d8c0b",
                        "order": "0x649"
                    },
                    {
                        "x": "0xdbdf22552a10abbe",
                        "y": "0x0dd6e81435a11566",
                        "order": "0x2f94b"
                    },
                    {
                        "x": "0x963ae5e637a9917a",
                        "y": "0x1974294368f7f6d1",
                        "order": "0x6203c5"
                    }
                ]
	        }
	    ]
	}]
