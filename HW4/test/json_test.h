#include "../src/json_object.h"
#include "../src/string_value.h"
#include "../src/json_iterator.h"
#include "../src/visitor.h"
#include "../src/beautify_visitor.h"
#include "../src/json_scanner.h"
#include "../src/json_parser.h"
#include "../src/json_builder.h"
#include "../src/iterator_factories.h"

TEST(JSonSuite, OneKeyStringValue) {
    JsonObject *jo = new JsonObject;
    Value * v1 = new StringValue("value1");
    jo->set("key1", v1);
    ASSERT_EQ(v1, jo->getValue("key1"));
    ASSERT_EQ("{\n\"key1\":\"value1\"\n}", jo->toString());

    delete v1;
}

TEST(JSonSuite, TwoKeyStringValue) {
    JsonObject *jo = new JsonObject;
    Value * v1 = new StringValue("value1");
    jo->set("key1", v1);
    Value * v2 = new StringValue("value2");
    jo->set("key2", v2);
    ASSERT_EQ("\"value1\"", jo->getValue("key1")->toString());
    ASSERT_EQ("\"value2\"", jo->getValue("key2")->toString());
    ASSERT_EQ("{\n\"key1\":\"value1\",\n\"key2\":\"value2\"\n}", jo->toString());

    delete v1;
    delete v2;

}

TEST(JSonSuite, Composite) {
    JsonObject *jo = new JsonObject;
    Value * v1 = new StringValue("value1");
    jo->set("key1", v1);
    Value * v2 = new StringValue("value2");
    jo->set("key2", v2);

    JsonObject *j_composite = new JsonObject;
    j_composite->set("keyc", jo);
    ASSERT_EQ(jo, j_composite->getValue("keyc"));
    ASSERT_EQ("{\n\"keyc\":{\n\"key1\":\"value1\",\n\"key2\":\"value2\"\n}\n}", j_composite->toString());

    delete v1;
    delete v2;
}

TEST(JSonSuite, NestedComposite){
    JsonObject *jo = new JsonObject;
    Value * v1 = new StringValue("value1");
    jo->set("key1", v1);
    Value * v2 = new StringValue("value2");
    jo->set("key2", v2);

    JsonObject *j_composite = new JsonObject;
    j_composite->set("keyc", jo);

    JsonObject *j_composite2 = new JsonObject;
    j_composite2->set("keyc2", j_composite);

    std::string keyc2_value = j_composite2->getValue("keyc2")->toString();
    ASSERT_EQ(j_composite->toString(), keyc2_value);
    ASSERT_EQ("{\n\"keyc2\":{\n\"keyc\":{\n\"key1\":\"value1\",\n\"key2\":\"value2\"\n}\n}\n}", j_composite2->toString());

    delete v1;
    delete v2;
}

TEST(JSonSuite, CompositeGetValueException) {
    JsonObject *jo = new JsonObject;
    Value * v1 = new StringValue("value1");
    jo->set("key1", v1);
    Value * v2 = new StringValue("value2");
    jo->set("key2", v2);

    JsonObject *j_composite = new JsonObject;
    j_composite->set("keyc", jo);
    ASSERT_ANY_THROW(j_composite->getValue("key111"));

    delete v1;
    delete v2;
}

TEST(JSonSuite, EmptyObject){
    JsonObject *jo = new JsonObject;
    ASSERT_EQ("{\n\n}", jo->toString());

}

TEST(JSonSuite, SetNullValueObject){
    JsonObject * jo = new JsonObject;
    Value * v1 = new StringValue("");
    jo->set("key1", v1);

    ASSERT_EQ("{\n\"key1\":\"\"\n}", jo->toString());

    delete v1;
}

TEST(JsonIteratorSuite, Iterator) {
    JsonObject *jo = new JsonObject;
    Value * v1 = new StringValue("value1");
    jo->set("key1", v1);
    Value * v2 = new StringValue("value2");
    jo->set("key2", v2);
    Value * a1 = new StringValue("a value 1");
    jo->set("akey1", a1);

    JsonIterator * it = jo->createIterator();
    it->first();
    std::string key = it->currentKey();
    Value * value = it->currentValue();
    ASSERT_EQ("akey1", key);
    ASSERT_EQ("\"a value 1\"", value->toString());

    it->next();
    key = it->currentKey();
    value = it->currentValue();
    ASSERT_EQ("key1", key);
    ASSERT_EQ("\"value1\"", value->toString());

    it->next();
    key = it->currentKey();
    value = it->currentValue();
    ASSERT_EQ("key2", key);
    ASSERT_EQ("\"value2\"", value->toString());

    it->next();
    ASSERT_EQ(true, it->isDone());

    delete it;
    delete v1;
    delete v2;
}

TEST(JsonIteratorSuite, StringValueIterator){
    Value * v1 = new StringValue("value1");

    JsonIterator * it = v1->createIterator();
    it->first();

    ASSERT_ANY_THROW(it->currentKey());
    ASSERT_ANY_THROW(it->currentValue());
    ASSERT_EQ(true, it->isDone());

    delete it;
    delete v1;

}

