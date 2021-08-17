# nRF24LE1 Programmer

Arduino sketches and Perl script to program flash on Nordic nRF24LE1 wireless SOC

More information can be found in [nRF24LE1_Starter_Kit](https://github.com/anabolyc/nRF24LE1_Starter_Kit) repo.

## How to build 

Open `./Programmer` folder in vscode and run `Build` task. Run `Upload` task to flash firmware.

## Hardware

You can make it on the breakboard or create a dedicated tool, like I did

In the first scenario connection would look like this

```
 Pin-Mapping:
 Arduino	         24Pin		 32Pin		48Pin
 D07 (RXD)	    12 P0.6		10 P0.4		15 P1.1
 D08 (PROG)	     5 PROG		 6 PROG		10 PROG
 D09 (RESET)	  13 RESET	19 RESET	30 RESET
 D10 (FCSN,TXD)	11 P0.5		15 P1.1		22 P2.0
 D11 (FMOSI)	   9 P0.3		13 P0.7		19 P1.5
 D12 (FMISO)	  10 P0.4		14 P1.0		20 P1.6
 D13 (FSCK)	     8 P0.2		11 P0.5 	16 P1.2

```

In case you want to go second way, schematic can be foung in the repo, also hw project is [open sourced](https://oshwlab.com/andrey.mal/2104-nrf24l01-dev-board_copy_copy). 

Hopefully board will be available to buy soon.