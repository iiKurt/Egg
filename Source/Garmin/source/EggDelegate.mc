import Toybox.Lang;
import Toybox.WatchUi;

class EggDelegate extends WatchUi.BehaviorDelegate {

    var view;
    var gameState;
    var pressed = 0;

    function initialize(_view, _gameState) {
        BehaviorDelegate.initialize();
        
        self.view = _view;
        self.gameState = _gameState;
    }

    function onMenu() as Boolean {
        WatchUi.pushView(new Rez.Menus.MainMenu(), new EggMenuDelegate(), WatchUi.SLIDE_UP);
        return true;
    }

    // Detect Menu behavior
    function onSelect() {
        // var label = view.findDrawableById("heading_label");
        // pressed += 1;
        // label.setText(pressed.format("%d"));

        // var bitmap = view.findDrawableById("mac_bitmap");
        // if ((pressed % 2) == 0) {
        //     bitmap.setBitmap(Application.loadResource(Rez.Drawables.sad_mac));
        // }
        // else {
        //     bitmap.setBitmap(Application.loadResource(Rez.Drawables.happy_mac));
        // }

        self.gameState.submit();
        self.view.requestUpdate();

        return true;
    }

}