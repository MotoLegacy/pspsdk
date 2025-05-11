/*
 * PSP Software Development Kit - https://github.com/pspdev
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * pspctrl.h - Prototypes for the sceCtrl library.
 *
 * Copyright (c) 2005 Marcus R. Brown <mrbrown@ocgnet.org>
 * Copyright (c) 2005 James Forshaw <tyranid@gmail.com>
 * Copyright (c) 2005 John Kelley <ps2dev@kelley.ca>
 *
 */

/* Note: Some of the structures, types, and definitions in this file were
   extrapolated from symbolic debugging information found in the Japanese
   version of Puzzle Bobble. */

#ifndef __CTRL_H__
#define __CTRL_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup Ctrl Controller Kernel Library
  * This module contains the imports for controllers (buttons, pad).
  */

/** @addtogroup Ctrl Controller Kernel Library */
/**@{*/

// MARK: Base

/**
 * @brief Enumeration representing digital controller button flags.
 *
 * Each flag corresponds to a different button and can be used to extract button states from ::SceCtrlData and ::SceCtrlLatch structures.
 * Flags can be combined using bitwise OR operation to check for mutliple key states at once.
 *
 * @note Some button states are available only in kernel mode.
 *
 * @see ::SceCtrlData
 * @see ::SceCtrlLatch
 */
enum PspCtrlButtons
{
	/** Select button. */
	PSP_CTRL_SELECT     = 0x000001,
	/** Start button. */
	PSP_CTRL_START      = 0x000008,
	/** Up D-Pad button. */
	PSP_CTRL_UP         = 0x000010,
	/** Right D-Pad button. */
	PSP_CTRL_RIGHT      = 0x000020,
	/** Down D-Pad button. */
	PSP_CTRL_DOWN      	= 0x000040,
	/** Left D-Pad button. */
	PSP_CTRL_LEFT      	= 0x000080,
	/** Left trigger. */
	PSP_CTRL_LTRIGGER   = 0x000100,
	/** Right trigger. */
	PSP_CTRL_RTRIGGER   = 0x000200,
	/** Triangle button. */
	PSP_CTRL_TRIANGLE   = 0x001000,
	/** Circle button. */
	PSP_CTRL_CIRCLE     = 0x002000,
	/** Cross button. */
	PSP_CTRL_CROSS      = 0x004000,
	/** Square button. */
	PSP_CTRL_SQUARE     = 0x008000,
	/**
	 * Kernel mode: Home button state.
	 * User mode: Exit dialog visible.
	 */
	PSP_CTRL_HOME       = 0x010000,
	/** Hold button. */
	PSP_CTRL_HOLD       = 0x020000,
	/** Music note button - kernel mode only.*/
	PSP_CTRL_NOTE       = 0x800000,
	/** Screen button - kernel mode only.*/
	PSP_CTRL_SCREEN     = 0x400000,
	/** Volume up button - kernel mode only.*/
	PSP_CTRL_VOLUP      = 0x100000,
	/** Volume down button - kernel mode only.*/
	PSP_CTRL_VOLDOWN    = 0x200000,
	/** Wlan switch up - kernel mode only.*/
	PSP_CTRL_WLAN_UP    = 0x040000,
	/** Remote hold position - kernel mode only.*/
	PSP_CTRL_REMOTE     = 0x080000,
	/** Disc present - kernel mode only.*/
	PSP_CTRL_DISC       = 0x1000000,
	/** Memory stick present - kernel mode only.*/
	PSP_CTRL_MS         = 0x2000000,
};

/**
 * @brief Controller mode.
 *
 * Specifies if analog data should be included in ::SceCtrlData.
 *
 * @see ::sceCtrlSetSamplingMode()
 * @see ::sceCtrlGetSamplingMode()
 * @see ::SceCtrlData
 */
