import Toybox.Lang;
import Toybox.Timer;

// TODO: Make Singleton
class GameState {
    var view;

    var gameArray;
    const arrayHeight = 2;
    const arrayLength = 8;
    const arrayBreak = 4;

    const startIndex = arrayLength - 1;
    var currentIndex = startIndex;

    var gameTimer;
    var tutorialTextActive = true;
    var gameComplete = false;
    var acknowledgementCount = 0;

    // TODO: incorporate colors directly into dict
    var eggEggTexts = {
        3 => "You won!",
        4 => "Yep...",
        5 => "You sure did.",
        6 => "Would you like a trophy?",
        7 => "Sorry, we're all out of stock.",
        9 => "Could you please stop pressing [Select]?",
        10 => "ZZZzzzzZZZzzZZZzzZzz",
        11 => "zzzZZZZzzzZZzzzZZzZZ",
        13 => "Agh! I'm trying to sleep here...",
        15 => "Go away please!",
        20 => "I don't have anything of value here...",
        30 => "I am impressed by your persistence.",
        50 => "Ok this is it, or is it?",
        69 => "Nice.",
        100 => "Do you really have nothing better to do?",
        420 => "420: Phrase it.",
        500 => "This must be getting old...",
        1000 => "R.I.P. button.",
        2002 => "Ayy",
        5000 => "I don't have anything for you.",
        10000 => "Fine: 🏆", // 🏆
        10001 => "Yeah this isn't a unicode font, ain't no trophies here",
        10002 => "Only corruption.",
        10004 => "~\\_(^_^)_/~"
    };

    function initialize() {
        // Shout out to all the Java programmers in the house
        self.gameArray = new [arrayHeight] as Array<Array>;

        // Initialize the sub-arrays
        for( var i = 0; i < arrayHeight; i += 1 ) {
            self.gameArray[i] = new [arrayLength] as Array<Number>;
        }

        Math.srand(System.getTimer()); 

        for (var i = 0; i < arrayLength; i += 1) {
            // Top array
            self.gameArray[0][i] = (Math.rand() % 16).toNumber();
            // Bottom array
            self.gameArray[1][i] = 0;
        }
    }

    function start(_view) {
        self.view = _view;

        self.gameTimer = new Timer.Timer();
        self.gameTimer.start(method(:timerCallback), aSensibleTime(), true);
    }

    function timerCallback() {
        self.gameArray[1][self.currentIndex] = Math.rand() % 16; // Generate random number;

        self.view.requestUpdate();
    }

    function submit() {
        self.tutorialTextActive = false;

        if (self.gameComplete) {
            self.acknowledgementCount++;
            return;
        }

        if (self.gameArray[1][self.currentIndex] == self.gameArray[0][self.currentIndex]) {
            if (self.currentIndex - 1 < 0) { // End of the row, game complete
                self.gameTimer.stop();
                //self.view.requestUpdate();

                self.gameComplete = true;
            }
            else {
                self.currentIndex--;

                self.gameTimer.stop();
                self.gameTimer.start(method(:timerCallback), aSensibleTime(), true);
            }
        }
        else if (self.currentIndex < self.startIndex) { // Pressed at wrong time
            self.currentIndex++; // Go back
        }
    }

    // Utility functions
    function getHexCharacter(n) as String {
        return (n > 9) ? (n - 10) + 'A' : n + '0';
    }

    function map(x, in_min, in_max, out_min, out_max) as Long {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    function aSensibleTime() as Number {
        //return map(currentIndex, 0, startIndex, 500, 1500); // Hard mode
        return map(self.currentIndex, 0, self.startIndex, 1500, 500); // EZ mode
    }
}
