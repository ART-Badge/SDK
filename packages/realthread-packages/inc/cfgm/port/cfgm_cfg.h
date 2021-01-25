/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-09-16     tyx          first implementation
 */

#ifndef __CFGM_CFG_H__
#define __CFGM_CFG_H__

#ifndef CFGM_CUSTOM_CONFIGURATION

#include "cfg_mgnt.h"

#define AREA_SN_AE   1 /* 阿联酋 United Arab Emirates         */
#define AREA_SN_AF   2 /* 阿富汗 Afghanistan                  */
#define AREA_SN_AL   3 /* 阿尔巴尼亚 Albania                  */
#define AREA_SN_AO   4 /* 安哥bai拉 Angola                    */
#define AREA_SN_AR   5 /* 阿根廷 Argentina                    */
#define AREA_SN_AT   6 /* 奥地利 Austria                      */
#define AREA_SN_AU   7 /* 澳大利亚du Australia                */
#define AREA_SN_AZ   8 /* 阿塞拜疆 Azerbaijan                 */
#define AREA_SN_BD   9 /* 孟加拉 Bangladesh                   */
#define AREA_SN_BE  10 /* 比利zhi时 Belgium                   */
#define AREA_SN_BG  11 /* 保加利亚 Bulgaria                   */
#define AREA_SN_BH  12 /* 巴林 Bahrain                        */
#define AREA_SN_BI  13 /* 布隆迪 Burundi                      */
#define AREA_SN_BJ  14 /* 贝宁 Benin                          */
#define AREA_SN_BM  15 /* 百慕大 Bermuda                      */
#define AREA_SN_BN  16 /* 文莱 Brunei                         */
#define AREA_SN_BO  17 /* 玻利维亚 Bolivia                    */
#define AREA_SN_BR  18 /* 巴西 Brazil                         */
#define AREA_SN_BS  19 /* 巴哈马 Bahamas                      */
#define AREA_SN_BT  20 /* 不丹 Bhutan                         */
#define AREA_SN_BW  21 /* 博茨瓦纳 Botswana                   */
#define AREA_SN_CA  22 /* 加拿大 Canada                       */
#define AREA_SN_CF  23 /* 中非共和国 Central Africa           */
#define AREA_SN_CG  24 /* 刚果 Congo                          */
#define AREA_SN_CH  25 /* 瑞士 Switzerland                    */
#define AREA_SN_CK  26 /* 库克群岛 Cook Is.                   */
#define AREA_SN_CL  27 /* 智利 Chile                          */
#define AREA_SN_CM  28 /* 喀麦隆 Cameroon                     */
#define AREA_SN_CN  29 /* 中国 China                          */
#define AREA_SN_CO  30 /* 哥伦比亚 Colombia                   */
#define AREA_SN_CR  31 /* 哥斯达黎加 Costa Rica               */
#define AREA_SN_CU  32 /* 古巴 Cuba                           */
#define AREA_SN_CV  33 /* 佛得角群岛 Cape Verde Is.           */
#define AREA_SN_CY  34 /* 塞浦路斯 Cyprus                     */
#define AREA_SN_CZ  35 /* 捷克共和国 Czech                    */
#define AREA_SN_DE  36 /* 德国 Germany                        */
#define AREA_SN_DK  37 /* 丹麦 Denmark                        */
#define AREA_SN_DZ  38 /* 阿尔及利亚 Algeria                  */
#define AREA_SN_EC  39 /* 厄瓜多尔 Ecuador                    */
#define AREA_SN_EE  40 /* 爱沙尼亚 Estonia                    */
#define AREA_SN_EG  41 /* 埃及 Egypt                          */
#define AREA_SN_ES  42 /* 西班牙 Spain                        */
#define AREA_SN_ET  43 /* 埃塞俄比亚 Ethiopia                 */
#define AREA_SN_FI  44 /* 芬兰 Finland                        */
#define AREA_SN_FJ  45 /* 斐济 Fiji                           */
#define AREA_SN_FR  46 /* 法国 France                         */
#define AREA_SN_GA  47 /* 加蓬 Gabon                          */
#define AREA_SN_GB  48 /* 英国 Great Britain                  */
#define AREA_SN_GD  49 /* 格林纳达 Grenada                    */
#define AREA_SN_GH  50 /* 加纳 Ghana                          */
#define AREA_SN_GM  51 /* 冈比亚 Gambia                       */
#define AREA_SN_GN  52 /* 几内亚 Guinea-Bissau                */
#define AREA_SN_GQ  53 /* 赤道几内亚 Equatorial Guinea        */
#define AREA_SN_GR  54 /* 希腊 Greece                         */
#define AREA_SN_GT  55 /* 危地马拉 Guatemala                  */
#define AREA_SN_GU  56 /* 关岛 Guam                           */
#define AREA_SN_GY  57 /* 圭亚那 Guyana                       */
#define AREA_SN_HK  58 /* 香港 Hong kong                      */
#define AREA_SN_HN  59 /* 洪都拉斯 Honduras                   */
#define AREA_SN_HR  60 /* 克罗地亚 Croatia                    */
#define AREA_SN_HT  61 /* 海地 Haiti                          */
#define AREA_SN_HU  62 /* 匈牙利 Hungary                      */
#define AREA_SN_ID  63 /* 印度尼西亚 Indonesia                */
#define AREA_SN_IE  64 /* 爱尔兰 Ireland                      */
#define AREA_SN_IL  65 /* 以色列 Israel                       */
#define AREA_SN_IN  66 /* 印度 India                          */
#define AREA_SN_IQ  67 /* 伊拉克 Iraq                         */
#define AREA_SN_IR  68 /* 伊朗 Iran                           */
#define AREA_SN_IS  69 /* 冰岛 Iceland                        */
#define AREA_SN_IT  70 /* 意大利 Italy                        */
#define AREA_SN_JM  71 /* 牙买加 Jamaica                      */
#define AREA_SN_JO  72 /* 约旦 Jordan                         */
#define AREA_SN_JP  73 /* 日本 Japan                          */
#define AREA_SN_KE  74 /* 肯尼亚 Kenya                        */
#define AREA_SN_KH  75 /* 柬埔寨 Cambodia                     */
#define AREA_SN_KP  76 /* 韩国 R.O.Korea                      */
#define AREA_SN_KR  77 /* 北朝鲜 D.P.R.Korea                  */
#define AREA_SN_KW  78 /* 科威特 Kuwait                       */
#define AREA_SN_KZ  79 /* 哈萨克斯坦 Kazakhstan               */
#define AREA_SN_LA  80 /* 老挝 Laos                           */
#define AREA_SN_LB  81 /* 黎巴嫩 Lebanon                      */
#define AREA_SN_LT  82 /* 立陶宛 Lithuania                    */
#define AREA_SN_LU  83 /* 卢森堡 Luxembourg                   */
#define AREA_SN_LV  84 /* 拉托维亚 Latvia                     */
#define AREA_SN_LY  85 /* 利比亚 Libya                        */
#define AREA_SN_MA  86 /* 摩洛哥 Morocco                      */
#define AREA_SN_MC  87 /* 摩纳哥 Monaco                       */
#define AREA_SN_MD  88 /* 摩尔多瓦 Moldova                    */
#define AREA_SN_MG  89 /* 马达加斯加 Madagascar               */
#define AREA_SN_ML  90 /* 马里 Mali                           */
#define AREA_SN_MN  91 /* 蒙古 Mongolia                       */
#define AREA_SN_MO  92 /* 澳门 Macao                          */
#define AREA_SN_MR  93 /* 毛里塔尼亚 Mauritania               */
#define AREA_SN_MT  94 /* 马耳他 Malta                        */
#define AREA_SN_MU  95 /* 毛里求斯 Mauritius                  */
#define AREA_SN_MV  96 /* 马尔代夫 Maldives                   */
#define AREA_SN_MX  97 /* 墨西哥 Mexico                       */
#define AREA_SN_MY  98 /* 马来西亚 Malaysia                   */
#define AREA_SN_MZ  99 /* 莫桑比克 Mozambique                 */
#define AREA_SN_NA 100 /* 纳米比亚 Namibia                    */
#define AREA_SN_NE 101 /* 尼日尔 Niger                        */
#define AREA_SN_NG 102 /* 尼日利亚 Nigeria                    */
#define AREA_SN_NI 103 /* 尼加拉瓜 Nicaragua                  */
#define AREA_SN_NL 104 /* 荷兰 Netherlands                    */
#define AREA_SN_NO 105 /* 挪威 Norway                         */
#define AREA_SN_NP 106 /* 尼泊尔 Nepal                        */
#define AREA_SN_NZ 107 /* 新西兰 New Zealand                  */
#define AREA_SN_OM 108 /* 阿曼 Oman                           */
#define AREA_SN_PA 109 /* 巴拿马 Panama                       */
#define AREA_SN_PE 110 /* 秘鲁 Peru                           */
#define AREA_SN_PG 111 /* 巴布亚新几内亚 Papua New Guinea     */
#define AREA_SN_PH 112 /* 菲律宾 Philippines                  */
#define AREA_SN_PK 113 /* 巴基斯坦 Pakistan                   */
#define AREA_SN_PL 114 /* 波兰 Poland                         */
#define AREA_SN_PT 115 /* 葡萄牙 Portugal                     */
#define AREA_SN_PY 116 /* 巴拉圭 Paraguay                     */
#define AREA_SN_QA 117 /* 卡塔尔 Qatar                        */
#define AREA_SN_RO 118 /* 罗马尼亚 Romania                    */
#define AREA_SN_RU 119 /* 俄罗斯 Russia                       */
#define AREA_SN_RW 120 /* 卢旺达 Rwanda                       */
#define AREA_SN_SA 121 /* 沙特阿拉伯 Saudi Arabia             */
#define AREA_SN_SD 122 /* 苏丹 Sudan                          */
#define AREA_SN_SE 123 /* 瑞典 Sweden                         */
#define AREA_SN_SG 124 /* 新加坡 Singapore                    */
#define AREA_SN_SK 125 /* 斯洛伐克 Slovakia                   */
#define AREA_SN_SM 126 /* 圣马力诺 San Marino                 */
#define AREA_SN_SN 127 /* 塞内加尔 Senegal                    */
#define AREA_SN_SO 128 /* 索马里 Somalia                      */
#define AREA_SN_SY 129 /* 叙利亚 Syria                        */
#define AREA_SN_TH 130 /* 泰国 Thailand                       */
#define AREA_SN_TJ 131 /* 塔吉克斯坦 Tadzhikistan             */
#define AREA_SN_TM 132 /* 土库曼斯坦 Turkmenistan             */
#define AREA_SN_TN 133 /* 突尼斯 Tunisia                      */
#define AREA_SN_TO 134 /* 汤加 Tonga                          */
#define AREA_SN_TW 135 /* 台湾 Taiwan                         */
#define AREA_SN_TZ 136 /* 坦桑尼亚 Tanzania                   */
#define AREA_SN_UA 137 /* 乌克兰 Ukraine                      */
#define AREA_SN_UG 138 /* 乌干达 Uganda                       */
#define AREA_SN_UK 139 /* 英国 United Kingdom                 */
#define AREA_SN_US 140 /* 美国 United States                  */
#define AREA_SN_UY 141 /* 乌拉圭 Uruguay                      */
#define AREA_SN_UZ 142 /* 乌兹别克斯坦 Uzbekistan             */
#define AREA_SN_VA 143 /* 梵蒂冈 Vatican City                 */
#define AREA_SN_VE 144 /* 委内瑞拉 Venezuela                  */
#define AREA_SN_VN 145 /* 越南 Viet Nam                       */
#define AREA_SN_YE 146 /* 也门 Yemen                          */
#define AREA_SN_YU 147 /* 南斯拉夫 Yugoslavia                 */
#define AREA_SN_ZA 148 /* 南非 South Africa                   */
#define AREA_SN_ZM 149 /* 赞比亚 Zambia                       */
#define AREA_SN_ZR 150 /* 扎伊尔 Zaire                        */
#define AREA_SN_ZW 151 /* 津巴布韦 Zimbabwe                   */

rt_err_t cfgm_sys_language_handle(const struct cfgm_items *item, rt_base_t value);
rt_err_t cfgm_app_language_handle(const struct cfgm_items *item, rt_base_t value);

#define CFGM_SYS_ITEMS  \
    {"language", AREA_SN_CN, cfgm_sys_language_handle},

#define CFGM_APP_ITEMS \
    {"language", AREA_SN_CN, cfgm_app_language_handle},

#endif

#endif
