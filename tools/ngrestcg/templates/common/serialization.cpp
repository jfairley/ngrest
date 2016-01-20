// serialize type $($thisElementValue)
### /// var $($name)It->second
### /// node $($name)ObjItem
##switch $(.type)
\
##case generic
##ifneq($(.name.!match/bool/),true)
        char $($name)Buff[NUM_TO_STR_BUFF_SIZE];
        NGREST_ASSERT(::ngrest::toCString($($var), $($name)Buff, NUM_TO_STR_BUFF_SIZE), "Failed to serialize $(.nsName)");
##endif
        $($node) = context->pool.alloc< ::ngrest::Value>(::ngrest::ValueType::\
##ifeq($(.name.!match/bool/),true)
Boolean\
##else
Number\
##endif
, \
##ifneq($(.name.!match/bool/),true)
$($name)Buff\
##else
$($var) ? "true" : "false"\
##endif
);
##case string
        $($node) = context->pool.alloc< ::ngrest::Value>(::ngrest::ValueType::String, $($var).c_str());
##case enum
        $($node) = context->pool.alloc< ::ngrest::Value>(::ngrest::ValueType::String, $(.nsName)Serializer::toCString($($var));
##case struct||typedef
        $($node) = context->pool.alloc< ::ngrest::Object>();
        $(.nsName)Serializer::serialize(context->pool, $($var), $($node));
##case template
\
// count = $(.templateParams.$count)
##switch $(.name)
\
### /// list
##case vector||list
        ::ngrest::Array* $($name)Array = context->pool.alloc< ::ngrest::Array>();
        $($node) = $($name)Array;
        ::ngrest::LinkedNode* $($name)ArrayItemLast = nullptr;
        for (auto $($name)It = $($var).begin(), $($name)End = $($var).end(); $($name)It != $($name)End; ++$($name)It) {
            ::ngrest::LinkedNode* $($name)ArrayItem = context->pool.alloc< ::ngrest::LinkedNode>();
            if ($($name)ArrayItemLast == nullptr) {
                $($name)Array->firstChild = $($name)ArrayItem;
            } else {
                $($name)ArrayItemLast->nextSibling = $($name)ArrayItem;
            }
            $($name)ArrayItemLast = $($name)ArrayItem;
\
##context $(.templateParams.templateParam1)
##pushvars
##var var (*$($name)It)
##var node $($name)ArrayItem->node
##var name $($name)Item
##indent +
##include <common/serialization.cpp>
##indent -
##popvars
##endcontext
        }
\
### /// map
##case map||unordered_map
        ::ngrest::Object* $($name)Obj = context->pool.alloc< ::ngrest::Object>();
        $($node) = $($name)Obj;
        ::ngrest::NamedNode* $($name)ObjItemLast = nullptr;
        for (auto $($name)It = $($var).begin(), end = $($var).end(); $($name)It != end; ++$($name)It) {
### // key
\
##var inlineValue
##switch $(.templateParams.templateParam1.type)
##case generic
##ifneq($(.templateParams.templateParam1.name.!match/bool/),true)
        char $($name)BuffItem[NUM_TO_STR_BUFF_SIZE];
        NGREST_ASSERT(::ngrest::toCString($($name)It->first, $($name)BuffItem, NUM_TO_STR_BUFF_SIZE), "Failed to serialize $(.nsName)");
##var inlineValue $($name)BuffItem
##else
##var inlineValue $($name)It->first ? "true" : "false"
##endif
##case string
##var inlineValue $($name)It->first.c_str()
##case enum
##var inlineValue $(.templateParams.templateParam1.nsName)Serializer::toCString($($name)It->first)
##default
##error Cannot serialize $(.templateParams.templateParam1) as response of $(service.name)/$(operation.name)
##endswitch
\
            ::ngrest::NamedNode* $($name)ObjItem = context->pool.alloc< ::ngrest::NamedNode>($($inlineValue));
            if ($($name)ObjItemLast == nullptr) {
                $($name)Obj->firstChild = $($name)ObjItem;
            } else {
                $($name)ObjItemLast->nextSibling = $($name)ObjItem;
            }
            $($name)ObjItemLast = $($name)ObjItem;

\
##context $(.templateParams.templateParam2)
##pushvars
##var var $($name)It->second
##var node $($name)ObjItem->node
##var name $($name)Item
##indent +
##include <common/serialization.cpp>
##indent -
##popvars
##endcontext
        }
\
### /// unsupported
##default
##error Serialization of template type $(.nsName) is not implemented
### /// end of template
##endswitch
\
##default
##error Serialization of type is not supported: $($thisElementValue): $(.type)
##endswitch
// serialize type $($thisElementValue) end