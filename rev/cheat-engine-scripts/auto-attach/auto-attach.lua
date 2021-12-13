PROCESS_NAME = 'Tibia.exe'

local processOpened = false;

function myattach(timer)

  -- If process exist
  if (getProcessIDFromProcessName(PROCESS_NAME) ~= nil) and (processOpened == false) then

    -- Uncomment for debugging
    print('process was found, attaching...');

    openProcess(PROCESS_NAME)
    processOpened = true;

  end

  -- If process does no longer exist, then we start look for the process.
  if (getProcessIDFromProcessName(PROCESS_NAME) == nil) then


    -- Uncomment for debugging
    -- print('Process is not running, starting to look for it...');

    processOpened = false;
    return;
  end

  -- Uncomment for debugging
  -- print('Process is running...');
end

t=createTimer(nil); -- create timer
timer_setInterval(t,5000) -- set interval (5 seconds)
timer_onTimer(t,myattach) -- start timer