enum PspCtrlMode
{
	/* Digital. */
	PSP_CTRL_MODE_DIGITAL = 0,
	/* Analog. */
	PSP_CTRL_MODE_ANALOG = 1,
};

/**
 * @brief Controller data.
 *
 * Contains current button and axis state.
 *
 * @note Axis state is present only in ::PSP_CTRL_MODE_ANALOG controller mode.
 *
 * @see ::sceCtrlPeekBufferPositive()
 * @see ::sceCtrlPeekBufferNegative()
 * @see ::sceCtrlReadBufferPositive()
 * @see ::sceCtrlReadBufferNegative()
 * @see ::PspCtrlMode
 */
typedef struct SceCtrlData {
	/** Current read frame. */
	unsigned int 	TimeStamp;
	/**
	 * Buttons in pressed state.
	 *
	 * Mask the value with one or more ::PspCtrlButtons flags to access specific buttons.
	 */
	unsigned int 	Buttons;
	/** X-axis value of the Analog Stick.*/
	unsigned char 	Lx;
	/** Y-axis value of the Analog Stick.*/
	unsigned char 	Ly;
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	union {
		struct {
#endif // DOXYGEN_SHOULD_SKIP_THIS
			/** X-axis value of the right Analog Stick, valid when using DualShock 3 on PSP GO, 
			 * 	a PS VITA system, through hardware/software hacking, or system emulation. */
			unsigned char Rx;
			/** Y-axis value of the right Analog Stick, valid when using DualShock 3 on PSP GO, 
			 * 	a PS VITA system, through hardware/software hacking, or system emulation. */
			unsigned char Ry;
			/** Reserved bytes unused by the firmware. */
			unsigned char Reserved[4];
		};
		/** 
		 * @private
		 * Reserved bytes. This is deprecated with the implementation of Rx and Ry and only here for backwards compatibility. 
		 */
		unsigned char Rsrv[6];
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	};
#endif // DOXYGEN_SHOULD_SKIP_THIS
} SceCtrlData;

/** 
 * @brief Controller latch data.
 * 
 * Contains information about button state changes between two controller service sampling cycles.
 * With each sampling cycle, the controller service compares the new pressed & releasedbutton states
 * with the previously collected pressed button states. This comparison will result in the following possible
 * states for each button:
 *
 * - **Make** - The button has just been pressed with its prior state being the released state.
 * Transition from 'released' state to 'pressed' state.
 * - **Press** - The button is currently in the 'pressed' state.
 * - **Break** - The button has just been released with its prior state being the 'pressed' state.
 * Transition from 'pressed' state to 'release' state.
 * - **Release** - The button is currently in the 'released' state.
 *
 * It is possible for a button to (briefly) be in two states at the same time. Valid combinations are as follows:
 * - **Make** & **Press**
 * - **Break** & **Release**
 *
 * In other words, if a button is in the **Make** state, then it is also in the **Press** state. However, this is not the case
 * for the inverse. A button in the **Press** state does not need to be in the **Make** state.
 *
 * Mask the values with one or more ::PspCtrlButtons flags to access specific buttons.
 *
 * These comparison results are stored internally as latch data and can be retrieved using the APIs ::sceCtrlPeekLatch() and
 * ::sceCtrlReadLatch().
 *
 * @remark The same can be accomplished by using the different sceCtrl[Read/Peek]Buffer[Positive/Negative]() APIs
 * and comparing the currently collected button sampling data with the previously collected one.
 *
 * @see ::PspCtrlButtons
 * @see ::sceCtrlPeekLatch()
 * @see ::sceCtrlReadLatch()
 */
typedef struct SceCtrlLatch {
	/** Button transitioned to pressed state. */
	unsigned int 	uiMake;
	/** Button transitioned to released state. */
	unsigned int 	uiBreak;
	/** Button is in the pressed state. */
	unsigned int 	uiPress;
	/** Button is in the released state. */
	unsigned int 	uiRelease;
} SceCtrlLatch;

