# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## Unreleased
Support for booting unpartitioned media with the chainload protocol.

### Added
- [ ] Switching between 16-bit and 32-bit mode
    - [ ] GAS macros for entering and leaving 16-bit mode
    - [ ] Function for calling BIOS interrupts from C
- [ ] Reading sectors from disk using BIOS interrupts
    - [ ] (https://stanislavs.org/helppc/int_13-8.html)[Read disk parameters]
    - [ ] (https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=41h:_Check_Extensions_Present)[Check present extensions]
    - [ ] (https://stanislavs.org/helppc/int_13-2.html)[Read disk sectors]
    - [ ] (https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=42h:_Extended_Read_Sectors_From_Drive)[Read extended disk sectors]
- [ ] Chainload protocol function
