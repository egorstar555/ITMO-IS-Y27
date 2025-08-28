package com.Starygin.config;

import org.apache.kafka.clients.admin.NewTopic;
import org.apache.kafka.clients.consumer.ConsumerConfig;
import org.apache.kafka.clients.producer.ProducerConfig;
import org.apache.kafka.common.serialization.StringDeserializer;
import org.apache.kafka.common.serialization.StringSerializer;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.env.Environment;
import org.springframework.kafka.config.ConcurrentKafkaListenerContainerFactory;
import org.springframework.kafka.config.TopicBuilder;
import org.springframework.kafka.core.ConsumerFactory;
import org.springframework.kafka.core.DefaultKafkaConsumerFactory;
import org.springframework.kafka.core.DefaultKafkaProducerFactory;
import org.springframework.kafka.core.ProducerFactory;
import org.springframework.kafka.listener.ConcurrentMessageListenerContainer;
import org.springframework.kafka.requestreply.ReplyingKafkaTemplate;
import org.springframework.kafka.support.serializer.JsonDeserializer;

import java.time.Duration;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

@Configuration
public class PetKafkaConfig {

    @Autowired
    Environment environment;

    @Bean
    ProducerFactory<String, Object> petProducerFactory() {
        Map<String, Object> config = new HashMap<>();

        config.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG,
                environment.getProperty("spring.kafka.producer.bootstrap-servers"));
        config.put(ProducerConfig.KEY_SERIALIZER_CLASS_CONFIG, StringSerializer.class);
        config.put(ProducerConfig.VALUE_SERIALIZER_CLASS_CONFIG, StringSerializer.class);

        return new DefaultKafkaProducerFactory<>(config);
    }

    @Bean
    public ConcurrentKafkaListenerContainerFactory<String, Object> petKafkaListenerContainerFactory(
            ConsumerFactory<String, Object> petConsumerFactory) {
        ConcurrentKafkaListenerContainerFactory<String, Object> factory =
                new ConcurrentKafkaListenerContainerFactory<>();

        factory.setConsumerFactory(petConsumerFactory);

        return factory;
    }

    @Bean
    public ConsumerFactory<String, Object> petConsumerFactory() {
        Map<String, Object> props = new HashMap<>();
        props.put(ConsumerConfig.BOOTSTRAP_SERVERS_CONFIG,
                environment.getProperty("spring.kafka.consumer.bootstrap-servers"));
        props.put(ConsumerConfig.GROUP_ID_CONFIG, "pets-group");
        props.put(ConsumerConfig.AUTO_OFFSET_RESET_CONFIG, "latest");
        props.put(JsonDeserializer.USE_TYPE_INFO_HEADERS, false);
        props.put(ConsumerConfig.KEY_DESERIALIZER_CLASS_CONFIG, StringDeserializer.class);
        props.put(ConsumerConfig.VALUE_DESERIALIZER_CLASS_CONFIG, StringDeserializer.class);
        props.put(JsonDeserializer.TRUSTED_PACKAGES, "*");

        return new DefaultKafkaConsumerFactory<>(props);
    }

    private static final List<String> TOPIC_NAMES = List.of(
            "pets.findAll",
            "pets.findOne",
            "pets.findByName",
            "pets.create-pet",
            "pets.update-pet",
            "pets.delete-pet",
            "pets.reply"
    );

    @Bean
    public List<NewTopic> createPetTopics() {
        return TOPIC_NAMES.stream()
                .map(name -> TopicBuilder.name(name)
                        .partitions(3)
                        .replicas(3)
                        .configs(Map.of("min.insync.replicas", "2"))
                        .build())
                .collect(Collectors.toList());
    }

    @Bean
    public ConcurrentMessageListenerContainer<String, Object> replyPetContainer(
            ConcurrentKafkaListenerContainerFactory<String, Object> petKafkaListenerContainerFactory) {
        ConcurrentMessageListenerContainer<String, Object> repliesContainer =
                petKafkaListenerContainerFactory.createContainer("pets.reply");
        repliesContainer.getContainerProperties().setGroupId("pet");
        repliesContainer.setAutoStartup(false);

        return repliesContainer;
    }

    @Bean
    public ReplyingKafkaTemplate<String, Object, Object> replyingPetTemplate(
            ProducerFactory<String, Object> petProducerFactory,
            ConcurrentMessageListenerContainer<String, Object> replyPetContainer) {
        ReplyingKafkaTemplate<String, Object, Object> replyTemplate =
                new ReplyingKafkaTemplate<>(petProducerFactory,
                        replyPetContainer);
        replyTemplate.setDefaultReplyTimeout(Duration.ofSeconds(10));
        replyTemplate.setSharedReplyTopic(true);

        return replyTemplate;
    }
}
