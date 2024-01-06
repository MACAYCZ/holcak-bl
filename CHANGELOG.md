# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.1.0] - 2024-01-06
Support for booting unpartitioned media with the chainload protocol.

### Added
- [X] Function for calling BIOS interrupts from C
- [X] Reading sectors from disk using BIOS interrupts
    - [X] [Read disk parameters](https://stanislavs.org/helppc/int_13-8.html)
    - [X] [Check present extensions](https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=41h:_Check_Extensions_Present)
    - [X] [Read disk sectors](https://stanislavs.org/helppc/int_13-2.html)
    - [X] [Extended read disk sectors](https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=42h:_Extended_Read_Sectors_From_Drive)
- [X] Chainload protocol function

### Changed
- [X] Change the address of stage2 to 0x8000 since the chainload protocol is overwriting address 0x7C00
- [X] Stage1 no longer relocating to a lower address due to the change in the stage2 address
