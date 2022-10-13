using System;
using System.Collections;
using System.Collections.Generic;
using System.Xml;

public class AutoConfig
{
    //enums.xls

    /*刷新坐标选择模式*/
    public enum eRefreshSiteSelectType
    {
        random = 1, /*随机选择*/
        sequence = 0, /*顺序选择*/
    };

    /*刷新类型*/
    public enum eRefreshType
    {
        monster = 1, /*怪物*/
        item = 2, /*物品*/
    };

    /*初始场景*/
    public enum eScene
    {
        init_map_id = 1001, /*初始场景*/
    };

    /*角色复活通用配置*/
    public enum refresh_rule
    {
        invincible_time = 3000, /*无敌时间*/
        delete_item = 1, /*是否删除角色道具（1：删除    2：不删除）*/
        delete_weapon = 1, /*是否删除角色武器（1：删除    2：不删除）*/
        give_weapon = 3001, /*给予武器*/
        give_item = 2001, /*基于道具*/
    };

    //structs.xls

    /*位移计算信息*/
    public class item_list
    {
        /*道具ID*/
        public readonly int _item_id;
        /*概率*/
        public readonly float _item_probability;
        public item_list(int item_id, float item_probability) {
            _item_id = item_id;
            _item_probability = item_probability;
        }
    };
    /*位移计算信息*/
    public class oRefreshInfo
    {
        /*资源id*/
        public readonly int _resource_config_id;
        /*权重*/
        public readonly float _probability;
        public oRefreshInfo(int resource_config_id, float probability) {
            _resource_config_id = resource_config_id;
            _probability = probability;
        }
    };
    /*位移计算信息*/
    public class weapon_list
    {
        /*武器ID*/
        public readonly int _weapon_id;
        /*概率*/
        public readonly float _weapon_probability;
        public weapon_list(int weapon_id, float weapon_probability) {
            _weapon_id = weapon_id;
            _weapon_probability = weapon_probability;
        }
    };
    //drop_group.xls
    public class o_config_drop_group
    {
        public int id; /*ID*/
        public string name; /*描述*/
        public float probability; /*掉落概率*/
        public int min; /*数量下限*/
        public int max; /*数量上限*/
        public float item_probability; /*道具组概率*/
        public IList<item_list> item_drop;/*道具掉落物品*/
        public float weapon_probability; /*武器组概率*/
        public IList<weapon_list> weapon_drop;/*掉落物品*/
    }
    public IDictionary<int, o_config_drop_group> o_config_drop_group_map;
    void init_config_drop_group(string path)
    {
        o_config_drop_group_map = new Dictionary<int, o_config_drop_group>();

        XmlDocument doc =  _xmlloader(path, "drop_group");
        XmlNode root = doc.SelectSingleNode("root");

        XmlNode node = root.FirstChild;
        while(null != node)
        {
            o_config_drop_group o = new o_config_drop_group();
            o.id = (int)float.Parse(node.Attributes["id"].Value);
            o.name = node.Attributes["name"].Value;
            o.probability = float.Parse(node.Attributes["probability"].Value);
            o.min = (int)float.Parse(node.Attributes["min"].Value);
            o.max = (int)float.Parse(node.Attributes["max"].Value);
            o.item_probability = float.Parse(node.Attributes["item_probability"].Value);

            o.item_drop = new List<item_list>();
            string[] item_drop_values = node.Attributes["item_drop"].Value.Split(';');
            for (int i = 0; i < item_drop_values.Length; i++)
            {
                string[] item_list_values = item_drop_values[i].Split(',');
                item_list oo = new item_list((int)float.Parse(item_list_values[0]), float.Parse(item_list_values[1]));

                o.item_drop.Add(oo);
            }
            o.weapon_probability = float.Parse(node.Attributes["weapon_probability"].Value);

            o.weapon_drop = new List<weapon_list>();
            string[] weapon_drop_values = node.Attributes["weapon_drop"].Value.Split(';');
            for (int i = 0; i < weapon_drop_values.Length; i++)
            {
                string[] weapon_list_values = weapon_drop_values[i].Split(',');
                weapon_list oo = new weapon_list((int)float.Parse(weapon_list_values[0]), float.Parse(weapon_list_values[1]));

                o.weapon_drop.Add(oo);
            }

            o_config_drop_group_map.Add(o.id, o);
            node = node.NextSibling;
        }
    }
    //item.xls
    public class o_config_item
    {
        public int id; /*id*/
        public string name; /*道具名称*/
        public string desc; /*道具描述*/
        public string pic; /*道具图片*/
        public int type; /*道具类型（1:货币类  2:收集类  3:使用类  4:触发类，拾取到会立即生效，不会进背包）*/
        public string resources; /*实体资源（道具的真实模型资源）*/
        public int number; /*堆叠数量*/
        public int trigger; /*触发ID(-1标识无触发)*/
    }
    public IDictionary<int, o_config_item> o_config_item_map;
    void init_config_item(string path)
    {
        o_config_item_map = new Dictionary<int, o_config_item>();

        XmlDocument doc =  _xmlloader(path, "item");
        XmlNode root = doc.SelectSingleNode("root");

        XmlNode node = root.FirstChild;
        while(null != node)
        {
            o_config_item o = new o_config_item();
            o.id = (int)float.Parse(node.Attributes["id"].Value);
            o.name = node.Attributes["name"].Value;
            o.desc = node.Attributes["desc"].Value;
            o.pic = node.Attributes["pic"].Value;
            o.type = (int)float.Parse(node.Attributes["type"].Value);
            o.resources = node.Attributes["resources"].Value;
            o.number = (int)float.Parse(node.Attributes["number"].Value);
            o.trigger = (int)float.Parse(node.Attributes["trigger"].Value);

            o_config_item_map.Add(o.id, o);
            node = node.NextSibling;
        }
    }
    //map.xls
    public class o_config_map
    {
        public int id; /*id*/
        public string name; /*地图名称*/
        public string desc; /*地图描述(文字描述)*/
        public string pic; /*图片展示（地图选择的展示界面）*/
        public int number_max; /*人数限制（最大人数限制）*/
        public int fight; /*推荐战力（暂时不作为条件判断）*/
        public string resources; /*资源路径*/
        public int team; /*组队(0不允许、1允许)*/
        public int kill; /*PK(0不允许、1允许)*/
        public int revive; /*允许复活(0不允许、1允许)*/
        public int dead_item; /*死亡掉落(0不允许、1允许)*/
    }
    public IDictionary<int, o_config_map> o_config_map_map;
    void init_config_map(string path)
    {
        o_config_map_map = new Dictionary<int, o_config_map>();

        XmlDocument doc =  _xmlloader(path, "map");
        XmlNode root = doc.SelectSingleNode("root");

        XmlNode node = root.FirstChild;
        while(null != node)
        {
            o_config_map o = new o_config_map();
            o.id = (int)float.Parse(node.Attributes["id"].Value);
            o.name = node.Attributes["name"].Value;
            o.desc = node.Attributes["desc"].Value;
            o.pic = node.Attributes["pic"].Value;
            o.number_max = (int)float.Parse(node.Attributes["number_max"].Value);
            o.fight = (int)float.Parse(node.Attributes["fight"].Value);
            o.resources = node.Attributes["resources"].Value;
            o.team = (int)float.Parse(node.Attributes["team"].Value);
            o.kill = (int)float.Parse(node.Attributes["kill"].Value);
            o.revive = (int)float.Parse(node.Attributes["revive"].Value);
            o.dead_item = (int)float.Parse(node.Attributes["dead_item"].Value);

            o_config_map_map.Add(o.id, o);
            node = node.NextSibling;
        }
    }
    //monster.xls
    public class o_config_monster
    {
        public int id; /*id*/
        public string name; /*名称*/
        public string desc; /*描述*/
        public string icon; /*icon*/
        public string resources; /*资源id*/
        public int type; /*怪物类型（1:普通  2:精英  3:boss  4:世界BOSS）*/
        public int life; /*生命值*/
        public int damage; /*攻击力*/
        public int speed_attack; /*攻击速度（毫秒）*/
        public float speed_move; /*移动速度*/
        public float range_alert; /*警戒范围*/
        public float range_track; /*追踪距离*/
        public int dead_item_group; /*死亡掉落(掉落组)*/
        public string effect_attack; /*攻击特效*/
        public string effect_accept; /*受击特效*/
        public string sound_attack; /*攻击音效*/
        public string sound_accept; /*受击音效*/
        public string sound_dead; /*死亡音效*/
    }
    public IDictionary<int, o_config_monster> o_config_monster_map;
    void init_config_monster(string path)
    {
        o_config_monster_map = new Dictionary<int, o_config_monster>();

        XmlDocument doc =  _xmlloader(path, "monster");
        XmlNode root = doc.SelectSingleNode("root");

        XmlNode node = root.FirstChild;
        while(null != node)
        {
            o_config_monster o = new o_config_monster();
            o.id = (int)float.Parse(node.Attributes["id"].Value);
            o.name = node.Attributes["name"].Value;
            o.desc = node.Attributes["desc"].Value;
            o.icon = node.Attributes["icon"].Value;
            o.resources = node.Attributes["resources"].Value;
            o.type = (int)float.Parse(node.Attributes["type"].Value);
            o.life = (int)float.Parse(node.Attributes["life"].Value);
            o.damage = (int)float.Parse(node.Attributes["damage"].Value);
            o.speed_attack = (int)float.Parse(node.Attributes["speed_attack"].Value);
            o.speed_move = float.Parse(node.Attributes["speed_move"].Value);
            o.range_alert = float.Parse(node.Attributes["range_alert"].Value);
            o.range_track = float.Parse(node.Attributes["range_track"].Value);
            o.dead_item_group = (int)float.Parse(node.Attributes["dead_item_group"].Value);
            o.effect_attack = node.Attributes["effect_attack"].Value;
            o.effect_accept = node.Attributes["effect_accept"].Value;
            o.sound_attack = node.Attributes["sound_attack"].Value;
            o.sound_accept = node.Attributes["sound_accept"].Value;
            o.sound_dead = node.Attributes["sound_dead"].Value;

            o_config_monster_map.Add(o.id, o);
            node = node.NextSibling;
        }
    }
    //refresh_rule.xls
    public class o_config_refresh_rule
    {
        public int id; /*刷新点ID*/
        public string desc; /*刷新点描述*/
        public int refresh_type; /*刷新种类 eRefreshType
1:怪物
2:物品*/
        public float range_section; /*刷新点范围*/
        public int site_select_type; /*刷新点类型 eRefreshSiteSelectType
1:随机型  
2:顺序型（按照列表顺序刷新，忽略概率）*/
        public IList<oRefreshInfo> resources_infos;/*刷新资源列表*/
        public int interval; /*刷新间隔（毫秒）*/
        public int repeat; /*重复刷新（0：只会刷新1次   1：可以重复刷新）*/
        public int number_stop; /*数量上限*/
        public int number_max; /*最大数量*/
        public string effect; /*刷新特效*/
        public string sound; /*刷新音效*/
        public int range_trigger; /*触发范围*/
    }
    public IDictionary<int, o_config_refresh_rule> o_config_refresh_rule_map;
    void init_config_refresh_rule(string path)
    {
        o_config_refresh_rule_map = new Dictionary<int, o_config_refresh_rule>();

        XmlDocument doc =  _xmlloader(path, "refresh_rule");
        XmlNode root = doc.SelectSingleNode("root");

        XmlNode node = root.FirstChild;
        while(null != node)
        {
            o_config_refresh_rule o = new o_config_refresh_rule();
            o.id = (int)float.Parse(node.Attributes["id"].Value);
            o.desc = node.Attributes["desc"].Value;
            o.refresh_type = (int)float.Parse(node.Attributes["refresh_type"].Value);
            o.range_section = float.Parse(node.Attributes["range_section"].Value);
            o.site_select_type = (int)float.Parse(node.Attributes["site_select_type"].Value);

            o.resources_infos = new List<oRefreshInfo>();
            string[] resources_infos_values = node.Attributes["resources_infos"].Value.Split(';');
            for (int i = 0; i < resources_infos_values.Length; i++)
            {
                string[] oRefreshInfo_values = resources_infos_values[i].Split(',');
                oRefreshInfo oo = new oRefreshInfo((int)float.Parse(oRefreshInfo_values[0]), float.Parse(oRefreshInfo_values[1]));

                o.resources_infos.Add(oo);
            }
            o.interval = (int)float.Parse(node.Attributes["interval"].Value);
            o.repeat = (int)float.Parse(node.Attributes["repeat"].Value);
            o.number_stop = (int)float.Parse(node.Attributes["number_stop"].Value);
            o.number_max = (int)float.Parse(node.Attributes["number_max"].Value);
            o.effect = node.Attributes["effect"].Value;
            o.sound = node.Attributes["sound"].Value;
            o.range_trigger = (int)float.Parse(node.Attributes["range_trigger"].Value);

            o_config_refresh_rule_map.Add(o.id, o);
            node = node.NextSibling;
        }
    }
    //role.xls
    public class o_config_role
    {
        public int id; /*id*/
        public string name; /*名称*/
        public string desc; /*描述*/
        public string icon; /*icon*/
        public string resources; /*资源ID*/
        public int life; /*血量*/
        public int weapon_near; /*近战武器（初始武器，-1没有)*/
        public int weapon_gun; /*远程武器（初始武器，-1没有)*/
        public float speed_move; /*移动速度*/
        public float speed_rotate; /*转身速度*/
    }
    public IDictionary<int, o_config_role> o_config_role_map;
    void init_config_role(string path)
    {
        o_config_role_map = new Dictionary<int, o_config_role>();

        XmlDocument doc =  _xmlloader(path, "role");
        XmlNode root = doc.SelectSingleNode("root");

        XmlNode node = root.FirstChild;
        while(null != node)
        {
            o_config_role o = new o_config_role();
            o.id = (int)float.Parse(node.Attributes["id"].Value);
            o.name = node.Attributes["name"].Value;
            o.desc = node.Attributes["desc"].Value;
            o.icon = node.Attributes["icon"].Value;
            o.resources = node.Attributes["resources"].Value;
            o.life = (int)float.Parse(node.Attributes["life"].Value);
            o.weapon_near = (int)float.Parse(node.Attributes["weapon_near"].Value);
            o.weapon_gun = (int)float.Parse(node.Attributes["weapon_gun"].Value);
            o.speed_move = float.Parse(node.Attributes["speed_move"].Value);
            o.speed_rotate = float.Parse(node.Attributes["speed_rotate"].Value);

            o_config_role_map.Add(o.id, o);
            node = node.NextSibling;
        }
    }
    //trigger.xls
    public class o_config_trigger
    {
        public int id; /*id*/
        public string name; /*名称*/
        public string desc; /*描述*/
        public string pic; /*状态图片*/
        public int type; /*类型*/
        public string effect; /*特效*/
        public string sound; /*音效*/
        public float time; /*持续时间*/
        public float value_1; /*参数1*/
        public float value_2; /*参数2*/
        public float value_3; /*参数3*/
        public float value_4; /*参数4*/
        public float value_5; /*参数5*/
    }
    public IDictionary<int, o_config_trigger> o_config_trigger_map;
    void init_config_trigger(string path)
    {
        o_config_trigger_map = new Dictionary<int, o_config_trigger>();

        XmlDocument doc =  _xmlloader(path, "trigger");
        XmlNode root = doc.SelectSingleNode("root");

        XmlNode node = root.FirstChild;
        while(null != node)
        {
            o_config_trigger o = new o_config_trigger();
            o.id = (int)float.Parse(node.Attributes["id"].Value);
            o.name = node.Attributes["name"].Value;
            o.desc = node.Attributes["desc"].Value;
            o.pic = node.Attributes["pic"].Value;
            o.type = (int)float.Parse(node.Attributes["type"].Value);
            o.effect = node.Attributes["effect"].Value;
            o.sound = node.Attributes["sound"].Value;
            o.time = float.Parse(node.Attributes["time"].Value);
            o.value_1 = float.Parse(node.Attributes["value_1"].Value);
            o.value_2 = float.Parse(node.Attributes["value_2"].Value);
            o.value_3 = float.Parse(node.Attributes["value_3"].Value);
            o.value_4 = float.Parse(node.Attributes["value_4"].Value);
            o.value_5 = float.Parse(node.Attributes["value_5"].Value);

            o_config_trigger_map.Add(o.id, o);
            node = node.NextSibling;
        }
    }
    //weapon.xls
    public class o_config_weapon
    {
        public int id; /*武器id*/
        public string name; /*武器名称*/
        public string desc; /*武器描述*/
        public string icon; /*武器icon*/
        public int type_weapon; /*武器类型(1:近战   2:枪械   3：投掷)*/
        public int damage; /*伤害（单次攻击造成的伤害）*/
        public float damage_Range; /*伤害范围(AOE伤害)*/
        public float speed_attack; /*攻击间隔(毫秒)*/
        public float crit_pro; /*暴击概率*/
        public float crit_damage; /*暴击伤害*/
        public int magazine; /*弹匣数*/
        public int type_penetrate; /*射击类型（1:点射   2:连射）*/
        public int type_shoot; /*穿透数量（0:不能穿透  ）*/
        public int consume_item; /*道具消耗(每次攻击时消耗的道具iD)*/
        public string resources; /*资源路径*/
        public string resources_bullet; /*子弹路径*/
        public int quality; /*品质*/
        public float distance; /*攻击距离*/
        public float speed_flight; /*飞行速度（枪械为出膛速度，手榴弹为飞行速度）*/
        public float speed_move; /*速度变更（持改武器时，移动速度倍率变化）*/
        public string sound_attack; /*攻击音效*/
        public string sound_reload; /*换弹音效*/
        public string sound_hit; /*受击音效(受击会和攻击不同材质产生的声音同时播放)*/
    }
    public IDictionary<int, o_config_weapon> o_config_weapon_map;
    void init_config_weapon(string path)
    {
        o_config_weapon_map = new Dictionary<int, o_config_weapon>();

        XmlDocument doc =  _xmlloader(path, "weapon");
        XmlNode root = doc.SelectSingleNode("root");

        XmlNode node = root.FirstChild;
        while(null != node)
        {
            o_config_weapon o = new o_config_weapon();
            o.id = (int)float.Parse(node.Attributes["id"].Value);
            o.name = node.Attributes["name"].Value;
            o.desc = node.Attributes["desc"].Value;
            o.icon = node.Attributes["icon"].Value;
            o.type_weapon = (int)float.Parse(node.Attributes["type_weapon"].Value);
            o.damage = (int)float.Parse(node.Attributes["damage"].Value);
            o.damage_Range = float.Parse(node.Attributes["damage_Range"].Value);
            o.speed_attack = float.Parse(node.Attributes["speed_attack"].Value);
            o.crit_pro = float.Parse(node.Attributes["crit_pro"].Value);
            o.crit_damage = float.Parse(node.Attributes["crit_damage"].Value);
            o.magazine = (int)float.Parse(node.Attributes["magazine"].Value);
            o.type_penetrate = (int)float.Parse(node.Attributes["type_penetrate"].Value);
            o.type_shoot = (int)float.Parse(node.Attributes["type_shoot"].Value);
            o.consume_item = (int)float.Parse(node.Attributes["consume_item"].Value);
            o.resources = node.Attributes["resources"].Value;
            o.resources_bullet = node.Attributes["resources_bullet"].Value;
            o.quality = (int)float.Parse(node.Attributes["quality"].Value);
            o.distance = float.Parse(node.Attributes["distance"].Value);
            o.speed_flight = float.Parse(node.Attributes["speed_flight"].Value);
            o.speed_move = float.Parse(node.Attributes["speed_move"].Value);
            o.sound_attack = node.Attributes["sound_attack"].Value;
            o.sound_reload = node.Attributes["sound_reload"].Value;
            o.sound_hit = node.Attributes["sound_hit"].Value;

            o_config_weapon_map.Add(o.id, o);
            node = node.NextSibling;
        }
    }
    public delegate XmlDocument dlgt_xml_load(string path, string name);
    dlgt_xml_load _xmlloader;

    public AutoConfig(string path, dlgt_xml_load fun)
    {
        _xmlloader = fun;

        init_config_drop_group(path);
        init_config_item(path);
        init_config_map(path);
        init_config_monster(path);
        init_config_refresh_rule(path);
        init_config_role(path);
        init_config_trigger(path);
        init_config_weapon(path);
    }
};