/**
 * Set the controller cycle setting.
 *
 * @param cycle - Cycle.  Normally set to 0.
 *
 * @return The previous cycle setting.
 */
int sceCtrlSetSamplingCycle(int cycle);

/**
 * Get the controller current cycle setting.
 *
 * @param pcycle - Return value.
 *
 * @return 0.
 */
int sceCtrlGetSamplingCycle(int *pcycle);

/**
 * Set the controller mode.
 *
 * @param mode - One of ::PspCtrlMode. If this is ::PSP_CTRL_MODE_DIGITAL, no data about the analog stick
 * will be present in the SceCtrlData struct read by SceCtrlReadBuffer.
 *
 * @return The previous mode.
 */
int sceCtrlSetSamplingMode(int mode);

/**
 * Get the current controller mode.
 *
 * @param pmode - Return value.
 *
 * @return 0.
 */
int sceCtrlGetSamplingMode(int *pmode);

/**
 * @brief Read latest controller data from the controller service.
 *
 * Controller data contains current button and axis state.
 *
 * @note Axis state is present only in ::PSP_CTRL_MODE_ANALOG controller mode.
 *
 * @param pad_data - A pointer to ::SceCtrlData structure that receives controller data.
 * @param count - Number of ::SceCtrlData structures to read.
 *
 * @see ::SceCtrlData
 * @see ::sceCtrlPeekBufferNegative()
 * @see ::sceCtrlReadBufferPositive()
 */
int sceCtrlPeekBufferPositive(SceCtrlData *pad_data, int count);

int sceCtrlPeekBufferNegative(SceCtrlData *pad_data, int count);

/**
 * @brief Read new controller data from the controller service.
 *
 * Controller data contains current button and axis state.
 *
 * **Example:**
 * @code
 * SceCtrlData pad;
 * sceCtrlSetSamplingCycle(0);
 * sceCtrlSetSamplingMode(1);
 * sceCtrlReadBufferPositive(&pad, 1);
 * // Do something with the read controller data
 * @endcode
 *
 * @note Axis state is present only in ::PSP_CTRL_MODE_ANALOG controller mode.
 *
 * @warning Controller data is collected once every controller sampling cycle.
 * If controller data was already read during a cycle, trying to read it again
 * will block the execution until the next one.
 *
 * @param pad_data - A pointer to ::SceCtrlData structure that receives controller data.
 * @param count - Number of ::SceCtrlData structures to read.
 *
 * @see ::SceCtrlData
 * @see ::sceCtrlReadBufferNegative()
 * @see ::sceCtrlPeekBufferPositive()
 */
int sceCtrlReadBufferPositive(SceCtrlData *pad_data, int count);

int sceCtrlReadBufferNegative(SceCtrlData *pad_data, int count);

/**
 * @brief Read latest latch data from the controller service.
 *
 * Latch data contains information about button state changes between two controller service sampling cycles.
 *
 * @param latch_data A pointer to ::SceCtrlLatch structure that receives latch data.
 *
 * @return On success, the number of times the controller service performed sampling since the last time
 * ::sceCtrlReadLatch() was called.
 * @return < 0 on error.
 * 
 * @see ::SceCtrlLatch
 * @see ::sceCtrlReadLatch()
 */
int sceCtrlPeekLatch(SceCtrlLatch *latch_data);

