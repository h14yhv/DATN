/*----------------------------------------------------------------------------
 *      Name:    DEMO.H
 *      Purpose: USB HID Demo Definitions
 *      Version: V1.10
 *----------------------------------------------------------------------------
 *      This file is part of the uVision/ARM development tools.
 *      Copyright (c) 2005-2006 Keil Software. All rights reserved.
 *      This software may only be used under the terms of a valid, current,
 *      end user licence from KEIL for a compatible version of KEIL software
 *      development tools. Nothing else gives you the right to use it.
 *---------------------------------------------------------------------------*/

/* HID Demo Variables */
extern BYTE InReport;
extern BYTE OutReport;

/* HID Demo Functions */
extern void GetInReport  (void);
extern void SetOutReport (void);
