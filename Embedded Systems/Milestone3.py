from gpiozero import Button, LED
from statemachine import StateMachine, State
from time import sleep
import board
import digitalio
import adafruit_character_lcd.character_lcd as characterlcd

from threading import Thread

class ManagedDisplay():
    def __init__(self):
        ##
        ## Setup the six GPIO lines to communicate with the display.
        ## This leverages the digitalio class to handle digital 
        ## outputs on the GPIO lines. There is also an analagous
        ## class for analog IO.
        ##
        ## You need to make sure that the port mappings match the
        ## physical wiring of the display interface to the 
        ## GPIO interface.
        ##
        ## compatible with all versions of RPI as of Jan. 2019
        ##
        self.lcd_rs = digitalio.DigitalInOut(board.D17)
        self.lcd_en = digitalio.DigitalInOut(board.D27)
        self.lcd_d4 = digitalio.DigitalInOut(board.D5)
        self.lcd_d5 = digitalio.DigitalInOut(board.D6)
        self.lcd_d6 = digitalio.DigitalInOut(board.D13)
        self.lcd_d7 = digitalio.DigitalInOut(board.D26)

        # Modify this if you have a different sized character LCD
        self.lcd_columns = 16
        self.lcd_rows = 2 

        # Initialise the lcd class
        self.lcd = characterlcd.Character_LCD_Mono(self.lcd_rs, self.lcd_en, 
                    self.lcd_d4, self.lcd_d5, self.lcd_d6, self.lcd_d7, 
                    self.lcd_columns, self.lcd_rows)

        # wipe LCD screen before we start
        self.lcd.clear()

    def cleanupDisplay(self):
        # Clear the LCD first - otherwise we won't be abe to update it.
        self.lcd.clear()
        self.lcd_rs.deinit()
        self.lcd_en.deinit()
        self.lcd_d4.deinit()
        self.lcd_d5.deinit()
        self.lcd_d6.deinit()
        self.lcd_d7.deinit()
        
    def clear(self):
        self.lcd.clear()

    def updateScreen(self, message):
        self.lcd.clear()
        self.lcd.message = message

    

