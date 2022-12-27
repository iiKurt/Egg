import Toybox.Lang;
import Toybox.System;
import Toybox.WatchUi;

class EggMenuDelegate extends WatchUi.MenuInputDelegate {

    function initialize() {
        MenuInputDelegate.initialize();
    }

    function onMenuItem(item as Symbol) as Void {
        if (item == :item_about) {
            WatchUi.pushView(new AboutView(), null, WatchUi.SLIDE_UP);
        }
    }

}
