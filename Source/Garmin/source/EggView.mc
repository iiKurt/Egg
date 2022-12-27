import Toybox.Graphics;
import Toybox.WatchUi;

class EggView extends WatchUi.View {
    
    var gameState;
    var font;

    var happyMac;
    var sadMac;

    var lastShownAcknowledgement = 0;

    function initialize(_gameState) {
        View.initialize();

        self.gameState = _gameState;
    }

    // Load your resources here
    function onLayout(dc as Dc) as Void {
        setLayout(Rez.Layouts.MainLayout(dc));

        self.font = Application.loadResource(Rez.Fonts.psf_font);
        self.happyMac = Application.loadResource(Rez.Drawables.happy_mac);
        self.sadMac = Application.loadResource(Rez.Drawables.sad_mac);
    }

    // Called when this View is brought to the foreground. Restore
    // the state of this View and prepare it to be shown. This includes
    // loading resources into memory.
    function onShow() as Void {
    }

    // Update the view
    function onUpdate(dc as Dc) as Void {
        // Call the parent onUpdate function to redraw the layout
        View.onUpdate(dc);

        // Mac
        var macX = (dc.getWidth() / 2) - (26 / 2);
        var macY = (dc.getHeight() / 2) - ((29 / 2) * 3);

        if (self.gameState.gameComplete) {
            dc.drawBitmap(macX, macY, self.happyMac);
        }
        else {
            dc.drawBitmap(macX, macY, self.sadMac);
        }

        dc.setColor(Graphics.COLOR_WHITE, Graphics.COLOR_BLACK);

        // Error code
        for (var i = 0; i < self.gameState.arrayLength; i++) { // For each column
            var x;
            var y;

            for (var j = 0; j < self.gameState.arrayHeight; j++) // For each row
            {
                // First number = offset
                // Second number - 6 = padding between each char
                // 10, 10
                x = ((dc.getWidth() / 2) - ((self.gameState.arrayLength / 2) * 10)) + (10 * i);
                // 14, 16
                y = ((dc.getHeight() / 2) - ((self.gameState.arrayHeight / 2) * -6)) + (16 * j);

                if (i < self.gameState.arrayBreak) // Left side
                {
                    x -= 2;
                }
                else {
                    x += 2;
                }

                dc.drawText(
                    x,
                    y,
                    font,
                    self.gameState.getHexCharacter(self.gameState.gameArray[j][i]),
                    Graphics.TEXT_JUSTIFY_LEFT
                );
            }

            // Iterating the currently changing item, draw an underline (only when game not complete)
            if (self.gameState.currentIndex == i && !self.gameState.gameComplete) {
                dc.drawLine(x, y + 15, x + 8, y + 15);
            }
        }

        // Tutorial text
        if (self.gameState.tutorialTextActive) {
            dc.setColor(Graphics.COLOR_BLUE, Graphics.COLOR_BLACK);
            drawWrappedText(dc, "Press [Select]\n to confirm");
        }

        // Easter egg text & counter
        if (self.gameState.gameComplete) {
            // Don't show the counter until it has been incremented at least once
            if (self.gameState.acknowledgementCount > 0) {
                dc.drawText(
                    (dc.getWidth() / 2),
                    (dc.getHeight() - (20 + 16)),
                    font,
                    self.gameState.acknowledgementCount,
                    Graphics.TEXT_JUSTIFY_CENTER
                );
            }

            if (self.gameState.acknowledgementCount >= 10000) {
                dc.setColor(Graphics.COLOR_YELLOW, Graphics.COLOR_BLACK);
            }

            if (self.gameState.eggEggTexts.hasKey(self.gameState.acknowledgementCount)) {
                self.lastShownAcknowledgement = self.gameState.acknowledgementCount;
                var text = self.gameState.eggEggTexts.get(self.gameState.acknowledgementCount);

                drawWrappedText(dc, text);
            }
            // Don't clear acknowledgments
            // Also make sure that the lastShownAcknowledgement index actually points to something
            else if (self.gameState.eggEggTexts.hasKey(self.lastShownAcknowledgement)){
                var text = self.gameState.eggEggTexts.get(self.lastShownAcknowledgement);

                drawWrappedText(dc, text);
            }
        }
    }

    // Called when this View is removed from the screen. Save the
    // state of this View here. This includes freeing resources from
    // memory.
    function onHide() as Void {
    }

    function drawWrappedText(dc, text) {
        drawWrappedTextR(dc, text, 0);
    }

    function drawWrappedTextR(dc, text, iteration) {
        var maxWidth = 16 + (2 * iteration);

        var printText = text.substring(0, maxWidth);

        dc.drawText(
            (dc.getWidth() / 2),
            20 + (iteration * 16),
            font,
            printText,
            Graphics.TEXT_JUSTIFY_CENTER
        );

        if (text.length() > maxWidth) {
            var remainingText = text.substring(maxWidth, text.length());

            iteration += 1;
            drawWrappedTextR(dc, remainingText, iteration);
        }
        else {
            return;
        }
    }
}