/**
 * @brief Read new latch data from the controller service.
 * 
 * Latch data contains information about button state changes between two controller service sampling cycles.
 *
 * **Example:**
 * @code
 * SceCtrlLatch latchData;
 *
 * while (1) {
 *     // Obtain latch data
 *     sceCtrlReadLatch(&latchData);
 *
 *     if (latchData.uiMake & PSP_CTRL_CROSS)
 *     {
 *         // The Cross button has just been pressed (transition from 'released' state to 'pressed' state)
 *     }
 *
 *     if (latchData.uiPress & PSP_CTRL_SQUARE)
 *     {
 *         // The Square button is currently in the 'pressed' state
 *     }
 *
 *    if (latchData.uiBreak & PSP_CTRL_TRIANGLE)
 *    {
 *        // The Triangle button has just been released (transition from 'pressed' state to 'released' state)
 *    }
 *
 *    if (latchData.uiRelease & PSP_CTRL_CIRCLE)
 *    {
 *        // The Circle button is currently in the 'released' state
 *    }
 *
 *    // As we clear the internal latch data with the ReadLatch() call, we can explicitly wait for the VBLANK interval
 *    // to give the controller service the time it needs to collect new latch data again. This guarantees the next call
 *    // to sceCtrlReadLatch() will return collected data again.
 *    //
 *    // Note: The sceCtrlReadBuffer*() APIs are implicitly waiting for a VBLANK interval if necessary.
 *    sceDisplayWaitVBlank();
 * }
 * @endcode
 * 
 * @warning Latch data is produced once every controller sampling cycle. If latch data was already read
 * during a cycle, trying to read it again will block the execution until the next one.
 * 
 * @param latch_data A pointer to ::SceCtrlLatch structure that receives latch data.
 * 
 * @return On success, the number of times the controller service performed sampling since the last time
 * ::sceCtrlReadLatch() was called.
 * @return < 0 on error.
 * 
 * @see ::SceCtrlLatch
 * @see ::sceCtrlPeekLatch()
 */
int sceCtrlReadLatch(SceCtrlLatch *latch_data);

/**
 * Set analog threshold relating to the idle timer.
 *
 * @param idlereset - Movement needed by the analog to reset the idle timer.
 * @param idleback - Movement needed by the analog to bring the PSP back from an idle state.
 *
 * Set to -1 for analog to not cancel idle timer.
 * Set to 0 for idle timer to be cancelled even if the analog is not moved.
 * Set between 1 - 128 to specify the movement on either axis needed by the analog to fire the event.
 *
 * @return < 0 on error.
 */
int sceCtrlSetIdleCancelThreshold(int idlereset, int idleback);

/**
 * Get the idle threshold values.
 *
 * @param idlerest - Movement needed by the analog to reset the idle timer.
 * @param idleback - Movement needed by the analog to bring the PSP back from an idle state.
 *
 * @return < 0 on error.
 */
int sceCtrlGetIdleCancelThreshold(int *idlerest, int *idleback);

// MARK: Extended (DualShock 3) functionality

/** Specifies the type of input data to be obtained. */
enum PspCtrlPort {
    /* Input is only obtained from the PSP. */
    PSP_CTRL_PORT_PSP = 0,
    /* Input is obtained from the PSP and a connected DualShock 3 controller. */
    PSP_CTRL_PORT_DS3 = 1,
    /* Input is obtained from the PSP and an unknown connected external device. */
    PSP_CTRL_PORT_UNKNOWN_2 = 2
};

/**
 * @brief Extended Controller data. Designed to accomodate features from
 * the SIXAXIS/DualShock 3 controller, as supported in PSP GO.
 *
 * @note It is also compatible with reading the standard PSP buttons, though
 * is interfaced with a bit differently than `SceCtrlData`.
 *
 * @see ::sceCtrlPeekBufferPositive2()
 * @see ::sceCtrlPeekBufferNegative2()
 * @see ::sceCtrlReadBufferPositive2()
 * @see ::sceCtrlReadBufferNegative2()
 * @see ::PspCtrlMode
 */
