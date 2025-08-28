package com.starygin.ownersms.service;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.starygin.ownersms.service.dto.OwnerDto;
import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.springframework.kafka.annotation.KafkaListener;
import org.springframework.kafka.core.KafkaTemplate;
import org.springframework.kafka.support.KafkaHeaders;
import org.springframework.messaging.Message;
import org.springframework.messaging.handler.annotation.Header;
import org.springframework.messaging.support.MessageBuilder;
import org.springframework.stereotype.Component;

@Component
public class OwnerRequestListener {
    private final OwnerService ownerService;
    private final KafkaTemplate<String, Object> kafkaTemplate;
    private final ObjectMapper objectMapper;

    public OwnerRequestListener(OwnerService ownerService,
                                KafkaTemplate<String, Object> kafkaTemplate, ObjectMapper objectMapper) {
        this.ownerService = ownerService;
        this.kafkaTemplate = kafkaTemplate;
        this.objectMapper = objectMapper;
    }

    @KafkaListener(topics = "owners.findOne", groupId = "owners-group", containerFactory = "kafkaListenerContainerFactory")
    public void handleFindOne(ConsumerRecord<String, Object> record,
                              @Header(KafkaHeaders.REPLY_TOPIC) String replyTopic,
                              @Header(KafkaHeaders.CORRELATION_ID) byte[] correlationId) throws JsonProcessingException {
        Long request = objectMapper.readValue(objectMapper.writeValueAsString(record.value()), Long.class);
        String reply = objectMapper.writeValueAsString(ownerService.findOne(request));

        sendReply(record.key(), reply, replyTopic, correlationId);
    }

    @KafkaListener(topics = "owners.findAll", groupId = "owners-group", containerFactory = "kafkaListenerContainerFactory")
    public void handleFindAll(ConsumerRecord<String, Object> record,
                              @Header(KafkaHeaders.REPLY_TOPIC) String replyTopic,
                              @Header(KafkaHeaders.CORRELATION_ID) byte[] correlationId) throws JsonProcessingException {
        String reply = objectMapper.writeValueAsString(ownerService.findAll());
        sendReply(record.key(), reply, replyTopic, correlationId);
    }

    @KafkaListener(topics = "owners.findByName", groupId = "owners-group", containerFactory = "kafkaListenerContainerFactory")
    public void handleFindByName(ConsumerRecord<String, Object> record,
                                 @Header(KafkaHeaders.REPLY_TOPIC) String replyTopic,
                                 @Header(KafkaHeaders.CORRELATION_ID) byte[] correlationId) throws JsonProcessingException {
        String reply = objectMapper.writeValueAsString(ownerService.findByName(record.value().toString()));
        sendReply(record.key(), reply, replyTopic, correlationId);
    }

    @KafkaListener(topics = "owners.create-owner", groupId = "owners-group", containerFactory = "kafkaListenerContainerFactory")
    public void handleCreateOwner(ConsumerRecord<String, Object> record,
                                  @Header(KafkaHeaders.REPLY_TOPIC) String replyTopic,
                                  @Header(KafkaHeaders.CORRELATION_ID) byte[] correlationId) throws JsonProcessingException {
        String reply = objectMapper.writeValueAsString(ownerService.create(objectMapper.readValue(record.value().toString(), OwnerDto.class)));
        sendReply(record.key(), reply, replyTopic, correlationId);
    }

    @KafkaListener(topics = "owners.delete-owner", groupId = "owners-group", containerFactory = "kafkaListenerContainerFactory")
    public void handleDeleteOwner(ConsumerRecord<String, Object> record,
                                  @Header(KafkaHeaders.REPLY_TOPIC) String replyTopic,
                                  @Header(KafkaHeaders.CORRELATION_ID) byte[] correlationId) throws JsonProcessingException {
        ownerService.delete(objectMapper.readValue(record.value().toString(), Long.class));
        sendReply(record.key(), "", replyTopic, correlationId);
    }

    @KafkaListener(topics = "owners.update-owner", groupId = "owners-group", containerFactory = "kafkaListenerContainerFactory")
    public void handleUpdateOwner(ConsumerRecord<String, Object> record,
                                  @Header(KafkaHeaders.REPLY_TOPIC) String replyTopic,
                                  @Header(KafkaHeaders.CORRELATION_ID) byte[] correlationId) throws JsonProcessingException {
        OwnerDto ownerDto = objectMapper.readValue(record.value().toString(), OwnerDto.class);
        String reply = objectMapper.writeValueAsString(ownerService.updateOwner(ownerDto, ownerDto.id()));
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