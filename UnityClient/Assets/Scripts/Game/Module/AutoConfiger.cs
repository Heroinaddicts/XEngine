using System;
using System.Collections;
using System.Collections.Generic;
using System.Xml;

public class AutoConfig
{
    //enums.xls

    /*ˢ������ѡ��ģʽ*/
    public enum eRefreshSiteSelectType
    {
        random = 1, /*���ѡ��*/
        sequence = 0, /*˳��ѡ��*/
    };

    /*ˢ������*/
    public enum eRefreshType
    {
        monster = 1, /*����*/
        item = 2, /*��Ʒ*/
    };

    /*��ʼ����*/
    public enum eScene
    {
        init_map_id = 1001, /*��ʼ����*/
    };

    /*��ɫ����ͨ������*/
    public enum refresh_rule
    {
        invincible_time = 3000, /*�޵�ʱ��*/
        delete_item = 1, /*�Ƿ�ɾ����ɫ���ߣ�1��ɾ��    2����ɾ����*/
        delete_weapon = 1, /*�Ƿ�ɾ����ɫ������1��ɾ��    2����ɾ����*/
        give_weapon = 3001, /*��������*/
        give_item = 2001, /*���ڵ���*/
    };

    //structs.xls

    /*λ�Ƽ�����Ϣ*/
    public class item_list
    {
        /*����ID*/
        public readonly int _item_id;
        /*����*/
        public readonly float _item_probability;
        public item_list(int item_id, float item_probability) {
            _item_id = item_id;
            _item_probability = item_probability;
        }
    };
    /*λ�Ƽ�����Ϣ*/
    public class oRefreshInfo
    {
        /*��Դid*/
        public readonly int _resource_config_id;
        /*Ȩ��*/
        public readonly float _probability;
        public oRefreshInfo(int resource_config_id, float probability) {
            _resource_config_id = resource_config_id;
            _probability = probability;
        }
    };
    /*λ�Ƽ�����Ϣ*/
    public class weapon_list
    {
        /*����ID*/
        public readonly int _weapon_id;
        /*����*/
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
        public string name; /*����*/
        public float probability; /*�������*/
        public int min; /*��������*/
        public int max; /*��������*/
        public float item_probability; /*���������*/
        public IList<item_list> item_drop;/*���ߵ�����Ʒ*/
        public float weapon_probability; /*���������*/
        public IList<weapon_list> weapon_drop;/*������Ʒ*/
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
        public string name; /*��������*/
        public string desc; /*��������*/
        public string pic; /*����ͼƬ*/
        public int type; /*�������ͣ�1:������  2:�ռ���  3:ʹ����  4:�����࣬ʰȡ����������Ч�������������*/
        public string resources; /*ʵ����Դ�����ߵ���ʵģ����Դ��*/
        public int number; /*�ѵ�����*/
        public int trigger; /*����ID(-1��ʶ�޴���)*/
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
        public string name; /*��ͼ����*/
        public string desc; /*��ͼ����(��������)*/
        public string pic; /*ͼƬչʾ����ͼѡ���չʾ���棩*/
        public int number_max; /*�������ƣ�����������ƣ�*/
        public int fight; /*�Ƽ�ս������ʱ����Ϊ�����жϣ�*/
        public string resources; /*��Դ·��*/
        public int team; /*���(0������1����)*/
        public int kill; /*PK(0������1����)*/
        public int revive; /*������(0������1����)*/
        public int dead_item; /*��������(0������1����)*/
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
        public string name; /*����*/
        public string desc; /*����*/
        public string icon; /*icon*/
        public string resources; /*��Դid*/
        public int type; /*�������ͣ�1:��ͨ  2:��Ӣ  3:boss  4:����BOSS��*/
        public int life; /*����ֵ*/
        public int damage; /*������*/
        public int speed_attack; /*�����ٶȣ����룩*/
        public float speed_move; /*�ƶ��ٶ�*/
        public float range_alert; /*���䷶Χ*/
        public float range_track; /*׷�پ���*/
        public int dead_item_group; /*��������(������)*/
        public string effect_attack; /*������Ч*/
        public string effect_accept; /*�ܻ���Ч*/
        public string sound_attack; /*������Ч*/
        public string sound_accept; /*�ܻ���Ч*/
        public string sound_dead; /*������Ч*/
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
        public int id; /*ˢ�µ�ID*/
        public string desc; /*ˢ�µ�����*/
        public int refresh_type; /*ˢ������ eRefreshType
1:����
2:��Ʒ*/
        public float range_section; /*ˢ�µ㷶Χ*/
        public int site_select_type; /*ˢ�µ����� eRefreshSiteSelectType
1:�����  
2:˳���ͣ������б�˳��ˢ�£����Ը��ʣ�*/
        public IList<oRefreshInfo> resources_infos;/*ˢ����Դ�б�*/
        public int interval; /*ˢ�¼�������룩*/
        public int repeat; /*�ظ�ˢ�£�0��ֻ��ˢ��1��   1�������ظ�ˢ�£�*/
        public int number_stop; /*��������*/
        public int number_max; /*�������*/
        public string effect; /*ˢ����Ч*/
        public string sound; /*ˢ����Ч*/
        public int range_trigger; /*������Χ*/
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
        public string name; /*����*/
        public string desc; /*����*/
        public string icon; /*icon*/
        public string resources; /*��ԴID*/
        public int life; /*Ѫ��*/
        public int weapon_near; /*��ս��������ʼ������-1û��)*/
        public int weapon_gun; /*Զ����������ʼ������-1û��)*/
        public float speed_move; /*�ƶ��ٶ�*/
        public float speed_rotate; /*ת���ٶ�*/
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
        public string name; /*����*/
        public string desc; /*����*/
        public string pic; /*״̬ͼƬ*/
        public int type; /*����*/
        public string effect; /*��Ч*/
        public string sound; /*��Ч*/
        public float time; /*����ʱ��*/
        public float value_1; /*����1*/
        public float value_2; /*����2*/
        public float value_3; /*����3*/
        public float value_4; /*����4*/
        public float value_5; /*����5*/
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
        public int id; /*����id*/
        public string name; /*��������*/
        public string desc; /*��������*/
        public string icon; /*����icon*/
        public int type_weapon; /*��������(1:��ս   2:ǹе   3��Ͷ��)*/
        public int damage; /*�˺������ι�����ɵ��˺���*/
        public float damage_Range; /*�˺���Χ(AOE�˺�)*/
        public float speed_attack; /*�������(����)*/
        public float crit_pro; /*��������*/
        public float crit_damage; /*�����˺�*/
        public int magazine; /*��ϻ��*/
        public int type_penetrate; /*������ͣ�1:����   2:���䣩*/
        public int type_shoot; /*��͸������0:���ܴ�͸  ��*/
        public int consume_item; /*��������(ÿ�ι���ʱ���ĵĵ���iD)*/
        public string resources; /*��Դ·��*/
        public string resources_bullet; /*�ӵ�·��*/
        public int quality; /*Ʒ��*/
        public float distance; /*��������*/
        public float speed_flight; /*�����ٶȣ�ǹеΪ�����ٶȣ�����Ϊ�����ٶȣ�*/
        public float speed_move; /*�ٶȱ�����ָ�����ʱ���ƶ��ٶȱ��ʱ仯��*/
        public string sound_attack; /*������Ч*/
        public string sound_reload; /*������Ч*/
        public string sound_hit; /*�ܻ���Ч(�ܻ���͹�����ͬ���ʲ���������ͬʱ����)*/
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
