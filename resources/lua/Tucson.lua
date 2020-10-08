local _player = nil     -- shortcut to player object, refereshed in every `onEnter` call

function onEnter(scene)
    _player = scene:getPlayer()
end 

function treasureChest_onSelect(scene, item)
    local w = scene:createModal(ModalType.Default)
    
    if _player:hasItemByName("key") then
        -- create the 'magic-space-vagina' and manually remove
        -- the key from the player's inventory. returning 
        -- true will tell the engine to remove 'treasure-chest'
        -- (aka `item`) from the scene
        local vagina = ItemFactory.createItem("magic-space-vagina")
        _player:addItem(vagina)
        _player:removeItemByName("key")
        Utils.showModal(scene, "You opened the chest!\nNow you have a Magic Space Vagina!")
        return true
    else
        Utils.showModal(scene, "You cannot open this right now")
    end

    return false
end

local firstTimeSevan = true
function sevan_select(scene, item)
    if firstTimeSevan then
        local w = scene:createModal(ModalType.Messages)
        w:pushMessage("Oh hi Tommy! How does your hobohole feel?")
        w:pushMessage("I know I was little rough on you\n that night\nLOL")
        w:pushMessage("Maybe you can help me. Do you know where I can\nfind some weed?")
        w:pushMessage("If you bring me a bag weed I'll pay you $20")
        w:pushMessage("And if you're feeling horny, let me know!\nI miss that dirty hole!")
        w:exec()
        firstTimeSevan = false
    else
        if not (_player:hasItemByName("bag-of-weed")) then
            Utils.showModal(scene, "Come back when you have some weed.")
        else
            Utils.showModal(scene, "Hey thanks man! Here's another $20")
            _player:removeItemByName('bag-of-weed')
            _player:setBalance(_player:getBalance() + 20)
        end
    end
end

function courtHouse_onEnter(scene)
    if _player:hasItemByName('bag-of-weed') or  _player:hasItemByName('bag-of-crack') then
        local res = Utils.showYesNo(scene, "You might be carrying some things that don't\nbelong in a courthouse!\nDo you still want to enter?")
        if res == true then
            while _player:hasItemByName('bag-of-weed') do
                _player:removeItemByName('bag-of-weed')
                _player:setBalance(_player:getBalance() - 50)
            end
            while _player:hasItemByName('bag-of-crack') do
                _player:removeItemByName('bag-of-crack')
                _player:setBalance(_player:getBalance() - 50)
            end
            Utils.showModal(scene, "bringing drugs into a courthouse is illegal,\nyou fucking idiot")
        end
    end
end