##
## CWMachine - This is our StateMachine implementation class.
## The purpose of this state machine is to send a message in 
## morse code, blinking the red light for a dot, and the blue light
## for a dash.
##
## A dot should be displayed for 500ms. 
## A dash should be displayed for 1500ms.
## There should be a pause of 250ms between dots/dashes.
## There should be a pause of 750ms between letters.
## There should be a pause of 3000ms between words.
##
class CWMachine(StateMachine):
    "A state machine designed to display morse code messages"

    ##
    ## Our two LEDs, utilizing GPIO 18, and GPIO 23
    ##
    redLight = LED(18)
    blueLight = LED(23)

    message1 = 'SOS'
    message2 = 'OK'
    activeMessage = message1
    endTransmission = False

    ##
    ## Define these states for our machine.
    ##
    ##  off - nothing lit up
    ##  dot - red lit for 500ms
    ##  dash - blue lit for 1500ms
    ##  dotDashPause - dark for 250ms
    ##  letterPause - dark for 750ms
    ##  wordPause - dark for 3000ms
    ##
    off = State(initial = True)
    dot = State()
    dash = State()
    dotDashPause = State()
    letterPause = State()
    wordPause = State()
    screen = ManagedDisplay()
    morseDict = {
        "A" : ".-", "B" : "-...", "C" : "-.-.", "D" : "-..",
        "E" : ".", "F" : "..-.", "G" : "--.", "H" : "....",
        "I" : "..", "J" : ".---", "K" : "-.-", "L" : ".-..",
        "M" : "--", "N" : "-.", "O" : "---", "P" : ".--.",
        "Q" : "--.-", "R" : ".-.", "S" : "...", "T" : "-",
        "U" : "..-", "V" : "...-", "W" : ".--", "X" : "-..-",
        "Y" : "-.--", "Z" : "--..", "0" : "-----", "1" : ".----",
        "2" : "..---", "3" : "...--", "4" : "....-", "5" : ".....",
        "6" : "-....", "7" : "--...", "8" : "---..", "9" : "----.",
        "+" : ".-.-.", "-" : "-....-", "/" : "-..-.", "=" : "-...-",
        ":" : "---...", "." : ".-.-.-", "$" : "...-..-", "?" : "..--..",
        "@" : ".--.-.", "&" : ".-...", "\"" : ".-..-.", "_" : "..--.-",
        "|" : "--...-", "(" : "-.--.-", ")" : "-.--.-"
    }
    # Transition events for Morse output states.
    doDot = (off.to(dot) | dot.to(off))
    doDash = (off.to(dash) | dash.to(off))
    # Transition events for timing-gap states.
    doDDP = (off.to(dotDashPause) | dotDashPause.to(off))
    doLP = (off.to(letterPause) | letterPause.to(off))
    doWP = (off.to(wordPause) | wordPause.to(off))

    def on_enter_dot(self):
        # Red light comes on for 500ms
        self.redLight.on()
        sleep(0.5)
        self.redLight.off()
  
    def on_exit_dot(self):
        # Red light forced off
        self.redLight.off()

    def on_enter_dash(self):
        # Blue light comes on for 1500ms
        self.blueLight.on()
        sleep(1.5)
        self.blueLight.off()
 
    def on_exit_dash(self):
        # Blue light forced off
        self.blueLight.off()

    def on_enter_dotDashPause(self):
        # wait for 250ms

        sleep(0.25)


    def on_exit_dotDashPause(self):
        pass


    def on_enter_letterPause(self):
        # wait for 750ms

        sleep(0.75)

    def on_exit_letterPause(self):
        pass

    def on_enter_wordPause(self):
        # wait for 3000ms
        
        sleep(3)


    def on_exit_wordPause(self):
        pass


    def toggleMessage(self):
        if self.activeMessage == self.message1:
            self.activeMessage = self.message2
        else:
            self.activeMessage = self.message1


    def processButton(self):
        self.toggleMessage()

    def run(self):
        myThread = Thread(target=self.transmit)
        myThread.start()
        
    ##
    ## transmit - utility method used to continuously send a
    ## message
    ##
    def transmit(self):
        while not self.endTransmission:
            sleep(3) # between each cycle, pause 3 seconds, similar to word pause, just to better show the morse code.
            self.screen.updateScreen(f"Sending:\n{self.activeMessage}")
            wordList = self.activeMessage.split()
            for wordIndex, word in enumerate(wordList):
                for charIndex, char in enumerate(word):
                    morse = self.morseDict.get(char)
                    if morse is None:
                        continue
                    for morseIndex, symbol in enumerate(morse):
                        if symbol == ".":
                            self.doDot()
                            self.doDot()
                        elif symbol == "-":
                            self.doDash()
                            self.doDash()
                    # Timed pause between symbols in one letter
                        if morseIndex < len(morse) - 1:
                            self.doDDP()
                            self.doDDP()
                    # Timed pause between letters in one word
                    if charIndex < len(word) - 1:
                        self.doLP()
                        self.doLP()
                # Timed pause between words
                if wordIndex < len(wordList) - 1:
                    self.doWP()
                    self.doWP()
        ## Cleanup the display i.e. clear it
        self.screen.cleanupDisplay()

## End class CWMachine definition

cwMachine = CWMachine()
cwMachine.run()

greenButton = Button(24) # Green button on GPIO 24
greenButton.when_pressed = cwMachine.toggleMessage # runs the state machines toggle message when it is pressed

repeat = True
while repeat:
    try:
        ## sleep for 20 seconds at a time. This value is not crucial, 
        ## all of the work for this application is handled by the 
        ## Button.when_pressed event process
        sleep(20)
    except KeyboardInterrupt:
        ## Catch the keyboard interrupt (CTRL-C) and exit cleanly
        ## we do not need to manually clean up the GPIO pins, the 
        ## gpiozero library handles that process.
        print("Cleaning up. Exiting...")
        repeat = False
        ## Cleanly exit the state machine after completing the last message
        cwMachine.endTransmission = True
        sleep(1)