TEST(JSonVisitorSuite, Visitor) {
    JsonObject *jo = new JsonObject;
    Value * v1 = new StringValue("value1");
    jo->set("key1", v1);
    Value * v2 = new StringValue("value2");
    jo->set("key2", v2);

    BeautifyVisitor visitor;
    jo->accept(&visitor);
    std::string result = visitor.getResult();

    std::string expectResult;
    expectResult += "{\n";
    expectResult += "    \"key1\": \"value1\",\n";
    expectResult += "    \"key2\": \"value2\"\n";
    expectResult += "}";
    ASSERT_EQ(expectResult, result);
}

TEST(JSonVisitorSuite, VisitorWithComposed) {
    JsonObject *jo = new JsonObject;
    Value * v1 = new StringValue("value1");
    jo->set("key1", v1);
    Value * v2 = new StringValue("value2");
    jo->set("key2", v2);

    JsonObject *j_composite = new JsonObject;
    j_composite->set("keyc", jo);
    BeautifyVisitor visitor;
    j_composite->accept(&visitor);
    std::string result = visitor.getResult();

    std::string expectResult;
    expectResult += "{\n";
    expectResult += "    \"keyc\": {\n";
    expectResult += "        \"key1\": \"value1\",\n";
    expectResult += "        \"key2\": \"value2\"\n";
    expectResult += "    }\n";
    expectResult += "}";
    ASSERT_EQ(expectResult, result);
}

TEST(JSonBuilderSuite, Builder){
    std::string jsonString = "{\n\"keyc\":{\n\"key1\":\"value1\",\n\"key2\":\"value2\"\n}\n}";

    JsonScanner scanner;
    JsonBuilder builder;
    JsonParser parser(&scanner, &builder);

    parser.setInput(jsonString);
    parser.parse();
    JsonObject * jsonObject = builder.getJsonObject();

    ASSERT_NE(jsonObject, nullptr);
    Value * valuec = jsonObject->getValue("keyc");
    ASSERT_EQ(valuec->toString(), "{\n\"key1\":\"value1\",\n\"key2\":\"value2\"\n}");

    JsonObject* nestedObject = dynamic_cast<JsonObject*>(valuec);
    Value * value1 = nestedObject->getValue("key1");
    ASSERT_EQ(value1->toString(), "\"value1\"");

    Value * value2 = nestedObject->getValue("key2");
    ASSERT_EQ(value2->toString(), "\"value2\"");
    
    delete valuec;
    delete value1;
    delete value2;
}

TEST(JSonBuilderSuite, BuilderEmptyObject){
    std::string jsonString = "{}";

    JsonScanner scanner;
    JsonBuilder builder;
    JsonParser parser(&scanner, &builder);

    parser.setInput(jsonString);
    parser.parse();
    JsonObject * jsonObject = builder.getJsonObject();

    ASSERT_NE(jsonObject, nullptr);
    ASSERT_EQ(jsonObject->toString(), "{\n\n}");

}

TEST(JsonIteratorFactorySuite, JsonObjectIteratorTest) {
    IteratorFactory * factory = JsonObjectIteratorFactory::instance();
    JsonObject *jo = new JsonObject;
    Value * v1 = new StringValue("value1");
    jo->set("key1", v1);
    Value * v2 = new StringValue("value2");
    jo->set("key2", v2);
    Value * a1 = new StringValue("a value 1");
    jo->set("akey1", a1);

    JsonIterator * it = jo->createIterator(factory);
    it->first();
    ASSERT_EQ("akey1", it->currentKey());
    ASSERT_EQ("\"a value 1\"", it->currentValue()->toString());

    it->next();
    ASSERT_EQ("key1", it->currentKey());
    ASSERT_EQ("\"value1\"", it->currentValue()->toString());

    it->next();
    ASSERT_EQ("key2", it->currentKey());
    ASSERT_EQ("\"value2\"", it->currentValue()->toString());

    it->next();
    ASSERT_EQ(true, it->isDone());

    delete it;
    delete v1;
    delete v2;
    delete a1;
    delete factory;

}

TEST(JsonIteratorFactorySuite, OrderByNameIteratorTest) {
    IteratorFactory * factory = OrderByNameIteratorFactory::instance();
    JsonObject *jo = new JsonObject;
    Value * v1 = new StringValue("value1");
    jo->set("key1", v1);
    Value * v2 = new StringValue("value2");
    jo->set("key2", v2);
    Value * a1 = new StringValue("a value 1");
    jo->set("akey1", a1);

    JsonIterator * it = jo->createIterator(factory);
    it->first();
    ASSERT_EQ("key2", it->currentKey());
    ASSERT_EQ("\"value2\"", it->currentValue()->toString());

    it->next();
    ASSERT_EQ("key1", it->currentKey());
    ASSERT_EQ("\"value1\"", it->currentValue()->toString());

    it->next();
    ASSERT_EQ("akey1", it->currentKey());
    ASSERT_EQ("\"a value 1\"", it->currentValue()->toString());

    it->next();
    ASSERT_EQ(true, it->isDone());

    delete it;
    delete v1;
    delete v2;
    delete a1;
    delete factory;

}
