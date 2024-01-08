# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
Minor fixes and changes for the previous version.

### Fixed
- [X] Fixed values of segment registers in 32-bit mode
    - [X] In function `_start`
    - [X] In function `x86_16_int`

### Changed
- [ ] Function `x86_16_int`
    - [ ] Returns output registers instead of taking them as an argument
    - [ ] Takes copy of input registers instead of taking them as a pointer
- [X] Split function `chainload` to `chainload_load` and `chainload_jump`
- [X] No longer rely on `.zero` in stage1, instead use `.section`

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
- [X] Change the address of stage2 to 0x8000 since the chainload protocol is using 512 bytes at address 0x7C00
- [X] Remove relocation of stage1 to a lower address due to the change in the stage2 address
