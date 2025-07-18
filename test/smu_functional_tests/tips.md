## Read/Write Testcases
| Test Case           | Address | Data Value      | Operation |
|---------------------|---------|-----------------|-----------|
| Minimum             | 0x10    | 0x00000000      | Write     |
| Minimum             | 0x10    | (expect: 0x0)   | Read      |
| Maximum             | 0x20    | 0xFFFFFFFF      | Write     |
| Maximum             | 0x20    | (expect: 0xFFFFFFFF) | Read |
| Single bit set low  | 0x30    | 0x00000001      | Write     |
| Single bit set low  | 0x30    | (expect: 0x1)   | Read      |
| Single bit set high | 0x40    | 0x80000000      | Write     |
| Single bit set high | 0x40    | (expect: 0x80000000) | Read |
| Alternating 1010... | 0x50    | 0xAAAAAAAA      | Write     |
| Alternating 1010... | 0x50    | (expect: 0xAAAAAAAA) | Read |
| Alternating 0101... | 0x60    | 0x55555555      | Write     |
| Alternating 0101... | 0x60    | (expect: 0x55555555) | Read |
| Only high byte      | 0x70    | 0xFF000000      | Write     |
| Only high byte      | 0x70    | (expect: 0xFF000000) | Read |
| Only low byte       | 0x80    | 0x000000FF      | Write     |
| Only low byte       | 0x80    | (expect: 0x000000FF) | Read |
| Equals to key       | 0x90    | 0xCAFEBABE      | Write     |
| Equals to key       | 0x90    | (expect: 0xCAFEBABE) | Read |
| Equals to address   | 0xA0    | 0xA0            | Write     |
| Equals to address   | 0xA0    | (expect: 0xA0)  | Read      |