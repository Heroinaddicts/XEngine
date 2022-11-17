using System;
using System.Collections;
using System.Collections.Generic;
using TCore.Api;
using UnityEngine;

public static class TimerManager
{

    public const int Unlimied = -1;

    static int g_total_frame = 0;

    static readonly float CONST_A_FRAME_USE_TICK = Time.fixedDeltaTime * 1000;

    class Handle
    {
        public readonly iTimer _timer;
        public readonly eTimeID _id;
        public readonly object _context;

        public readonly int _delay;
        public readonly int _count;
        public readonly int _interval;

        public int _count_of_executed;
        public int _next_executed_frame;
        public bool _is_valid;


        Handle(iTimer timer, in eTimeID id, in int delay, in int count, in int interval, in object context)
        {
            _timer = timer;
            _id = id;
            _context = context;

            _delay = delay;
            _count = count;
            _interval = interval;

            _count_of_executed = 0;
            _next_executed_frame = g_total_frame + (int)Math.Ceiling(delay / CONST_A_FRAME_USE_TICK);
            _is_valid = true;
        }
    }

    static IDictionary<int, IDictionary<int, IDictionary<int, Handle>>> _handle_map = new Dictionary<int, IDictionary<int, IDictionary<int, Handle>>>();


    static Handle findHandle(iTimer timer, in eTimeID id, in object context)
    {
        IDictionary<int, IDictionary<int, Handle>> temp = _handle_map[timer.GetHashCode()];
        if (temp != null)
        {
            IDictionary<int, Handle> temp2 = temp[(int)id];
            if (temp2 != null)
            {
                return temp2[context.GetHashCode()];
            }
        }

        return null;
    }

    static Handle insertHandle(iTimer timer, in eTimeID id, in int delay, in int count, in int interval, in object context)
    {
        Handle handle = findHandle(timer, id, context);
        if (handle != null)
        {
            //Debug.LogError("")
            return null;
        }

        return null;
    }

    public static bool Launch(iTimer timer, in eTimeID id, in int delay, in int count, in int interval, in object context)
    {
        return false;
    }
}
