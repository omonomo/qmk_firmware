# MCU name
MCU = atmega32u4

# Processor frequency.
F_CPU = 16000000

#
# LUFA specific
#
# Target architecture (see library "Board Types" documentation).
ARCH = AVR8

# Input clock frequency.
F_USB = $(F_CPU)

# Interrupt driven control endpoint task(+60)
OPT_DEFS += -DINTERRUPT_CONTROL_ENDPOINT

# Boot Section Size in *bytes*
OPT_DEFS += -DBOOTLOADER_SIZE=4096

# Bootloader
BOOTLOADER = atmel-dfu

# Build Options
MAGIC_ENABLE = no       # Magic keycordes(+600)
COMMAND_ENABLE = no	    # Commands for debug and configuration
DYNAMIC_MACRO_ENABLE = no # Dynamic Macros
GRAVE_ESC_ENABLE = no   # Grave ESC Keycordes(+100)
LEADER_ENABLE = no      # Leader key
MOUSEKEY_ENABLE = no    # Mouse keys(+4700)
PROGRAMMABLE_BUTTON_ENABLE = no # Programmable button
REPEAT_KEY_ENABLE = no  # Repeat key
SPACE_CADET_ENABLE = no # Space cadet(+400)
AUTO_SHIFT_ENABLE = no  # Auto shift
AUTOCORRECT_ENABLE = no # Autocorrect word
CAPS_WORD_ENABLE = no   # Caps word
COMBO_ENABLE = no       # Key combo
KEY_LOCK_ENABLE = no    # key press lock
KEY_OVERRIDE_ENABLE = no # key override
OS_DETECTION_ENABLE = no # OS Detect
RAW_ENABLE = no         # Raw HID
SECURE_ENABLE = no      # Lock sequence
SEND_STRING_ENABLE = yes # Send string
SEQUENCER_ENABLE = no   # MIDI sequencer
TAP_DANCE_ENABLE = no   # Tap dance
TRI_LAYER_ENABLE = no   # lower, upper, adjust layer
UNICODE_ENABLE = no     # Unicode characters can be input straight
WPM_ENABLE = no         # Word Per Minute Calculation

QUANTUM_PAINTER_ENABLE = no # Display
HD44780_ENABLE = no     # LCD
ST7565_ENABLE = no      # LCD
OLED_ENABLE = no        # OLED
BACKLIGHT_ENABLE = no   # Enable keyboard backlight functionality
LED_MATRIX_ENABLE = no  # LED Matrix
RGBLIGHT_ENABLE = no    # Enable WS2812 RGB underlight.
RGB_MATRIX_ENABLE = no  # RGB Matrix Lighting
AUDIO_ENABLE = no       # Audio output on port C6
BLUETOOTH_ENABLE = no   # Enable Bluetooth with the Adafruit EZ-Key HID
BOOTMAGIC_ENABLE = no   # Virtual DIP switch configuration(+1000)
DIGITIZER_ENABLE = no   # Mouse cursor to be placed at absolute coordinates
DIP_SWITCH_ENABLE = no  # Dip Switches
ENCODER_ENABLE = no     # Encoders
HAPTIC_ENABLE = no      # Haptic Feedback
JOYSTICK_ENABLE = no    # Joystick
MIDI_ENABLE = no        # MIDI controls
POINTING_DEVICE_ENABLE = yes # Pointing device
POINTING_DEVICE_DRIVER = custom
PS2_MOUSE_ENABLE = no   # PS/2 mouse
PS2_ENABLE = no         # PS/2
NKRO_ENABLE = yes       # USB Nkey Rollover - if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
STENO_ENABLE = no       # StenoType
EXTRAKEY_ENABLE = no    # Audio control and System control(+450)
VELOCIKEY_ENABLE= no    # Control the speed of lighting effects
SPLIT_KEYBOARD = no     # Split keyboard support
WAIT_FOR_USB = no       # Forces the keyboard to wait for a USB connection to be established before it starts up
NO_USB_STARTUP_CHECK = no # Disables usb suspend check after keyboard startup
DEFERRED_EXEC_ENABLE = no # Enables deferred executor support
DYNAMIC_TAPPING_TERM_ENABLE = no # Allows to configure the global tapping term on the fly

#CONSOLE_ENABLE = yes    # Console for debug(+400)
CONSOLE_ENABLE = no	    # Console for debug(+400)
LTO_ENABLE = yes        # Link Time Optimize (-3000)
#DEBOUNCE_TYPE = sym_eager_pk # Debounce_algolithm
DEBOUNCE_TYPE = sym_defer_pk # Debounce_algolithm
#DEBOUNCE_TYPE = sym_defer_g # Debounce_algolithm Default

#DEBOUNCE_TYPE = custom # Debounce_algolithm Custom
#SRC += d_asym_defer_eager_pk.c
SRC += func_edit_text.c\
	func_key_output.c\
	func_mod_output.c\
	func_tap_replace.c\
	sub_counter.c\
	sub_flag_mode.c\
	sub_key_mod.c\
	sub_layer.c\
	sub_mouse.c\
	sub_other.c\
	sub_override.c\
	sub_romazi.c