typedef struct SceCtrlData2 {
	/** Current read frame. */
	unsigned int 			TimeStamp;
	/**
	 * Buttons in pressed state.
	 *
	 * Mask the value with one or more ::PspCtrlButtons flags to access specific buttons.
	 */
	unsigned int 			Buttons;
	/** X-axis value of the Analog Stick.*/
	unsigned char 			Lx;
	/** Y-axis value of the Analog Stick.*/
	unsigned char 			Ly;
	/** X-axis value of the right Analog Stick. */
	unsigned char 			Rx;
	/** Y-axis value of the right Analog Stick. */
	unsigned char 			Ry;
	/**
	* @private
	* Reserved bytes unused by the firmware.
	*/
	unsigned char 			Reserved[4];
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	union {
		struct {
#endif // DOXYGEN_SHOULD_SKIP_THIS
			/** Current pressure applied to the right Directional Pad button. */
			unsigned char	DPadSensRight;
			/**
			 * @private
			 * Unused, for byte indexing u32.
			 */
			unsigned char	unused1;
			/** Current pressure applied to the left Directional Pad button. */
			unsigned char 	DPadSensLeft;
			/**
			 * @private
			 * Unused, for byte indexing u32.
			 */
			unsigned char 	unused2;
#ifndef DOXYGEN_SHOULD_SKIP_THIS
		};

		/**
		 * @private
		 * Left/Right Directional Pad pressure storage before byte indexing.
		 */
		unsigned int 	DPadSensRL;
	};
#endif // DOXYGEN_SHOULD_SKIP_THIS
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	union {
		struct {
#endif // DOXYGEN_SHOULD_SKIP_THIS
			/** Current pressure applied to the up Directional Pad button. */
			unsigned char	DPadSensUp;
			/**
			 * @private
			 * Unused, for byte indexing u32.
			 */
			unsigned char	unused3;
			/** Current pressure applied to the down Directional Pad button. */
			unsigned char 	DPadSensDown;
			/**
			 * @private
			 * Unused, for byte indexing u32.
			 */
			unsigned char 	unused4;
#ifndef DOXYGEN_SHOULD_SKIP_THIS
		};

		/**
		 * @private
		 * Up/Down Directional Pad pressure storage before byte indexing.
		 */
		unsigned int 	DPadSensUD;
	};
#endif // DOXYGEN_SHOULD_SKIP_THIS
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	union {
		struct {
#endif // DOXYGEN_SHOULD_SKIP_THIS
			/** Current pressure applied to the Triangle Face button. */
			unsigned char	FaceSensTriangle;
			/**
			 * @private
			 * Unused, for byte indexing u32.
			 */
			unsigned char	unused5;
			/** Current pressure applied to the Circle Face button. */
			unsigned char 	FaceSensCircle;
			/**
			 * @private
			 * Unused, for byte indexing u32.
			 */
			unsigned char 	unused6;
#ifndef DOXYGEN_SHOULD_SKIP_THIS
		};

		/**
		 * @private
		 * Triangle/Circle Face Button pressure storage before byte indexing.
		 */
		unsigned int 	FaceButtonTC;
	};
#endif // DOXYGEN_SHOULD_SKIP_THIS
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	union {
		struct {
#endif // DOXYGEN_SHOULD_SKIP_THIS
			/** Current pressure applied to the Cross Face button. */
			unsigned char	FaceSensCross;
			/**
			 * @private
			 * Unused, for byte indexing u32.
			 */
			unsigned char	unused7;
			/** Current pressure applied to the Square Face button. */
			unsigned char 	FaceSensSquare;
			/**
			 * @private
			 * Unused, for byte indexing u32.
			 */
			unsigned char 	unused8;
#ifndef DOXYGEN_SHOULD_SKIP_THIS
		};

		/**
		 * @private
		 * Cross/Square Face Button pressure storage before byte indexing.
		 */
		unsigned int 	FaceButtonCS;
	};
#endif // DOXYGEN_SHOULD_SKIP_THIS
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	union {
		struct {
#endif // DOXYGEN_SHOULD_SKIP_THIS
			/** Current pressure applied to the L1 Bumper. */
			unsigned char	BumperSensL1;
			/**
			 * @private
			 * Unused, for byte indexing u32.
			 */
			unsigned char	unused9;
			/** Current pressure applied to the R1 Bumper. */
			unsigned char 	BumperSensR1;
			/**
			 * @private
			 * Unused, for byte indexing u32.
			 */
			unsigned char 	unused10;
#ifndef DOXYGEN_SHOULD_SKIP_THIS
		};

		/**
		 * @private
		 * L1/R1 pressure storage before byte indexing.
		 */
		unsigned int 	BumperSensL1R1;
	};
#endif // DOXYGEN_SHOULD_SKIP_THIS
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	union {
		struct {
#endif // DOXYGEN_SHOULD_SKIP_THIS
			/** Current pressure applied to the L2 Trigger. */
			unsigned char	TriggerSensL2;
			/**
			 * @private
			 * Unused, for byte indexing u32.
			 */
			unsigned char	unused11;
			/** Current pressure applied to the L2 Trigger. */
			unsigned char 	TriggerSensR2;
			/**
			 * @private
			 * Unused, for byte indexing u32.
			 */
			unsigned char 	unused12;
#ifndef DOXYGEN_SHOULD_SKIP_THIS
		};

		/**
		 * @private
		 * L1/R1 pressure storage before byte indexing.
		 */
		unsigned int 	TriggerSensL2R2;
	};
#endif // DOXYGEN_SHOULD_SKIP_THIS
	/** Current SIXAXIS X-Axis Tilt */
	unsigned int 		SixAxisX;
	/** Current SIXAXIS Y-Axis Tilt */
	unsigned int 		SixAxisY;
} SceCtrlData2;

