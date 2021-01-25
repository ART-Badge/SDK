/*****************************************************
** 文 件 名： touch.js
** 内容简述：设置默认 js 代码
** 创建日期：2020-12-13 12:35:28
** 修改记录：
    日期    版本      修改人     修改内容
2020-12-13 1.0.0 LAPTOP-T79B2D91 设置默认 js 代码
******************************************************/
module.exports = {
    TouchMode : {
        cancel: 0,
        press: 1, // 长按
        move: 2,  // 移动
    },

    PageTouchInit : function (page) {
        page.touchStartY = 0;// panel中触摸的Y坐标
        page.touchStartPosition = 0;// page中开始触摸的坐标
        page.touchEndPosition = 0;// page中结束触摸的坐标
        page.touchStatus = this.TouchMode.cancel;
        page.touchTimer = 0;
        page.navigateEnable = true;
    },

    PageTouchUninit : function (page) {
        var that = page;
        if (that.touchTimer != 0) {
            clearInterval(that.touchTimer);
            that.touchTimer = 0;
        }
    },

    PageTouchEvent : function (page, event, longPress, R2L, L2R, T2D, D2T) {
        var that = page;
        var touchItem = event.touchs[0];
        var this_module = this;
        if (touchItem.type == "touchstart") {
            //console.log(" >>> touchStart")
            /**长按操作**/
            that.touchStatus = this_module.TouchMode.press;
            that.touchStartPosition = { x: touchItem.x, y: touchItem.y };
            //console.log("that.touchTimer: ", that.touchTimer);
            if (that.touchTimer != 0) {
                clearInterval(that.touchTimer);
                that.touchTimer = 0;
            }

            that.touchTimer = setTimeout(function () {
                //console.log(">> long press");
                clearInterval(that.touchTimer);
                that.touchTimer = 0;
                if (that.touchStatus == this_module.TouchMode.press) {
                    if (typeof (longPress) == "function") {
                        longPress();
                    }
                }
            }, 1000);

        } else if (touchItem.type == "touchmove") {
            //console.log(" >>> touch move")
            that.touchStatus = this_module.TouchMode.move;
            that.touchEndPosition = { x: touchItem.x, y: touchItem.y };
        } else if (touchItem.type == "touchend") {
            console.log(" >>> touch end")
            if (that.touchStatus == this_module.TouchMode.move) {
                var d_ValueX = that.touchEndPosition.x - that.touchStartPosition.x
                var d_ValueY = that.touchEndPosition.y - that.touchStartPosition.y
                // console.log(" x : " + d_ValueX);
                // console.log(" y : " + d_ValueY);
                // console.log("  that.navigateEnable : " +  that.navigateEnable);
                if (d_ValueY > 50 && that.navigateEnable == true) {
                    console.log("slide down")
                    if (typeof (T2D) == "function") {
                        T2D();
                        return;
                    }
                } else if (d_ValueY < -50 && that.navigateEnable == true) {
                    console.log("slide up")
                    if (typeof (D2T) == "function") {
                        D2T();
                        return;
                    }
                }

                if (d_ValueX > 50 && that.navigateEnable == true) {
                    console.log("slide right")
                    if (typeof (L2R) == "function") {
                        L2R();
                        return;
                    }
                } else if (d_ValueX < -50 && that.navigateEnable == true) {
                    console.log("slide left")
                    if (typeof (R2L) == "function") {
                        R2L();
                        return;
                    }
                }
            }
            that.touchStatus = this_module.TouchMode.cancel;
            if (that.touchTimer != 0) {
                clearInterval(that.touchTimer);
                that.touchTimer = 0;
            }
        } else if (touchItem.type == "touchcancel") {
            if (that.touchTimer != 0) {
                clearInterval(that.touchTimer);
                that.touchTimer = 0;
            }
        }
    }
};
