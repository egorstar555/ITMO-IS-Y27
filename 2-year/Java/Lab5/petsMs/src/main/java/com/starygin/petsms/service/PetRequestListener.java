package com.starygin.petsms.service;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.starygin.petsms.service.dto.PetDto;
import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.kafka.annotation.KafkaListener;
import org.springframework.kafka.core.KafkaTemplate;
import org.springframework.kafka.support.KafkaHeaders;
import org.springframework.messaging.Message;
import org.springframework.messaging.handler.annotation.Header;
import org.springframework.messaging.support.MessageBuilder;
import org.springframework.stereotype.Component;

import java.util.List;

@Component
public class PetRequestListener {
    private final PetService petService;
    private final KafkaTemplate<String, Object> kafkaTemplate;
    private final ObjectMapper objectMapper;

    @Autowired
    public PetRequestListener(PetService petService, KafkaTemplate<String, Object> kafkaTemplate, ObjectMapper objectMapper) {
        this.petService = petService;
        this.kafkaTemplate = kafkaTemplate;
        this.objectMapper = objectMapper;
    }

    @KafkaListener(topics = "pets.findAll", groupId = "pets-group", containerFactory = "kafkaListenerContainerFactory")
    public void handleFindAll(ConsumerRecord<String, Object> record,
                              @Header(KafkaHeaders.REPLY_TOPIC) String replyTopic,
                              @Header(KafkaHeaders.CORRELATION_ID) byte[] correlationId) throws JsonProcessingException {
        List<PetDto> data = petService.findAll();
        String reply = objectMapper.writeValueAsString(data);
        sendReply(record.key(), reply, replyTopic, correlationId);
    }

    @KafkaListener(topics = "pets.findByName", groupId = "pets-group", containerFactory = "kafkaListenerContainerFactory")
    public void handleFindByName(ConsumerRecord<String, Object> record,
                                 @Header(KafkaHeaders.REPLY_TOPIC) String replyTopic,
                                 @Header(KafkaHeaders.CORRELATION_ID) byte[] correlationId) throws JsonProcessingException {
        String reply = objectMapper.writeValueAsString(petService.findByName(record.value().toString()));
        sendReply(record.key(), reply, replyTopic, correlationId);
    }

    @KafkaListener(topics = "pets.findOne", groupId = "pets-group", containerFactory = "kafkaListenerContainerFactory")
    public void handleFindOne(ConsumerRecord<String, Object> record,
                              @Header(KafkaHeaders.REPLY_TOPIC) String replyTopic,
                              @Header(KafkaHeaders.CORRELATION_ID) byte[] correlationId) throws JsonProcessingException {
        Long request = objectMapper.readValue(objectMapper.writeValueAsString(record.value()), Long.class);
        String reply = objectMapper.writeValueAsString(petService.findOne(request));

        sendReply(record.key(), reply, replyTopic, correlationId);
    }

    @KafkaListener(topics = "pets.create-pet", groupId = "pets-group", containerFactory = "kafkaListenerContainerFactory")
    public void handleCreatePet(ConsumerRecord<String, Object> record,
                                @Header(KafkaHeaders.REPLY_TOPIC) String replyTopic,
                                @Header(KafkaHeaders.CORRELATION_ID) byte[] correlationId) throws JsonProcessingException {
        String reply = objectMapper.writeValueAsString(petService.create(objectMapper.readValue(record.value().toString(), PetDto.class)));
        sendReply(record.key(), reply, replyTopic, correlationId);
    }

    @KafkaListener(topics = "pets.delete-pet", groupId = "pets-group", containerFactory = "kafkaListenerContainerFactory")
    public void handleDeletePet(ConsumerRecord<String, Object> record,
                                @Header(KafkaHeaders.REPLY_TOPIC) String replyTopic,
                                @Header(KafkaHeaders.CORRELATION_ID) byte[] correlationId) throws JsonProcessingException {
        petService.delete(objectMapper.readValue(record.value().toString(), Long.class));
        sendReply(record.key(), "", replyTopic, correlationId);
    }

    @KafkaListener(topics = "pets.update-pet", groupId = "pets-group", containerFactory = "kafkaListenerContainerFactory")
    public void handleUpdatePet(ConsumerRecord<String, Object> record,
                                @Header(KafkaHeaders.REPLY_TOPIC) String replyTopic,
                                @Header(KafkaHeaders.CORRELATION_ID) byte[] correlationId) throws JsonProcessingException {
        PetDto petDto = objectMapper.readValue(record.value().toString(), PetDto.class);
        String reply = objectMapper.writeValueAsString(petService.updatePet(petDto, petDto.id()));
        sendReply(record.key(), reply, replyTopic, correlationId);
    }

    private void sendReply(String key, Object payload, String replyTopic, byte[] correlationId) {
        Message<Object> message = MessageBuilder.withPayload(payload)
                .setHeader(KafkaHeaders.TOPIC, replyTopic)
                .setHeader(KafkaHeaders.KEY, key)
                .setHeader(KafkaHeaders.CORRELATION_ID, correlationId)
                .build();

        kafkaTemplate.send(message);
    }
}