/**
 * This structure is used to copy external input data into PSP internal controller buffers. 
 */
typedef struct {
    /** Unknown. Is set to 0xC by Sony. */
    unsigned int 	unk1;
    /** 
	 * Pointer to a transfer function to copy input data into a PSP internal controller buffer. 
	 * The function should return a value >= 0 on success, < 0 otherwise.
	 */
	unsigned int 	(*copyInputData)(void *pSrc, SceCtrlData2 *pDst);
} SceCtrlInputDataTransferHandler;

/**
 * @brief Read latest controller data from the controller service.
 *
 * Controller data contains current button states (including applied pressure),
 * as well as axis states.
 *
 * @note Axis state is present only in ::PSP_CTRL_MODE_ANALOG controller mode.
 *
 * @param port - Valid ::PspCtrlPort entry.
 * @param pad_data - A pointer to ::SceCtrlData2 structure that receives controller data.
 * @param count - Number of ::SceCtrlData2 structures to read.
 *
 * @see ::SceCtrlData2
 * @see ::sceCtrlPeekBufferNegative2()
 * @see ::sceCtrlReadBufferPositive2()
 */
int sceCtrlPeekBufferPositive2(unsigned int port, SceCtrlData2 *pad_data, int count);

int sceCtrlPeekBufferNegative2(unsigned int port, SceCtrlData2 *pad_data, int count);

/**
 * @brief Read new controller data from the controller service.
 *
 * Controller data contains current button states (including applied pressure),
 * as well as axis states.
 *
 * @note Axis state is present only in ::PSP_CTRL_MODE_ANALOG controller mode.
 *
 * @warning Controller data is collected once every controller sampling cycle.
 * If controller data was already read during a cycle, trying to read it again
 * will block the execution until the next one.
 *
 * @param port - Valid ::PspCtrlPort entry.
 * @param pad_data - A pointer to ::SceCtrlData2 structure that receives controller data.
 * @param count - Number of ::SceCtrlData2 structures to read.
 *
 * @see ::SceCtrlData2
 * @see ::sceCtrlReadBufferNegative2()
 * @see ::sceCtrlPeekBufferPositive2()
 */
int sceCtrlReadBufferPositive2(unsigned int port, SceCtrlData2 *pad_data, int count);

int sceCtrlReadBufferNegative2(unsigned int port, SceCtrlData2 *pad_data, int count);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif
