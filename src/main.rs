#![allow(arithmetic_overflow)]



use simple_logger::SimpleLogger;
use std::str;
struct Cartridge{
    entry_point
}

fn main() {
    SimpleLogger::new().init().unwrap();
    let tetris = include_bytes!("tetris.gb");

    //load virtal cartriage infromation
    //https://gbdev.io/pandocs/The_Cartridge_Header.html
    let entry_point = &tetris[0x100..0x103];
    let title = &tetris[0x134..0x143];
    let cartridge_type = &tetris[0x147];
    let rom_size = &tetris[0x148];
    let ram_size = &tetris[0x149];
    let header_checksum = &tetris[0x14D];
    let game_code = &tetris[0x150..tetris.len()];



    let mut checksum = 0u8;
    for adress in 0x0134..0x014C{
        checksum = checksum - tetris[adress] - 1; // calculate the checksue
    }
    if header_checksum != &checksum{
        log::warn!(target: "checksum","Header Checksum is Invalid, Game will still launch, but expect errors.... good luck girly");
        log::info!(target: "checksum","Expected {:X?}, Got {:X?}",header_checksum,checksum);
    }

    log::info!(target: "cartridge","Loading Game {}",str::from_utf8(title).unwrap());
    



}